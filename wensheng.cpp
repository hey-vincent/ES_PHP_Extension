#include <iostream>
#include <phpx.h>
#include "./common.h"
#include "./Http.h"

using namespace std;
using namespace php;

#define EX_NAME   "elasticsearch"
#define CLASS_NAME  "ElasticSearchClient"

#define REG_FN(f) pEx->registerFunction(PHPX_FN(f));

#define ES_Version  "1.0.0"

static Http http(1);

// Es 类
php::Class *pEsClass = NULL;


// -------------------------------------------------------------------
// 普通函数（全局函数）声明
// -------------------------------------------------------------------

// 指定参数信息
ArgInfo *pConstructorArg = new ArgInfo(2, false);
ArgInfo *esInfo_Arg = new ArgInfo(0, false);


// 1. 普通函数声明方式
void esInfo(Args &args, Variant &retval){
    cout << "Hey. Welcome to " << CLASS_NAME << ".Enjoy!" << endl;
}


// 2. 宏简化方式
PHPX_FUNCTION(esWiki){
    retval = "ElasticSearch是一个基于Lucene的搜索服务器。它提供了一个分布式多用户能力的全文搜索引擎，基于RESTful web接口。Elasticsearch是用Java开发的，并作为Apache许可条款下的开放源码发布，是当前流行的企业级搜索引擎。设计用于云计算中，能够达到实时搜索，稳定，可靠，快速，安装使用方便。";
}

PHPX_FUNCTION(esDomain){
    retval = "https://www.elastic.co";
}


// -------------------------------------------------------------------
//  类声明
// -------------------------------------------------------------------
// 类成员声明
// -------------------------------------------------------------------
Variant esHost("");
Variant esPort(0);
Variant vMessage("");

const Variant cEsFullName("ElasticSearchClient");

// -------------------------------------------------------------------
// 类方法声明
// -------------------------------------------------------------------

// 1.普通函数方式声明
// 构造函数
void ElasticSearchClient_Constructor(Object &ob, Args &args, Variant &retval){
	if (args.count() != 2){
        retval = NULL;
        vMessage = "ElasticSearchClientX的构造函数需要2个参数。";
        ob.set("message", vMessage);
        errReport(vMessage.toCString());
        return;
    }

    esHost = args[0];
    esPort = args[1];

    ob.set("host", esHost);
    ob.set("port", esPort);
    

    // pEsClass 空判断
    http.setTimeout(pEsClass->getStaticProperty("connect_timeout").toInt());
    retval=ob;
    return;
}

// 2. PHPX_METHOD宏简化声明(宏展开后，函数原型是一样的)
PHPX_METHOD(ES, add) {
    std::string sUrl, sResp,sDoc;
   if(!getUrl(_this.get("host").toString(), _this.get("port").toString(), sUrl)){
       vMessage = "ElasticSearchClient::Add URL解析错误。";
       _this.set("message", vMessage);
       errReport(vMessage.toCString());
       retval = false;
       return;
   }
   // 第一个参数的类型：字符串 - /type/index/id
   if(args[0].isString()){
	   sUrl += args[0].toString();
		// 文档数据处理
		if (args[1].isString()){
			sDoc.assign(args[1].toCString(), args[1].toString().size());
		} else if (args[1].isArray()){
			Array ary = args[1];
			sDoc.assign(args[1].jsonEncode().toCString(),args[1].jsonEncode().toString().size() );
		} else {
            vMessage = "ElasticSearchClient::add 参数类型错误";
            _this.set("message", vMessage);
            errReport(vMessage.toCString());
            retval = false;
            return;
		}
   } else if (args[0].isArray()){ // 数组参数 
	   if(!buildEsUri(args[0], sUrl)){
            vMessage = "ElasticSearchClient::add 传入的第一个参数(数组),解析错误。";
            _this.set("message", vMessage);
            errReport(vMessage.toCString());
            retval = false;
            return;
       }

       sDoc =  ((Array)args[0]).get("body").jsonEncode().toString();

   } else{
        vMessage = "ElasticSearchClient::add 参数类型错误";
        _this.set("message", vMessage);
        errReport(vMessage.toCString());
        retval = false;
        return;
   }

    // 设置请求头
    map<string,string> mHeader;
    mHeader["Content-Type"] = "application/json;charset=utf-8";

    // 请求
    http.Put(sUrl,mHeader,sDoc,sResp );
    retval = sResp;
    return;
}

/**
 * 成员方法 Query
 * @param _obj
 * @param args
 * @param returnVal
 * 两种参数类型：array(index,type,id) 或 string(uri) 
 * 返回值： 错误 false；成功 json串
 */
void ES_Query(Object &_obj, Args &args, Variant &returnVal){
    returnVal = NULL;
    if (args.count() < 1){
        vMessage = "ElasticSearchClient::Query 至少需要一个参数.";
        _obj.set("message", vMessage);
        errReport(vMessage.toCString());
        returnVal = false;
        return;
    }

    string sURL,sRes;
    if(!getUrl(_obj.get("host").toString(), _obj.get("port").toString(), sURL)){
        vMessage = "ElasticSearchClient::search URL解析错误。";
        _obj.set("message", vMessage);
        errReport(vMessage.toCString());
        returnVal = false;
        return;
    }

    // 查询的uri
    Variant sUri;
    
    if (args[0].isString()){
        sURL += args[0].toString();
    }else if(args[0].isArray()){
        if(!buildEsUri(args[0], sURL)){
            vMessage = "ElasticSearchClient::search 参数错误。";
            _obj.set("message", vMessage);
            errReport(vMessage.toCString());
            returnVal = false;
            return;
            
        }
    }else{
        vMessage = "ElasticSearchClient::search 参数错误。";
        _obj.set("message", vMessage);
        errReport(vMessage.toCString());
        returnVal = false;
        return;
        
    }
	
    http.get(sURL,"",sRes);
    returnVal = sRes;
    return;
}

/**
 * 成员方法 Delete
 * @param obj
 * @param args
 * @param ret
 */
void ES_Delete(Object &obj, Args &args, Variant &ret){

    string sUrl,sResp;
    if(!getUrl(obj.get("host").toString(), obj.get("port").toString(), sUrl)){
        vMessage = "ElasticSearchClient::remove URL解析错误。";
        obj.set("message", vMessage);
        errReport(vMessage.toCString());
        ret = false;
        return;
    }

    if (args[0].isString()){
        sUrl += args[0].toString();
    } else if(args[0].isArray()){
        if(!buildEsUri(args[0], sUrl)){
            vMessage = "ElasticSearchClient::remove 参数错误。";
            obj.set("message", vMessage);
            errReport(vMessage.toCString());
            ret = false;
            return;
        }
    } else{
        vMessage = "ElasticSearchClient::remove 方法参支持的参数类型为STRING或ARRAY";
        obj.set("message", vMessage);
        errReport(vMessage.toCString());
        ret = false;
        return;
    }
   
    http.Delete(sUrl, sResp);

    ret = sResp;

    return;
}


/**
 * 成员方法 Update
 * @param obj
 * @param args
 * @param ret
 */
void ES_Update(Object &obj, Args &args, Variant &ret){
    Args parameters;
    for(int i=0; i<args.count(); i++){ parameters.append(args[i]); }
    
    ES_Query(obj, parameters,ret);
    if(ret == false ){ return;}

    ES_Delete(obj,parameters, ret);
    if(ret == false ){ return;}

    ES_add(obj, args, ret);
    return;
}


void getMessage(Object &_this, Args &args, Variant &ret){
    ret = vMessage;
    return;
}


void showDoc(Object &_this, Args &args, Variant &ret)
{
    string strDoc = "define(\"ES_EXT_BUILD_ID\", \"100\");";
    ret = strDoc;
    return;
}


void setConTimeout(Object &_this, Args &args, Variant &ret){
     vMessage = "ElasticSearchClient::setConTimeout 参数错误。";
      _this.set("message", vMessage);
      errReport(vMessage.toCString());
      ret = false;

    // _this.setStaticProperty("connect_timeout", args[0]);
    
    ret = true;
    return;
}


void setReqTimeout(Object &_this, Args &args, Variant &ret){
      vMessage = "ElasticSearchClient::setReqTimeout 参数错误。";
      _this.set("message", vMessage);
      errReport(vMessage.toCString());
      ret = false;
      
    pEsClass->setStaticProperty("request_timeout", args[0]);
    ret = true;
    return;
}

// 扩展入口
PHPX_EXTENSION(){

    // 实例化扩展
	Extension *pEx = new Extension("ElasticSearchX", "1.0.0");

    pEx->registerConstant("ES", "ElasticSearch");

    // 注册普通函数
    // 1.普通方式 C++函数与PHP函数不用同名 TODO
    pEx->registerFunction("esInfo", esInfo, esInfo_Arg);

    // 2.宏方式: php里的函数与C++代码里的函数都是esWiki
    pEx->registerFunction(PHPX_FN(esWiki));

    // 更懒的方式...自定义
    REG_FN(esDomain)

    /*
        MINIT = onStart
        RINIT = onBeforeRequest
        MSHUTDOWN = onShutdown
        RSHUTDOWN = onAfterRequest
    */

    // Class 类提供针对扩展类的定制
    pEsClass = new Class("ElasticSearchClientX");

    // ArgInfo *pArg = new ArgInfo(2,false);

    // onStart 回调
    // 模块初始化阶段
    pEx->onStart = [pEx](){
        // 注册类方法
        pEsClass->addMethod("__construct", ElasticSearchClient_Constructor, PUBLIC, nullptr);//pConstructorArg

        // 宏简化(写起来简单，但有局限,也不方便IDE跳转) ： add -> Add
        pEsClass->addMethod(PHPX_ME(ES, add), PUBLIC);

        // 更直观，约束少
        pEsClass->addMethod("get", ES_Query, PUBLIC, nullptr);

        pEsClass->addMethod("remove", ES_Delete, PUBLIC);

        pEsClass->addMethod("update", ES_Update, PUBLIC);

        pEsClass->addMethod("showEsDocument", showDoc, PUBLIC);

        pEsClass->addMethod("getMessage", getMessage, PUBLIC);

        pEsClass->addMethod("setConnectTimeout", setConTimeout, PUBLIC | STATIC);

        pEsClass->addMethod("setRequestTimeout", setReqTimeout, PUBLIC | STATIC);

        // 注册类成员
        // 普通成员
        pEsClass->addProperty("host", esHost, PUBLIC);
        pEsClass->addProperty("port", esPort, PUBLIC);
        pEsClass->addProperty("message", vMessage, PUBLIC);
        // 常量
        pEsClass->addConstant("ES_FULL_NAME", cEsFullName);
        pEsClass->addConstant("VERSION", "1.0.0");
        // 静态成员
        pEsClass->addProperty("version","0.0.0", STATIC);
        pEsClass->addProperty("connect_timeout", 1, STATIC);
        pEsClass->addProperty("request_timeout", 2, STATIC);

        // 注册类
        pEx->registerClass(pEsClass);
    };


    // 请求结束回调
    pEx->onAfterRequest = []() {

        if(NULL != esInfo_Arg) {
            delete esInfo_Arg;
            // esInfo_Arg = NULL;
        }

        if(NULL != pConstructorArg){
            delete pConstructorArg;
            pConstructorArg = NULL;
        } 

        // if(NULL != pEsClass){
        //     delete pEsClass;
        //     pEsClass = NULL;
        // } 
    };

    // Before Request
    // 模块激活 RINT
    pEx->onBeforeRequest = [](){ };

    // Extension Shutdown
    pEx->onShutdown = [](){

        if(NULL != pEsClass){
            delete pEsClass;
            pEsClass = NULL;
        } 
     };

    pEx->info (
        {"ElasticSearchX: A ElasticSearch Client based on PHP-X","loaded"},
        {
            {"author", "wensheng"},
            {"version", ES_Version },
            { "date", "2017-11-22" }
        }
    );

	return pEx;
}




#include <iostream>
#include <phpx.h>
#include "./common.h"
#include "./Http.h"

using namespace std;
using namespace php;

/**
 *  * 扩展名：elasticsearch
    * 类：ElasticsearchClient
    * 成员方法：
        * construct(host,port)
        * add(doc)
        * delete(query)
        * update(query,modify)
        * query(query)
    * 成员变量
        * host
        * port
    * 静态变量
        * version
    * 全局方法
        * esWiki
        * esDomain
        * esInfo
 */

#define EX_NAME   "elasticsearch"
#define CLASS_NAME  "ElasticSearchClient"

#define REG_FN(f) pEx->registerFunction(PHPX_FN(f));

#define ES_Version  "1.0.0"

static Http http;


// -------------------------------------------------------------------
// 普通函数（全局函数）声明
// -------------------------------------------------------------------

// 指定参数信息
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

const Variant cEsFullName("ElasticSearchClient");

// -------------------------------------------------------------------
// 类方法声明
// -------------------------------------------------------------------

// 1.普通函数方式声明
// 构造函数
ArgInfo *pConstructorArg = new ArgInfo(2, false);
void ElasticSearchClient_Constructor(Object &ob, Args &args, Variant &retval){
	if (args.count() != 2){
        retval = NULL;
        errReport("ElasticSearchClient的构造函数需要2个参数。");
        return;
    }

    esHost = args[0];
    esPort = args[1];

    ob.set("host", esHost);
    ob.set("port", esPort);

    retval=ob;
    return;
}

// 2. PHPX_METHOD宏简化声明(宏展开后，函数原型是一样的)
PHPX_METHOD(ES, Add) {
    std::string sUrl, sResp,sDoc;
   if(!getUrl(_this.get("host").toString(), _this.get("port").toString(), sUrl)){
       return errReport("ElasticSearchClient::Add URL解析错误。");
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
			retval = false;
			return;
		}
   } else if (args[0].isArray()){ // 数组参数 
	   if(!buildEsUri(args[0], sUrl)){
           return errReport("ElasticSearchClient::Add 传入的第一个参数(数组),解析错误。");
       }
       
       sDoc =  ((Array)args[0]).get("body").jsonEncode().toString();

   } else{
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
        return errReport("ElasticSearchClient::Query 至少需要一个参数.");
    }

    string sURL,sRes;
    if(!getUrl(_obj.get("host").toString(), _obj.get("port").toString(), sURL)){
        returnVal = false;
        return errReport("ElasticSearchClient::Query URL解析错误。");
    }

    // 查询的uri
    Variant sUri;
    
    if (args[0].isString()){
        sURL += args[0].toString();
    }else if(args[0].isArray()){
        if(!buildEsUri(args[0], sURL)){
            return errReport("Delete方法传入的数组参数有误。");
        }
    }else{
        returnVal = false;
        return errReport("Query方法参支持的参数类型为STRING或ARRAY");
    }
    //cout << sURL << endl;
	
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
        ret = false;
        return errReport("ElasticSearchClient::Delete URL解析错误。");
    }

    if (args[0].isString()){
        sUrl += args[0].toString();
    } else if(args[0].isArray()){
        if(!buildEsUri(args[0], sUrl)){
            return errReport("Delete方法传入的数组参数有误。");
        }
    } else{
        ret = false;
        return errReport("Delete方法参支持的参数类型为STRING或ARRAY");
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
    for(int i=0; i<args.count(); i++){
        parameters.append(args[i]);
    }
    
    ES_Query(obj, parameters,ret);
    ES_Delete(obj,parameters, ret);
    ES_Add(obj, args, ret);
}

// 扩展入口
PHPX_EXTENSION(){
    // 实例化扩展
	Extension *pEx = new Extension("ElasticSearchClient", "1.0.0");

    pEx->registerConstant("ES", "ElasticSearch");

    // 注册普通函数
    // 1.普通方式 C++函数与PHP函数不用同名
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
    Class *pEsClass = new Class("Elasticsearch");

    // onStart 回调
    // 模块初始化阶段
    pEx->onStart = [pEsClass,pEx](){
        // 注册类方法
        pEsClass->addMethod("__construct", ElasticSearchClient_Constructor, PUBLIC, pConstructorArg);

        // 宏简化(写起来简单，但有局限,不方便IDE跳转) ： add -> Add
        pEsClass->addMethod(PHPX_ME(ES, Add), PUBLIC);

        // 更直观，约束少
        pEsClass->addMethod("Query", ES_Query, PUBLIC, nullptr);

        pEsClass->addMethod("Delete", ES_Delete, PUBLIC);

        pEsClass->addMethod("Update", ES_Update, PUBLIC);


        // 注册类成员
        // 普通成员
        pEsClass->addProperty("host", esHost, PUBLIC);
        pEsClass->addProperty("port", esPort, PUBLIC);
        // 常量
        pEsClass->addConstant("ES_FULL_NAME", cEsFullName);
        // 静态成员
        pEsClass->addProperty("version","0.0.0", STATIC);

        // 注册类
        pEx->registerClass(pEsClass);
    };


    // 请求结束回调
    pEx->onAfterRequest = [&](){
        // delete esInfo_Arg;
        // delete pConstructorArg;
    };

    // Before Request
    // 模块激活 RINT
    pEx->onBeforeRequest = [&](){
        
    };

    // Extension Shutdown
    pEx->onShutdown = [&](){
        // if(NULL != pEsClass){delete pEsClass;}
    };

    pEx->info (
        {"elasticsearchx: A ElasticSearch Client based on PHP-X","loaded"},
        {
            {"author", "wensheng"},
            {"version", ES_Version },
            { "date", "2017-11-22" }
        }
    );

	return pEx;
}




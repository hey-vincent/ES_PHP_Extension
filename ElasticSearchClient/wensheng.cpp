#include <iostream>
#include "phpx.h"
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
        * 连接超时
        * 请求超时
    * 静态变量
        * version
    * 超全局变量
        * 
    * 全局方法
        * 获取超全局变量
 */

#define EX_NAME   "elasticsearch"
#define CLASS_NAME  "ElasticSearchClient"
#define ES  CLASS_NAME

#define REG_FN(f) pEx->registerFunction(PHPX_FN(f));


static Http http;




// -------------------------------------------------------------------
// 普通函数（全局函数）声明
// -------------------------------------------------------------------

// 指定参数信息
ArgInfo *esInfo_Arg = new ArgInfo(1, false);

// 1. 普通函数声明方式
void esInfo(Args &args, Variant &retval){
    cout << "Hey. Welcome to " << ES << ".Enjoy!" << endl;
//    TODO
//    retval = ZVAL_NULL();
}


// 2. 宏简化方式
PHPX_FUNCTION(esWiki){
    retval = "ElasticSearch是一个基于Lucene的搜索服务器。它提供了一个分布式多用户能力的全文搜索引擎，基于RESTful web接口。Elasticsearch是用Java开发的，并作为Apache许可条款下的开放源码发布，是当前流行的企业级搜索引擎。设计用于云计算中，能够达到实时搜索，稳定，可靠，快速，安装使用方便。";
}

PHPX_FUNCTION(esDomain){
    cout << "https://www.elastic.co" << endl;
}


// -------------------------------------------------------------------
//  类声明
// -------------------------------------------------------------------
// 类成员声明
// -------------------------------------------------------------------
static Variant esHost("");
static Variant esPort(0);

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
PHPX_METHOD(ElasticSearchClient, add) {
    std::string sUrl, sResp,sDoc;
    sUrl = "http://" + _this.get("host").toString() + ":" + std::to_string(_this.get("port").toInt());
    sUrl += args[0].toString();

    // 文档数据处理
    if (args[1].isString()){
        sDoc.assign(args[1].toCString(), args[1].toString().size());
    } else if (args[1].isArray()){
        sDoc.assign(args[1].jsonEncode().toCString(),args[1].jsonEncode().toString().size() );
    }else{
        retval = "参数类型错误";
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
 */
void ElasticSearchClient_Query(Object &_obj, Args &args, Variant &returnVal){
    if (args.count() < 1){
        cout << "ElasticSearchClient::query至少需要1个参数" << endl;
    }

    Variant sUri = args[0];
    Http http;
    std::string sURL,sRes,sPort;
    sPort = esPort.isEmpty()? ":80" : (":"+std::to_string(esPort.toInt()));
    sURL = "http://" + esHost.toString() + sPort + sUri.toString();
  
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
void ElasticSearchClient_Delete(Object &obj, Args &args, Variant &ret){
    string sUrl = "http://" + obj.get("host").toString() + ":" + std::to_string(obj.get("port").toInt());
    sUrl += args[0].toString();
    string sResp;
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
void ElasticSearchClient_Update(Object &obj, Args &args, Variant &ret){

}



// 扩展入口
PHPX_EXTENSION(){
    // 实例化扩展
	Extension *pEx = new Extension("demo", "1.0.0");

    // 注册普通函数
    // 1.普通方式
    pEx->registerFunction("esInfo", esInfo, esInfo_Arg);

    // 2.宏方式
    pEx->registerFunction(PHPX_FN(esWiki));


    // 更懒的方式...
    REG_FN(esDomain)




    // onStart 回调
    pEx->onStart = [=](){
        // Class 类提供针对扩展类的定制
        Class *pEsClass = new Class("ElasticSearchClient");

        // 注册类方法
        pEsClass->addMethod("__construct", ElasticSearchClient_Constructor, PUBLIC, pConstructorArg);

        // 宏简化(写起来简单，但有局限,不方便IDE跳转)
        pEsClass->addMethod(PHPX_ME(ElasticSearchClient, add), PUBLIC);

        pEsClass->addMethod("query", ElasticSearchClient_Query, PUBLIC);

        pEsClass->addMethod("Delete", ElasticSearchClient_Delete, PUBLIC);

        pEsClass->addMethod("Update", ElasticSearchClient_Update, PUBLIC);


        // 注册类成员
        // 普通成员
        pEsClass->addProperty("host", esHost, PUBLIC);
        pEsClass->addProperty("port", esPort, PUBLIC);
        // 常量
        pEsClass->addConstant("ES_FULL_NAME", cEsFullName);

        // 注册类
        pEx->registerClass(pEsClass);
    };



    // 请求结束回调
    pEx->onAfterRequest = [&](){
        delete esInfo_Arg;
    };


	return pEx;
}




#include <iostream>

#include <string>

using namespace std;
using namespace php;

// 拼接url
bool getUrl(string host, string port , string &url,  bool ssl =  false){
    url = ssl ? "https://" : "http://";
    if(host.empty())    return false;
    url += host;
    url += port.empty() ? "" : ":" + port;
    return true;
}


// trigger error

enum ERROR_LEVEL
{
    NOTICE = 1,
};

void errReport(const char *pErr, ERROR_LEVEL level = ERROR_LEVEL::NOTICE)
{
    // todo PHP风格的Notice/warning/Fatal
    cout << "Notice:" << pErr << endl;
}

// 从数组中拼凑ES文档的uri
// TODO field checking 
bool buildEsUri(php::Array arr, string &uri, vector<string> fieldsEssential = {"index", "type", "id", "body"})
{
	string sIndex = arr.get("index").toString();
    if(sIndex.empty()){
        return false;
    }
    uri += uri.back() == '/' ? sIndex : "/"+sIndex;

	string sType = arr.get("type").toString();
    if(sType.empty()){
        return false;
    }
    uri += uri.back() == '/' ? sType : "/"+sType;
   
	string sId = arr.get("id").toString();
    if(!sType.empty()){
        uri += uri.back() == '/' ? sId : "/"+sId;
    }

    return true;
}
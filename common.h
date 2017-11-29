#include <iostream>

#include <string>

using namespace std;

// 拼接url
bool getUrl(string host, string port , string &url,  bool ssl =  false){
    url = ssl ? "https://" : "http://";
    if(host.empty())    return false;
    url += host;
    url += port.empty() ? "" : ":" + port;
    return true;
}


// trigger error

enum ERROR_LEVEL{
    NOTICE = 1,
};

void errReport(char *pErr, ERROR_LEVEL level = ERROR_LEVEL::NOTICE){
    cout << "Notice:" << pErr << endl;
}
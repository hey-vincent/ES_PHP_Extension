//
// Created by MOMO on 2017/11/14.
//

#ifndef PHPX_HTTP_H
#define PHPX_HTTP_H
#include <iostream>
#include <string>
#include <map>

using namespace std;

class Http {

public:
    bool get(std::string strUrl, std::string strParam, string &res);
    bool puts(std::string strUrl, map<string,string> mapHeader, std::string strParam, string &res);
    bool post(std::string strUrl, std::string strParam, string &sRep);
    bool Put(string strUrl, map<string,string> mapHeader, string strParams, string &resp);
    bool Delete(string strUrl, string &resp);
};


#endif //PHPX_HTTP_H

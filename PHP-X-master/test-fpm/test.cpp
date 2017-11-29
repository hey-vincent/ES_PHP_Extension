#include <iostream>
#include <phpx.h>

using namespace php;
using namespace std;

void Sum( Args &arg, Variant &ret){
    vector<char>buf(512,0);
    int nLen = sprintf(&buf[0], "hello %s , Welcome to Nginx and Fpm.", arg[0].toString().c_str());
    string s;s.assign(&buf[0], nLen);
    s.resize(nLen);
    ret = s;
    return;
}

void Wensheng_Test(Object& _o, Args &arg, Variant &v){
    vector<char>buf(512,0);
    int nLen = sprintf(&buf[0], "hello %s , Welcome to Nginx and Fpm.", arg[0].toString().c_str());
    string s;s.assign(&buf[0], nLen);
    s.resize(nLen);
    v = s;
    return;
}


int *p = NULL;

PHPX_EXTENSION(){
     ArgInfo *pAg = new ArgInfo(1, false);
     pAg->add("who", nullptr, IS_STRING);

    Extension *pEx = new Extension("Wensheng", "0.0.1.0");
    pEx->registerFunction("mySum", Sum, pAg);

    php::Class *pClass = new Class("WenshengTest");

    pEx->onStart = [pEx, pClass,pAg]() noexcept {
        pEx->registerClass(pClass);
        pClass->addMethod("test", Wensheng_Test, PUBLIC);
    };
    


    pEx->onAfterRequest = [&] () {
        delete p;
    };

    pEx->info({"Wensheng Testing", "Loaded"},{{"author", "wensheng"},{"date", "now"}});
    return pEx;
}

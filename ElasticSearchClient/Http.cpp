//
// Created by MOMO on 2017/11/14.
//

#include "Http.h"
#include <stdio.h>
#include <curl/curl.h>

#define LOCAL_FILE "./xtest.txt"

/**
 * 请求回调
 * @param ptr
 * @param size
 * @param nmemb
 * @param stream
 * @return
 */
size_t responseCallBack(void *ptr, size_t size, size_t nmemb, void *stream)
{
    string *str = (string*)stream;
    (*str).append((char*)ptr, size*nmemb);
    return size * nmemb;
}

/**
 * Get 请求
 * @param strUrl
 * @param strParams
 * @param resp
 * @return
 */
bool Http::get(string strUrl, string strParams, string &resp)
{
    CURL *curl;
    CURLcode res;
    bool result = false;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, "");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, responseCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &resp);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK){
            result = false;
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        result = true;
    }
    curl_global_cleanup();
    return result;
}

bool Http::Delete(string strUrl, string &resp)
{
    CURL *curl;
    CURLcode res;
    bool result = false;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, responseCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &resp);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK){
            result = false;
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        result = true;
    }
    curl_global_cleanup();
    return result;
}


/**
 * PUT 方式请求
 *
 * @param strUrl
 * @param mapHeader 头部Map
 * @param strParams 请求体
 * @param resp  响应数据
 * @return
 */
bool Http::puts(string strUrl, map<string,string> mapHeader, string strParams, string &resp)
{
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    string sTmp;
    struct curl_slist *pHead = NULL;

    // 拼凑请求头
    for(map<string,string>::iterator it = mapHeader.begin();
            it != mapHeader.end();
            it++){
        sTmp = (*it).first + ":" + (*it).second;
        pHead = curl_slist_append(pHead, sTmp.c_str());
    }

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_PUT, 1);
//        curl_easy_setopt(curl, CURLOPT_PUT, strParams.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, responseCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &resp);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, pHead);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));

        curl_easy_cleanup(curl);
        return false;
    }
    curl_global_cleanup();
    return true;
}


bool Http::post(string strUrl, string strParams, string &sRep)
{
    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
    struct curl_slist *pHead = NULL;
    pHead = curl_slist_append(pHead, "Content-type:application/json");
    /* get a curl handle */
    curl = curl_easy_init();
    if (curl)
    {
        /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:9200/wensheng/best/");
        /* Now specify the POST data */
        // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strParams.c_str());

        // Add
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, responseCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &sRep);

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, pHead);
        

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}

/**
 * PUT请求
 * @param strUrl
 * @param mapHeader
 * @param strParams
 * @param sResp
 * @return
 */
bool Http::Put(string strUrl, map<string,string> mapHeader, string strParams, string &sResp)
{
    CURL *curl;
    CURLcode res;
    bool result = false;

    curl_global_init(CURL_GLOBAL_ALL);
    struct curl_slist *pHead = NULL;
    string stmp;

    for (map<string,string>::iterator i = mapHeader.begin(); i != mapHeader.end() ; ++i) {
        stmp = (*i).first + ":" + (*i).second;
        pHead = curl_slist_append(pHead, stmp.c_str());
    }

    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, pHead);
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strParams.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, responseCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &sResp);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            result = false;
        }
        result = true;
        curl_slist_free_all(pHead);
        curl_easy_cleanup(curl);
        return result;
    }
    return false;
}
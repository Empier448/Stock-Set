#include "curl_functions.h"
#include <iostream>

using namespace std;

const string CACERT_PATH = "C:\\Users\\Plaifa\\AppData\\Local\\Programs\\Python\\Python312\\Lib\\site-packages\\certifi\\cacert.pem";

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

string fetchData(const string& url) {
    CURL* curl;
    CURLcode res;
    string buffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, CACERT_PATH.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "YourCustomUserAgentString");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            return "";
        }
        else {
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            cout << "HTTP Response Code: " << response_code << endl;
            if (response_code != 200) {
                cerr << "Error: Received HTTP response code " << response_code << endl;
                return "";
            }
        }
        curl_easy_cleanup(curl);
    }
    return buffer;
}

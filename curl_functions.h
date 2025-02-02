#ifndef CURL_FUNCTIONS_H
#define CURL_FUNCTIONS_H

#include <string>
#include <curl/curl.h>

std::string fetchData(const std::string& url);

#endif // CURL_FUNCTIONS_H
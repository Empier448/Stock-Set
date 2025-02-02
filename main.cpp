#include <iostream>
#include <vector>
#include <ctime>
#include <json/json.h>
#include "curl_functions.h"
#include "file_utils.h"
#include "main.h"  

using namespace std;

//const string OUTPUT_DIR = "C:/Users/Plaifa/Downloads/Python/Data/";
const string STOCK_LIST_PATH = "C:/Users/Plaifa/Downloads/Python/Data/stocks_data1.csv"; // ไฟล์ CSV ที่มีรายชื่อหุ้น

void fetchStockData() {
    vector<string> symbols = getStockSymbolsFromCSV(STOCK_LIST_PATH);
    vector<vector<string>> csvData;
    csvData.push_back({ "Ticker", "Date", "Open", "High", "Low", "Close", "Volume" });

    time_t now = time(0);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&now));
    string currentDate(buffer);

    for (const auto& symbol : symbols) {
        string url = "https://query1.finance.yahoo.com/v8/finance/chart/" + symbol + "?interval=1d";
        string jsonData = fetchData(url);

        if (jsonData.empty()) {
            cerr << "Error: Received empty JSON response for symbol: " << symbol << endl;
            continue;
        }

        Json::CharReaderBuilder reader;
        Json::Value root;
        string errs;
        istringstream ss(jsonData);
        if (!Json::parseFromStream(reader, ss, &root, &errs)) {
            cerr << "Error parsing JSON: " << errs << endl;
            cerr << "Received JSON: " << jsonData << endl;
            continue;
        }

        if (root["chart"]["result"].empty()) {
            cerr << "Error: No data found for symbol: " << symbol << endl;
            continue;
        }

        const Json::Value quote = root["chart"]["result"][0]["indicators"]["quote"][0];
        string open = quote["open"][0].isNull() ? "N/A" : to_string(quote["open"][0].asFloat());
        string high = quote["high"][0].isNull() ? "N/A" : to_string(quote["high"][0].asFloat());
        string low = quote["low"][0].isNull() ? "N/A" : to_string(quote["low"][0].asFloat());
        string close = quote["close"][0].isNull() ? "N/A" : to_string(quote["close"][0].asFloat());
        string volume = quote["volume"][0].isNull() ? "N/A" : to_string(quote["volume"][0].asUInt64());

        string cleanSymbol = (symbol.size() > 3 && symbol.substr(symbol.size() - 3) == ".BK") ? symbol.substr(0, symbol.size() - 3) : symbol;
        csvData.push_back({ cleanSymbol, currentDate, open, high, low, close, volume });
    }
    saveToCSV(csvData, "stocks_data_yahoo.csv");
}

int main() {
    fetchStockData();
    cout << "Stock data saved successfully." << endl;
    return 0;
}



//g++ main.cpp curl_functions.cpp file_utils.cpp -o stock_fetcher -I../include -lcurl -ljsoncpp

#include "file_utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>

using namespace std;

vector<string> getStockSymbolsFromCSV(const string& filePath) {
    vector<string> symbols;
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return symbols;
    }

    string line;
    getline(file, line); // ข้าม header
    while (getline(file, line)) {
        stringstream ss(line);
        string ticker;
        getline(ss, ticker, ',');

        if (ticker.length() < 3 || ticker.substr(ticker.length() - 3) != ".BK") {
            ticker += ".BK";
        }

        symbols.push_back(ticker);
    }

    file.close();
    return symbols;
}

bool directoryExists(const string& path) {
    struct stat info;
    return stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR);
}

void saveToCSV(const vector<vector<string>>& data, const string& filename) {
    const string OUTPUT_DIR = "C:/Users/Plaifa/Downloads/Python/Data/";
    if (!directoryExists(OUTPUT_DIR)) {
        cerr << "Error: Directory " << OUTPUT_DIR << " does not exist." << endl;
        return;
    }

    ofstream file(OUTPUT_DIR + filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) file << ",";
        }
        file << endl;
    }
    file.close();
}
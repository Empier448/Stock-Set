#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>

std::vector<std::string> getStockSymbolsFromCSV(const std::string& filePath);
bool directoryExists(const std::string& path);
void saveToCSV(const std::vector<std::vector<std::string>>& data, const std::string& filename);

#endif // FILE_UTILS_H
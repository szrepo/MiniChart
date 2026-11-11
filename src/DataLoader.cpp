#include "DataLoader.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

// ---------- small helpers (only visible inside this file) ----------
static std::string trim(const std::string& s) {
    size_t start = 0, end = s.size();
    // also strips a UTF-8 BOM, quotes, spaces and \r
    while (start < end && (std::isspace((unsigned char)s[start]) || s[start] == '"' ||
                           (unsigned char)s[start] == 0xEF || (unsigned char)s[start] == 0xBB ||
                           (unsigned char)s[start] == 0xBF)) start++;
    while (end > start && (std::isspace((unsigned char)s[end - 1]) || s[end - 1] == '"')) end--;
    return s.substr(start, end - start);
}

static std::string toLower(std::string s) {
    for (char& c : s) c = (char)std::tolower((unsigned char)c);
    return s;
}

static std::vector<std::string> split(const std::string& line) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, ',')) parts.push_back(trim(item));
    return parts;
}

// Returns the index of the column named `name`, or -1 if it's missing
static int findColumn(const std::vector<std::string>& header, const std::string& name) {
    for (size_t i = 0; i < header.size(); i++)
        if (toLower(header[i]) == name) return (int)i;
    return -1;
}

// ---------- the loader ----------
std::vector<Candle> DataLoader::loadCSV(const std::string& path, std::string& error) {
    std::vector<Candle> candles;
    error.clear();

    std::ifstream file(path);
    if (!file.is_open()) {
        error = "Could not open file: " + path;
        return candles;
    }

    std::string line;
    if (!std::getline(file, line)) {
        error = "File is empty: " + path;
        return candles;
    }

    // Read the header and find where each column lives
    std::vector<std::string> header = split(line);
    int iDate = findColumn(header, "date");
    int iOpen = findColumn(header, "open");
    int iHigh = findColumn(header, "high");
    int iLow = findColumn(header, "low");
    int iClose = findColumn(header, "close");   // exact match, so "Adj Close" is ignored
    int iVol = findColumn(header, "volume");

    if (iDate < 0 || iOpen < 0 || iHigh < 0 || iLow < 0 || iClose < 0 || iVol < 0) {
        error = "CSV header must contain: Date, Open, High, Low, Close, Volume";
        return candles;
    }

    int maxIndex = std::max({iDate, iOpen, iHigh, iLow, iClose, iVol});
    int skipped = 0;

    while (std::getline(file, line)) {
        if (trim(line).empty()) continue;
        std::vector<std::string> f = split(line);
        if ((int)f.size() <= maxIndex) { skipped++; continue; }

        try {
            Candle c;
            c.date   = f[iDate];
            c.open   = std::stod(f[iOpen]);
            c.high   = std::stod(f[iHigh]);
            c.low    = std::stod(f[iLow]);
            c.close  = std::stod(f[iClose]);
            c.volume = std::stod(f[iVol]);
            candles.push_back(c);
        } catch (...) {
            skipped++;   // bad number, "null", etc. Skip the row, don't crash
        }
    }

    if (candles.empty()) {
        error = "No valid rows found in: " + path;
        return candles;
    }

    // YYYY-MM-DD strings sort correctly as plain text
    std::sort(candles.begin(), candles.end(),
              [](const Candle& a, const Candle& b) { return a.date < b.date; });

    if (skipped > 0)
        error = "Warning: skipped " + std::to_string(skipped) + " bad rows";
    return candles;
}
#pragma once
#include <string>
#include <vector>
#include "Candle.h"

class DataLoader {
public:
    // Returns the candles sorted by date. On failure, returns an empty
    // vector and puts a message in `error`.
    static std::vector<Candle> loadCSV(const std::string& path, std::string& error);
};
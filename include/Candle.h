#pragma once
#include <string>

struct Candle {
    std::string date;
    double open   = 0;
    double high   = 0;
    double low    = 0;
    double close  = 0;
    double volume = 0;
};
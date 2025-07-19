#pragma once

#include <cstdint>
#include <string>

struct TradeInfo 
{
    uint64_t timestamp;
    double price;
    int vol;
    bool is_bid;
    std::string symbol;
};

struct PositionInfo
{
    double price;
    int vol;
    bool is_bid;
};

struct PnLInfo
{
    uint64_t timestamp;
    double pnl;
    std::string symbol;
};

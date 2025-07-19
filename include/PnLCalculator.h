#pragma once

#include <string>
#include <vector>
#include <deque>
#include <unordered_map>

#include "data_definition.h"

enum class AccountingScheme
{
    FIFO,
    LIFO,
    UNKNOWN
};

static AccountingScheme str2AccountingScheme(std::string str)
{
    if (str == "fifo") return AccountingScheme::FIFO;
    else if (str == "lifo") return AccountingScheme::LIFO;
    else return AccountingScheme::UNKNOWN;
};

class PnLCalculator
{
public:
    PnLCalculator(AccountingScheme accounting_scheme) : accounting_scheme_(accounting_scheme) {}
    ~PnLCalculator() {}

    void handle_tradeinfos(const std::vector<TradeInfo> &trade_infos);
    void print_pnl() const;

private:
    void handle_tradeinfo(const TradeInfo &trade_info);

    AccountingScheme accounting_scheme_;

    std::unordered_map<std::string, std::deque<PositionInfo>> position_infos_by_symbols_;
    std::vector<PnLInfo> pnl_infos;
};
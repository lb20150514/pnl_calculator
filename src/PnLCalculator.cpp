#include <iomanip>
#include <sstream>
#include <iostream>

#include "PnLCalculator.h"

void PnLCalculator::handle_tradeinfos(const std::vector<TradeInfo> &trade_infos)
{
    for (const auto &trade_info : trade_infos)
    {
        handle_tradeinfo(trade_info);
    }
}

void PnLCalculator::handle_tradeinfo(const TradeInfo &trade_info)
{
    auto &position_infos = position_infos_by_symbols_[trade_info.symbol];
    if (position_infos.size() < 1 || (position_infos.begin()->is_bid == trade_info.is_bid))
    {
        position_infos.emplace_back(PositionInfo{trade_info.price, trade_info.vol, trade_info.is_bid});
    }
    else
    {
        int changed_vol = trade_info.vol;
        double pnl = 0.0;
        auto beg = accounting_scheme_ == AccountingScheme::FIFO ? position_infos.begin() : position_infos.end();
        auto end = accounting_scheme_ == AccountingScheme::FIFO ? position_infos.end() : position_infos.begin();
        auto iter = beg;
        while (iter != end && changed_vol > 0)
        {
            iter = accounting_scheme_ == AccountingScheme::FIFO ? iter : (--iter);
            if (iter->vol > changed_vol)
            {
                pnl += (trade_info.price - iter->price) * (changed_vol * (trade_info.is_bid ? -1 : 1));
                iter->vol -= changed_vol;
                changed_vol = 0;
            }
            else if (iter->vol == changed_vol)
            {
                pnl += (trade_info.price - iter->price) * (changed_vol * (trade_info.is_bid ? -1 : 1));
                changed_vol = 0;
                iter = position_infos.erase(iter);
            }
            else
            {
                pnl += (trade_info.price - iter->price) * (iter->vol * (trade_info.is_bid ? -1 : 1));
                changed_vol -= iter->vol;
                iter = position_infos.erase(iter);
            }
        }

        if (changed_vol > 0)
        {
            position_infos.emplace_back(PositionInfo{trade_info.price, changed_vol, trade_info.is_bid});
        }

        pnl_infos.emplace_back(PnLInfo{trade_info.timestamp, pnl, trade_info.symbol});
    }
}

void PnLCalculator::print_pnl() const
{
    std::cout << "TIMESTAMP,SYMBOL,PNL" << std::endl;
    for (const auto &pnl_info : pnl_infos)
    {
        std::cout << pnl_info.timestamp << "," << pnl_info.symbol << "," << std::fixed << std::setprecision(2) << pnl_info.pnl << std::endl;
    }
}
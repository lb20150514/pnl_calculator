#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "PnLCalculator.h"

void read_tradeinfos_from_csv(const std::string &file_path, std::vector<TradeInfo> &trade_infos)
{
    std::ifstream file(file_path);
    std::string line;

    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> fields;

        while (std::getline(ss, field, ','))
        {
            fields.push_back(field);
        }

        uint64_t timestamp = std::stoul(fields[0]);
        std::string symbol = fields[1];
        bool is_bid = fields[2][0] == 'B';
        double price = std::stod(fields[3]);
        int vol = std::stoi(fields[4]);

        trade_infos.emplace_back(TradeInfo{timestamp, price, vol, is_bid, symbol});
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: [executable_file] [input_file] ['fifo'|'lifo']" << std::endl;
        exit(1);
    }

    std::string input_file_path = argv[1];
    std::string accounting_scheme_str = argv[2];

    AccountingScheme accounting_scheme = str2AccountingScheme(accounting_scheme_str);
    if (accounting_scheme == AccountingScheme::UNKNOWN)
    {
        std::cerr << "Wrong scheme: " << accounting_scheme_str << "! Only 'fifo' or 'lifo' supported." << std::endl;
        exit(1);
    }

    std::vector<TradeInfo> trade_infos;
    read_tradeinfos_from_csv(input_file_path, trade_infos);

    PnLCalculator pnl_calculator(accounting_scheme);
    pnl_calculator.handle_tradeinfos(trade_infos);

    pnl_calculator.print_pnl();

    return 0;
}
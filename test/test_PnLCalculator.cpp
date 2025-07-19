#include "gtest/gtest.h"
#include "PnLCalculator.h"

class PnLCalculatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        fifo_pnl_calculator = new PnLCalculator(AccountingScheme::FIFO);
        lifo_pnl_calculator = new PnLCalculator(AccountingScheme::LIFO);
    }
    
    void TearDown() override {
        delete fifo_pnl_calculator;
        delete lifo_pnl_calculator;
    }
    
    PnLCalculator* fifo_pnl_calculator;
    PnLCalculator* lifo_pnl_calculator;
};

TEST_F(PnLCalculatorTest, FIFO_SingleTradeNoPnL) {
    std::vector<TradeInfo> trade_infos = {
        TradeInfo{101, 10.00, 100, true, "TFS"}
    };
    fifo_pnl_calculator->handle_tradeinfos(trade_infos);
    testing::internal::CaptureStdout();
    fifo_pnl_calculator->print_pnl();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(output, "TIMESTAMP,SYMBOL,PNL\n");
}

TEST_F(PnLCalculatorTest, FIFO_OneFullyClosedPnL) {
    std::vector<TradeInfo> trade_infos = {
        TradeInfo{101, 10.00, 100, true, "TFS"},
        TradeInfo{102, 12.00, 100, false, "TFS"}
    };
    fifo_pnl_calculator->handle_tradeinfos(trade_infos);
    testing::internal::CaptureStdout();
    fifo_pnl_calculator->print_pnl();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(output, 
        "TIMESTAMP,SYMBOL,PNL\n"
        "102,TFS,200.00\n");
}

// Should work but input guarantee buy_vol == sell_vol
TEST_F(PnLCalculatorTest, LIFO_PartiallyClosedPnL) {
    std::vector<TradeInfo> trade_infos = {
        TradeInfo{101, 10.00, 50, true, "TFS"},
        TradeInfo{102, 11.00, 50, true, "TFS"},
        TradeInfo{103, 12.00, 75, false, "TFS"}
    };
    lifo_pnl_calculator->handle_tradeinfos(trade_infos);
    testing::internal::CaptureStdout();
    lifo_pnl_calculator->print_pnl();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(output, 
        "TIMESTAMP,SYMBOL,PNL\n"
        "103,TFS,100.00\n");
}

TEST_F(PnLCalculatorTest, LIFO_MultipleSymbols) {
    std::vector<TradeInfo> trade_infos = {
        TradeInfo{101, 10.00, 100, true, "TFS"},
        TradeInfo{102, 21.00, 50, true, "APPL"},
        TradeInfo{103, 12.00, 75, false, "TFS"},
        TradeInfo{104, 23.00, 50, false, "APPL"}
    };
    fifo_pnl_calculator->handle_tradeinfos(trade_infos);
    testing::internal::CaptureStdout();
    fifo_pnl_calculator->print_pnl();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(output, 
        "TIMESTAMP,SYMBOL,PNL\n"
        "103,TFS,150.00\n"
        "104,APPL,100.00\n");
}

TEST_F(PnLCalculatorTest, LIFO_FirstSellThenBuy) {
    std::vector<TradeInfo> trade_infos = {
        TradeInfo{101, 10.00, 100, false, "TFS"},
        TradeInfo{102, 30.00, 100, false, "APPL"},
        TradeInfo{103, 11.00, 75, true, "TFS"},
        TradeInfo{104, 25.00, 200, true, "APPL"},
        TradeInfo{104, 8.00, 25, true, "TFS"},
        TradeInfo{105, 40.00, 100, false, "APPL"}
    };
    lifo_pnl_calculator->handle_tradeinfos(trade_infos);
    testing::internal::CaptureStdout();
    lifo_pnl_calculator->print_pnl();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(output, 
        "TIMESTAMP,SYMBOL,PNL\n"
        "103,TFS,-75.00\n"
        "104,APPL,500.00\n"
        "104,TFS,50.00\n"
        "105,APPL,1500.00\n");
}
#pragma once
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"


class MerkelMain
{
    public:
        MerkelMain();
        /** Call this to start the sim*/
        void init();
    private:
        void printMenu();
        void printHelp();
        void printMarketstat();
        void enterAsk();
        void enterBid();
        void printWallet();
        void gotoNextTimeframe();
        int getUseroption();
        void processUseroption(int userOption);

        std::string currentTime;

        OrderBook orderBook{"testmatch.csv"};
};
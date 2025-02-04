#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook
{
    public:
    /** Constructs , reading a csv datafile */
        OrderBook(std::string filename);
        /** return Vector of all online products in the data set */
        std::vector<std::string> getKnownProducts();
        /** retun vector of orders according to the sent filters*/
        std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                              std::string product,
                                              std::string timestamp);

        /** returns the earliest time in the orderbook*/
        std::string getEarliestTime();
        /** returns the next time after the 
         * sent time in the order book
         * if there's no next timestamp, wraps around to the start
         * */
        std::string getNextTime(std::string timestamp);
        void insertOrder(OrderBookEntry& order);
        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        static double getHighPrice(std::vector<OrderBookEntry> &orders);
        static double getLowPrice(std::vector<OrderBookEntry> &orders);
        static double getMeanPrice(std::vector<OrderBookEntry> &orders);
        static double getAverageBids(std::vector<OrderBookEntry> &orders);
        static double getAverageAsks(std::vector<OrderBookEntry> &orders);
        static double getOpeningAsk(std::vector<OrderBookEntry> &orders);
        static double getClosingAsk(std::vector<OrderBookEntry> &orders);

    private:
        std::vector<OrderBookEntry> orders;
};

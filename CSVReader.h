#pragma once

#include "OrderBookEntry.h"
#include <vector>

class CSVReader
{
    public:
        CSVReader();

     static std::vector<OrderBookEntry> readCSV(std::string csvFile);//stateless function
     static std::vector<std::string> tokenise(std::string csvLine, char seperator);
     static OrderBookEntry stringToOBE(std::string price,
                                       std::string amount,
                                       std::string timestamp,
                                       std::string product,
                                       OrderBookType OrderBookType);

 private:
    static OrderBookEntry stringToOBE(std::vector<std::string> strings);
};
#include "CSVReader.h"
#include <iostream>
#include <fstream>

CSVReader::CSVReader()
{

}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename) //stateless function
{
    std::vector<OrderBookEntry> entries;
    std::ifstream csvFile{csvFilename};
    std::string line;
    if (csvFile.is_open())
    {
        while(std::getline(csvFile, line))
        {
            try {
                OrderBookEntry obe = stringToOBE(tokenise(line, ','));
                entries.push_back(obe);
                }catch(const std::exception& e)
                {
                    std::cout << "CSVReader::readCSV bad data" << std::endl;
                }
            }// end of while 
    }           
    std::cout << "CSVReader::readCSV read "<< entries.size() << " entries" << std::endl;
    return entries;
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char seperator)
{
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token;
    start = csvLine.find_first_not_of(seperator, 0);
    do{
        end = csvLine.find_first_of(seperator, start);
        if (start == csvLine.length() ||  start == end )
            break;
        if (end >= 0)
            token = csvLine.substr(start, end - start);
        else
            token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);

        start = end + 1;
    } while (end > 0); //while (end != std::string::npos);

    return tokens;
}

OrderBookEntry CSVReader::stringToOBE(std::vector<std::string> tokens)
{

    double price, amount;

    if (tokens.size() != 5) // bad
    {
        std::cout << " Bad line " << std::endl;
        throw std::exception{};
    }
//we have 5 tokens
    try {
         price = std::stod(tokens[3]);
         amount = std::stod(tokens[4]);
    }catch (const std::exception &e){
        std::cout << "CSVReader::stringToOBE Bad float! " << tokens[3] << std::endl;
        std::cout << "CSVReader::stringToOBE Bad float! " << tokens[4] << std::endl;
        throw;
    }
    
    OrderBookEntry obe{price,
                            amount,
                            tokens[0],
                            tokens[1],
                            OrderBookEntry::stringToOrderBookType(tokens[2])};
        return obe;
}

OrderBookEntry CSVReader::stringToOBE(std::string priceString,
                                      std::string amountString,
                                      std::string timestamp,
                                      std::string product,
                                      OrderBookType OrderType)
{
    double price, amount;
    try
    {
        price = std::stod(priceString);
        amount = std::stod(amountString);
    }
    catch (const std::exception &e)
    {
        std::cout << "CSVReader::stringToOBE Bad float! " << priceString << std::endl;
        std::cout << "CSVReader::stringToOBE Bad float! " << amountString << std::endl;
        throw;
    }
    OrderBookEntry obe{price,
                       amount,
                       timestamp,
                       product,
                       OrderType};
    return obe;
}
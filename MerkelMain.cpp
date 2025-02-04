#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "OrderBook.h"
#include <limits>
MerkelMain::MerkelMain()
{

}


void MerkelMain::init()
{
    //loadOrderbook();
    int input;
    currentTime = orderBook.getEarliestTime();
    while (true)
    {
        printMenu();
        input = getUseroption();
        processUseroption(input);
    }
}


void MerkelMain::printMenu()
{
    std::cout << "1: print help" << std::endl;
    std::cout << "2: print exchange stats" << std::endl;
    std::cout << "3: make an Ask" << std::endl;
    std::cout << "4: make a bid" << std::endl;
    std::cout << "5: print wallet" << std::endl;
    std::cout << "6: continue " << std::endl;
    std::cout << "============" << std::endl;
    std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketstat()
{

    
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                                  p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries)<< std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
        std::cout << "Mean ask: " << OrderBook::getMeanPrice(entries) << std::endl;
        std::cout << "The Average Ask: " << OrderBook::getAverageAsks(entries) << std::endl;
        std::cout << "Opening ask: " << OrderBook::getOpeningAsk(entries) << std::endl;
        std::cout << "Closing  ask: " << OrderBook::getClosingAsk(entries) << std::endl;
    }

    for (std::string const &p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid,
                                                                  p, currentTime);
        std::cout << "Bid seen: " << entries.size() << std::endl;
        std::cout << "Max Bid: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min Bid: " << OrderBook::getLowPrice(entries) << std::endl;
        std::cout << "The Average Bid: " << OrderBook::getAverageBids(entries) << std::endl;
    }
    // std::cout << "Orderbook Contains : "<< orders.size() <<  " entries" << std::endl;
    // unsigned int bids = 0;
    // unsigned int asks = 0;
    // for (OrderBookEntry& e : orders)
    // {
    //     if (e.orderType == OrderBookType::ask)
    //     {
    //         asks ++;
    //     }
    //     if (e.orderType == OrderBookType::bid)
    //     {
    //         bids++;
    //     }
    // }
    // std::cout << "Orderbook asks: " << asks << " bids:" << bids << std::endl;
}
void MerkelMain::enterAsk()
{
    std::cout << "Make an Ask- enter the amount: product, price, amount, eg  ETH/BTC,200,0.5 " << std::endl;
    std::string input;
    /**clearing out the console input such that we can process it new with the getline function*/
   /**std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');*/
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input!  " << input << std::endl;
    }
    else{
        try{
            OrderBookEntry obe = CSVReader::stringToOBE(
            tokens[1],
            tokens[2],
            currentTime,
            tokens[0],
            OrderBookType::ask
            );
            orderBook.insertOrder(obe);
        }
        catch (const std::exception &e)
        {
            std::cout << "MerkelMain::enterAsk bad input!" << std::endl;
        }
    }

    std::cout << "You typed: " << input << std::endl;
}
void MerkelMain::enterBid()
{
    std::cout << "Make a bid - enter the amount: product, price, amount, eg  ETH/BTC,200,0.5 " << std::endl;
    std::string input;
    /**clearing out the console input such that we can process it new with the getline function*/
    /**std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');*/
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input!  " << input << std::endl;
    }
    else
    {
        try
        {
            OrderBookEntry obe = CSVReader::stringToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid);
            orderBook.insertOrder(obe);
        }
        catch (const std::exception &e)
        {
            std::cout << "MerkelMain::enterBid bad input!" << std::endl;
        }
    }

    std::cout << "You typed: " << input << std::endl;
}
void MerkelMain::printWallet()
{
    std::cout << "Your wallet is EMPTY " << std::endl;
}

void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame " << std::endl;
    std::vector<OrderBookEntry> sales =  orderBook.matchAsksToBids("ETH/BTC", currentTime);
    std::cout << "Sales: " << sales.size() << std::endl;
    for (OrderBookEntry &sale : sales)
    {
        std::cout << "sale price: " << sale.price << " amount " << sale.amount << std::endl;
    }
    currentTime = orderBook.getNextTime(currentTime);
}

int MerkelMain::getUseroption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-6" << std::endl;
    std::getline(std::cin, line);
try{
    userOption = std::stoi(line);
}catch(const std::exception& e)
{
    
}
    std::cout << "You Chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUseroption(int userOption)
{
    if (userOption == 0)
    {
        std::cout << "invalid choice, choose 1-6 " << std::endl;
    }
    if (userOption == 1)
    {
        printHelp();
    }
    if (userOption == 2)
    {
        printMarketstat();
    }
    if (userOption == 3)
    {
        enterAsk();
    }
    if (userOption == 4)
    {
        enterBid();
    }
    if (userOption == 5)
    {
        printWallet();
    }
    if (userOption == 6)
    {
        gotoNextTimeframe();
    }
}
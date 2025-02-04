#include "OrderBook.h" 
#include <map>
#include <algorithm>

/** Constructs , reading a csv datafile */
OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}
    /** return Vector of all online products in the data set */
std::vector<std::string> OrderBook::getKnownProducts(){
    std::vector<std::string> products;

    std::map<std::string,bool> prodMap;
    for(OrderBookEntry& e : orders)
    {
        prodMap[e.product] = true;
    }

    //now flattenn the map to a vector of strings
    for (auto const &e : prodMap)
    {
        products.push_back(e.first);
    }
    return products;
}
    /** retun vector of orders according to the sent filters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                                std::string product,
                                               std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry& e : orders)
    {
        if (e.orderType == type &&
            e.product == product &&
            e.timestamp == timestamp)
            {
                orders_sub.push_back(e);
            }
    }
    return orders_sub;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
    double max = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price > max)max = e.price;
    
    }
    return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry> &orders)
{
    double min = orders[0].price;
    for (OrderBookEntry &e : orders)
    {
        if (e.price < min)
            min = e.price;
    }
    return min;
}

double OrderBook::getMeanPrice(std::vector<OrderBookEntry> &orders)
{
    //check if the orders vector is empty 
    if(orders.empty())
    {
        return 0.0;
    }
    double total = 0.0;
    for (const OrderBookEntry &e : orders)
    //iterate through all orders to calculate the total price 
    {
        total += e.price; // Accumulate the total Price 
    }
    //calculate nd return the mean price 
    return total / orders.size();
}
double OrderBook::getAverageBids(std::vector<OrderBookEntry> &orders)
{
    double total = 0.0;
    int count = 0;
    for (const OrderBookEntry &e : orders)
    {
        if (e.orderType == OrderBookType::bid)
        {
            total += e.price;
            count++;
        }
    }
    return count > 0 ? total / count : 0.0;//handle empty bids gracefully 
}

double OrderBook::getAverageAsks(std::vector<OrderBookEntry> &orders)
{
    double total = 0.0;
    int count = 0;
    for (const OrderBookEntry &e : orders)
    {
        if (e.orderType == OrderBookType::ask)
        {
            total += e.price;
            count++;
        }
    }
    return count > 0 ? total / count : 0.0; //handle empty ask gracefully
}

double OrderBook::getOpeningAsk(std::vector<OrderBookEntry>& orders)
{
    if (orders.empty())
    {
        return 0.0;
    }
    //return the frirst price in the vector(Opening ask)
    if (orders.front().orderType == OrderBookType::ask)
    {
        return orders.front().price;  
    }

    return 0.0;
}

double OrderBook::getClosingAsk(std::vector<OrderBookEntry> &orders) 
{
    if (orders.empty())
    {
        return 0.0;
    }
    //return the last price in the vector(closing ask)
    if (orders.back().orderType == OrderBookType::ask)
    {
        return orders.back().price;
    }

    return 0.0;
}

std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp)
{

    std::string next_timestamp = "";
    for (OrderBookEntry &e : orders)
    {
        if (e.timestamp > timestamp)
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}

void OrderBook::insertOrder(OrderBookEntry & order)
{
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    // asks = orderbook.asks in this timeframe
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask,
                                                    product,
                                                    timestamp);
    // bids = orderbook.bids in this timeframe
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid,
                                                    product,
                                                    timestamp);
    // sales = []
    std::vector<OrderBookEntry> sales;
    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
    // for ask in asks:
    for (OrderBookEntry &ask : asks)
    {
        // for bid in bids:
        for (OrderBookEntry &bid : bids)
        {
            // if bid.price >= ask.price # we have a match
            if (bid.price >= ask.price)
            {
                // sale = new orderbookentry()
                // sale.price = ask.price
                OrderBookEntry sale{ask.price, 0, timestamp, product, OrderBookType::sale};
                /* now work out how much was sold and 
    #create new bids and ask covering 
    #anything that was not sold */
                // if bid.amount == ask.amount: # bid completely clears ask
                if (bid.amount == ask.amount)
                {
                    // sale.amount = ask.amount
                    sale.amount = ask.amount;
                    // sales.append(sale)
                    sales.push_back(sale);
                    // bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
                    // # can do no more with this ask
                    // # go onto the next ask
                    // break
                    break;
                }

                // if bid.amount > ask.amount: # ask is completely gone slice the bid
                if (bid.amount > ask.amount)
                {
                    // sale.amount = ask.amount
                    sale.amount = ask.amount;
                    // sales.append(sale)
                    sales.push_back(sale);
                    // # we adjust the bid in place
                    // # so it can be used to process the next ask
                    // bid.amount = bid.amount - ask.amount
                    bid.amount = bid.amount - ask.amount;
                    // # ask is completely gone, so go to next ask
                    // break
                    break;
                }

                // if bid.amount < ask.amount # bid is completely gone, slice the ask
                if (bid.amount < ask.amount)
                {
                    // sale.amount = bid.amount
                    sale.amount = bid.amount;
                    // sales.append(sale)
                    sales.push_back(sale);

                    // # update the ask
                    // # and allow further bids to process the remaining amount
                    // ask.amount = ask.amount - bid.amount
                    ask.amount = ask.amount - bid.amount;

                    // bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
                    //2
                    // # some ask remains so go to the next bid
                    // continue
                    continue;
                }
            }
        }
    }
    // return sales
    return sales;
}
#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>

/** construct, reading a csv data file */
OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}

/** return vector of all know products in the dataset*/
std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products;

    std::map<std::string, bool> prodMap;

    for (OrderBookEntry &e : orders)
    {
        prodMap[e.product] = true;
    }

    // now flatten the map to a vector of strings
    for (auto const &e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}
/** return vector of Orders according to the sent filters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                                 std::string product,
                                                 std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry &e : orders)
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

std::vector<OrderBookEntry> OrderBook::getOrdersinTimeRange(OrderBookType type,
                                                            std::string product,
                                                            std::string timestamp,
                                                            int t)
{
    std::vector<OrderBookEntry> orders_sub;
    int i = 0;
    int index;
    std::vector<std::string> times;
    times.push_back(orders[0].timestamp);

    for (OrderBookEntry &e : orders)
    {
        if (e.timestamp != times[i])
        {
            times.push_back(e.timestamp);
            i++;
        }
        if (e.timestamp == timestamp)
        {
            index = i;
        }
    }

    for (int i = 0; i < t; i++)
    {
        for (OrderBookEntry &e : orders)
        {
            if (e.orderType == type &&
                e.product == product &&
                e.timestamp == times[index - i])
            {
                orders_sub.push_back(e);
            }
        }
    }

    return orders_sub;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry> &orders)
{
    double max = orders[0].price;
    for (OrderBookEntry &e : orders)
    {
        if (e.price > max)
            max = e.price;
    }
    return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry> &orders)
{
    double min = orders[0].price;
    for (OrderBookEntry &e : orders)
    {
        if (e.price < min)
        {
            min = e.price;
        }
    }
    return min;
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

std::string OrderBook::getLastTimestamps(std::string timestamp)
{
    int i = 0;
    std::vector<std::string> arrTimestamps;
    arrTimestamps.push_back(orders[0].timestamp);

    for (OrderBookEntry &e : orders)
    {
        if (e.timestamp != arrTimestamps[i])
        {
            arrTimestamps.push_back(e.timestamp);
            i++;
        }
    }
    for (int j = 0; j < arrTimestamps.size(); j++)
    {
        if (timestamp == arrTimestamps[j] && j > 0)
        {
            return arrTimestamps[j - 1];
        }
        if (timestamp == arrTimestamps[j] && j == 0)
        {
            return arrTimestamps[0];
        }
    }
}

std::vector<std::string> OrderBook::getTimestamps()
{
    int i = 0;
    std::vector<std::string> arrTimestamps;
    arrTimestamps.push_back(orders[0].timestamp);

    std::vector<std::string> timestamps;
    for (OrderBookEntry &e : orders)
    {
        if (e.timestamp != arrTimestamps[i])
        {
            arrTimestamps.push_back(e.timestamp);
            i++;
        }
    }

    return arrTimestamps;
}

void OrderBook::insertOrder(OrderBookEntry &order)
{
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    // asks = orderbook.asks
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask,
                                                 product,
                                                 timestamp);
    // bids = orderbook.bids
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid,
                                                 product,
                                                 timestamp);

    // sales = []
    std::vector<OrderBookEntry> sales;

    // I put in a little check to ensure we have bids and asks
    // to process.
    if (asks.size() == 0 || bids.size() == 0)
    {
        std::cout << " OrderBook::matchAsksToBids no bids or asks" << std::endl;
        return sales;
    }

    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
    // for ask in asks:
    std::cout << "max ask " << asks[asks.size() - 1].price << std::endl;
    std::cout << "min ask " << asks[0].price << std::endl;
    std::cout << "max bid " << bids[0].price << std::endl;
    std::cout << "min bid " << bids[bids.size() - 1].price << std::endl;
    
    // Verify if any matches can be made
    if (bids[0].price < asks[0].price)
    {
        std::cout << " OrderBook::matchAsksToBids No matches between bid and ask prices" << std::endl;
        return sales;
    }

    int counter = 0; // Asks counter
    for (OrderBookEntry &bid : bids)
    {
        //     for bid in bids:
        while (counter < asks.size())
        {
            //         if bid.price >= ask.price # we have a match
            if (bid.price >= asks[counter].price)
            {
                //             sale = new order()
                //             sale.price = ask.price
                OrderBookEntry sale{asks[counter].price, 0, timestamp,
                                    product,
                                    OrderBookType::asksale};

                if (bid.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (asks[counter].username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::asksale;
                }

                //             # now work out how much was sold and
                //             # create new bids and asks covering
                //             # anything that was not sold
                //             if bid.amount == ask.amount: # bid completely clears ask
                if (bid.amount == asks[counter].amount)
                {
                    //                 sale.amount = ask.amount
                    sale.amount = asks[counter].amount;
                    //                 sales.append(sale)
                    sales.push_back(sale);
                    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
                    //                 # can do no more with this ask
                    //                 # go onto the next ask
                    //                 break

                    //                 # As you can do no more with this ask
                    //                 # we can use the counter to skip the line instead of wasting recources rereadiang the used line
                    counter++;
                    break;
                }
                //           if bid.amount > ask.amount:  # ask is completely gone slice the bid
                if (bid.amount > asks[counter].amount)
                {
                    //                 sale.amount = ask.amount
                    sale.amount = asks[counter].amount;
                    //                 sales.append(sale)
                    sales.push_back(sale);
                    //                 # we adjust the bid in place
                    //                 # so it can be used to process the next ask
                    //                 bid.amount = bid.amount - ask.amount
                    bid.amount = bid.amount - asks[counter].amount;
                    //                 # ask is completely gone, so go to next ask
                    //                 break

                    //              # As you can do no more with this ask
                    //              # we can use the counter to skip the line instead of wasting recources rereadiang the used line in the next iteration
                    counter++;
                    break;
                }

                //             if bid.amount < ask.amount # bid is completely gone, slice the ask
                if (bid.amount < asks[counter].amount && bid.amount > 0)
                {
                    //                 sale.amount = bid.amount
                    sale.amount = bid.amount;
                    //                 sales.append(sale)
                    sales.push_back(sale);
                    //                 # update the ask
                    //                 # and allow further bids to process the remaining amount
                    //                 ask.amount = ask.amount - bid.amount
                    asks[counter].amount = asks[counter].amount - bid.amount;
                    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
                    //                 # some ask remains so go to the next bid
                    //                 continue
                    //                 The counter isn't changed as there is leftover ask amounts which can be used by the next user
                    continue;
                }
            }
            else
            {
                break;
            }
        }
    }
    return sales;
}
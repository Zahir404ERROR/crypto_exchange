#include "TradeBotMain-MarkThis.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"

// intro to bots and users
std::string BotIntro = "advisorbot> ";
std::string UserIntro = "user> ";

TradeBotMain::TradeBotMain()
{
}

// Initiate the bot
void TradeBotMain::init()
{
    std::string input;
    currentTime = orderBook.getEarliestTime();
    wallet.insertCurrency("BTC", 10);

    while (true)
    {
        printCommands();
        input = GetCommands();
        CommandParsingSystem(input);
    }
}

// Print intro command
void TradeBotMain::printCommands()
{
    std::cout << "\n"
              << BotIntro << "Enter Your Command (eg.Help): " << std::endl;
}

// Print intro command
std::string TradeBotMain::GetCommands()
{
    std::cout << UserIntro;
    std::string userCommand;
    std::getline(std::cin, userCommand);
    return userCommand;
}

// Command parsing code
void TradeBotMain::CommandParsingSystem(std::string CommandInput)
{
    // C1: Help Command Input Checker
    if (CommandInput == "help")
    {
        c1Help();
    }

    // C2: Help CMD Input Checker
    std::string commands[8] = {"prod", "min", "max", "avg", "predict", "time", "step", "step back"};
    // array of commands
    for (int i = 0; i < 8; i++)
    {
        if (CommandInput == ("help " + commands[i]))
        {
            c2HelpCMD(commands[i]);
        }
    }

    // C3: Print Products Input Checker
    if (CommandInput == "prod")
    {
        c3Prod();
    }

    for (std::string const &p : orderBook.getKnownProducts())
    {
        // C4: Min Input Checker
        if (CommandInput == ("min " + p + " ask"))
        {
            c4MinAsk(p);
        }
        if (CommandInput == ("min " + p + " bid"))
        {
            c4MinBid(p);
        }

        // C5: Max Input Checker
        if (CommandInput == ("max " + p + " ask"))
        {
            c5MaxAsk(p);
        }
        if (CommandInput == ("max " + p + " bid"))
        {
            c5MaxBid(p);
        }

        // C6: Avg Input Checker
        std::string strTimesteps;
        std::string checkerask = "avg " + p + " ask ";
        strTimesteps = eraseSubStr(CommandInput, checkerask);
        if (strTimesteps != "")
        {
            if (CommandInput == ("avg " + p + " ask " + strTimesteps))
            {
                int timesteps = std::stoi(strTimesteps);
                c6AvgAsk(p, timesteps);
            }
        }

        std::string checkerbid = "avg " + p + " bid ";
        strTimesteps = eraseSubStr(CommandInput, checkerbid);
        if (strTimesteps != "")
        {
            if (CommandInput == ("avg " + p + " bid " + strTimesteps))
            {
                int timesteps = std::stoi(strTimesteps);
                c6AvgBid(p, timesteps);
            }
        }

        // C7: Predict Input Checker
        if ((CommandInput == "predict min " + p + " bid") || (CommandInput == "predict max " + p + " bid"))
        {
            c7PredictBid(p);
        }
        if ((CommandInput == "predict min " + p + " ask") || (CommandInput == "predict max " + p + " ask"))
        {
            c7PredictAsk(p);
        }
    }

    // C8: Time Input Checker
    if (CommandInput == "time")
    {
        c8Time();
    }

    // C9: Time Step Checker
    if (CommandInput == "step")
    {
        c9Step();
    }

    // C9: Time Step Back Checker
    if (CommandInput == "step back")
    {
        c10StepBack();
    }
}

// Help command prints commands
void TradeBotMain::c1Help()
{
    std::cout << "The available commands are help, help <cmd>, prod, min, max, avg, predict, time, step, step back" << std::endl;
}

// Help command prints ecample commands
void TradeBotMain::c2HelpCMD(std::string HelpCommand)
{
    //help command - list available products
    if (HelpCommand == "prod")
    {
        std::cout << "prod -> list available products eg.ETH/BTC,DOGE/BTC" << std::endl;
    }
    //help command - Minimum ask or bid
    if (HelpCommand == "min")
    {
        std::cout << "min ETH/BTC ask -> The min ask for ETH/BTC is 1.0" << std::endl;
    }
    //help command -  Maximum ask or bid
    if (HelpCommand == "max")
    {
        std::cout << "max ETH/BTC ask -> The max ask for ETH/BTC is 1.0" << std::endl;
    }
    //help command - average ask or bid
    if (HelpCommand == "avg")
    {
        std::cout << "avg ETH/BTC bid 10 -> Average ETH/BTC bid over last 10 time steps" << std::endl;
    }
    //help command prediction max - average ask or bid
    if (HelpCommand == "predict")
    {
        std::cout << "predict max ETH/BTC bid -> Prediction using a moving average" << std::endl;
    }
    //help command - time
    if (HelpCommand == "time")
    {
        std::cout << "time -> states current time in dataset, i.e. which timeframe are we looking at" << std::endl;
    }
    //help command - step
    if (HelpCommand == "step")
    {
        std::cout << "step -> move to next time step" << std::endl;
    }
    //help command - step back
    if (HelpCommand == "step back")
    {
        std::cout << "step back-> move to previous time step" << std::endl;
    }
}
// Prints ptoducts in time period
void TradeBotMain::c3Prod()
{
    std::cout << BotIntro;
    for (std::string const &p : orderBook.getKnownProducts())
    {
        std::cout << " " << p << " ";
    }
    std::cout << std::endl;
}

// Finds min Ask for a product in time period
void TradeBotMain::c4MinAsk(std::string ProductName)
{
    for (std::string const &p : orderBook.getKnownProducts())
    {
        std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask, p, currentTime);
        if (ProductName == p)
        {
            std::cout << BotIntro << "The min ask for " << p << " is " << OrderBook::getLowPrice(askEntries) << std::endl;
        }
    }
}

// Finds min Bid for a product in time period
void TradeBotMain::c4MinBid(std::string ProductName)
{
    for (std::string const &p : orderBook.getKnownProducts())
    {
        std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid, p, currentTime);
        if (ProductName == p)
        {
            std::cout << BotIntro << "The min bid for " << p << " is " << OrderBook::getLowPrice(bidEntries) << std::endl;
        }
    }
}

// Finds max Ask for a product in time period
void TradeBotMain::c5MaxAsk(std::string ProductName)
{
    for (std::string const &p : orderBook.getKnownProducts())
    {
        std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask, p, currentTime);
        if (ProductName == p)
        {
            std::cout << BotIntro << "The max ask for " << p << " is " << OrderBook::getHighPrice(askEntries) << std::endl;
        }
    }
}

void TradeBotMain::c5MaxBid(std::string ProductName)
{
    for (std::string const &p : orderBook.getKnownProducts())
    {
        std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid, p, currentTime);
        if (ProductName == p)
        {
            std::cout << BotIntro << "The max bid for " << p << " is " << OrderBook::getHighPrice(bidEntries) << std::endl;
        }
    }
}

void TradeBotMain::c6AvgAsk(std::string ProductName, int t)
{
    std::vector<OrderBookEntry> askEntries = orderBook.getOrdersinTimeRange(OrderBookType::ask,
                                                                            ProductName,
                                                                            currentTime, t);
    double average = 0;
    for (int i = 0; i < askEntries.size(); i++)
    {
        average = average + askEntries[i].price;
    }
    average = average / askEntries.size();
    std::cout << BotIntro << "The average " << ProductName << " ask price over the last " << t << " timesteps was " << average << std::endl;
}

void TradeBotMain::c6AvgBid(std::string ProductName, int t)
{
    std::vector<OrderBookEntry> bidEntries = orderBook.getOrdersinTimeRange(OrderBookType::bid,
                                                                            ProductName,
                                                                            currentTime, t);
    double averageBid = 0;
    for (int i = 0; i < bidEntries.size(); i++)
    {
        averageBid = averageBid + bidEntries[i].price;
    }
    averageBid = averageBid / bidEntries.size();
    std::cout << BotIntro << "The average " << ProductName << " ask price over the last " << t << " timesteps was " << averageBid << std::endl;
}

void TradeBotMain::c7PredictAsk(std::string ProductName)
{
    std::vector<std::string> arrTimestamps = orderBook.getTimestamps();
    int index = searchVector(arrTimestamps, currentTime);
    std::vector<double> averages;
    double average;
    double SimpleMovingAverage;
    int counter;

    std::vector<OrderBookEntry> askEntries = orderBook.getOrdersinTimeRange(OrderBookType::ask,
                                                                            ProductName,
                                                                            currentTime, index + 1);

    for (int i = 0; i <= index; i++)
    {
        average = 0;
        counter = 0;
        for (int j = 0; j < askEntries.size(); j++)
        {
            if (askEntries[j].timestamp == arrTimestamps[i])
            {
                average = average + askEntries[j].price;
                counter++;
            }
        }
        average = average / counter;
        std::cout << "Average" << arrTimestamps[i] << " - " << average << std::endl;
        averages.push_back(average);
    }

    for (int i = 0; i < averages.size(); i++)
    {
        SimpleMovingAverage = SimpleMovingAverage + averages[i];
    }
    SimpleMovingAverage = SimpleMovingAverage / averages.size();
    std::cout << "Simple Moving Average " << SimpleMovingAverage << std::endl;
}

void TradeBotMain::c7PredictBid(std::string ProductName)
{
    std::vector<std::string> arrTimestamps = orderBook.getTimestamps();
    int index = searchVector(arrTimestamps, currentTime);
    std::vector<double> averages;
    double average;
    double SimpleMovingAverage;
    int counter;

    std::vector<OrderBookEntry> askEntries = orderBook.getOrdersinTimeRange(OrderBookType::bid,
                                                                            ProductName,
                                                                            currentTime, index + 1);

    for (int i = 0; i <= index; i++)
    {
        average = 0;
        counter = 0;
        for (int j = 0; j < askEntries.size(); j++)
        {
            if (askEntries[j].timestamp == arrTimestamps[i])
            {
                average = average + askEntries[j].price;
                counter++;
            }
        }
        average = average / counter;
        std::cout << "Average " << arrTimestamps[i] << " - " << average << std::endl;
        averages.push_back(average);
    }

    for (int i = 0; i < averages.size(); i++)
    {
        SimpleMovingAverage = SimpleMovingAverage + averages[i];
    }
    SimpleMovingAverage = SimpleMovingAverage / averages.size();
    std::cout << "Simple Moving Average " << SimpleMovingAverage << std::endl;
}

void TradeBotMain::c8Time()
{
    std::cout << currentTime << std::endl;
}

void TradeBotMain::c9Step()
{
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "now at " << currentTime << std::endl;
}

void TradeBotMain::c10StepBack()
{
    currentTime = orderBook.getLastTimestamps(currentTime);
    std::cout << "now at " << currentTime << std::endl;
}

// ===================================== Helper Functions =======================================

std::string TradeBotMain::eraseSubStr(std::string &mainStr, const std::string &toErase)
{
    int tempTimesteps = 0;
    std::string temp = mainStr;
    // Search for the substring in string
    size_t pos = temp.find(toErase);
    if (pos != std::string::npos)
    {
        // If found then erase it from string
        temp.erase(pos, toErase.length());
        tempTimesteps = std::stoi(temp);
        return temp;
    }
    else
    {
        return "";
    }
}

int TradeBotMain::searchVector(std::vector<std::string> v, std::string s)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (s == v[i])
        {
            return i;
        }
        else
        {
        }
    }
}
double getMean(double sum, int period)
{
    return sum / period;
}
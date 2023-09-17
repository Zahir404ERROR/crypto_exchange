#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"


class TradeBotMain
{
    public:
        TradeBotMain();
        /** Call this to start the sim */
        void init();
    private: 
        /** Parses and Implements User Input */
        void CommandParsingSystem(std::string CommandInput);
        /** Prints Menu */
        void printCommands();
        /** Gets user Command */
        std::string GetCommands();
        /** Helps users by listing program functions  */
        void c1Help();
        /** Detailed help for each function passed through  */
        void c2HelpCMD(std::string CommandInput);
        /** Lists available products for the current time period  */
        void c3Prod();
        /** Calculates minimum ask price  */
        void c4MinAsk(std::string ProductName);
        /** Calculates minimum bid price  */
        void c4MinBid(std::string ProductName);
        /** Calculates max ask price  */
        void c5MaxAsk(std::string ProductName);
        /** Calculates max bid price  */
        void c5MaxBid(std::string ProductName);
        /** Calculates average ask price of a product within the current time period */
        void c6AvgAsk(std::string ProductName, int t);
        /** Calculates average bid price of a product within the current time period */
        void c6AvgBid(std::string ProductName, int t);
        /** Predicts ask price using moving average */
        void c7PredictAsk(std::string ProductName);
        /** Predicts bid price using moving average */
        void c7PredictBid(std::string ProductName);
        /** Displays current time */
        void c8Time();
        /** Moves to next time stamp */
        void c9Step();
        /** Returns to prvious time stamp */
        void c10StepBack();

        /** Helper Function */
        std::string eraseSubStr(std::string & mainStr, const std::string & toErase);
        /** Helper Function */
        int searchVector(std::vector<std::string> v, std::string s);

        /** Current Time */
        std::string currentTime;

        /** Data passed to orderBook to be read and organized */      
	    OrderBook orderBook{"20200601.csv"};
        /** Walet object*/
        Wallet wallet;

};

#include "Wallet.h"
#include <iostream>
#include "MerkelMain.h"
#include "TradeBotMain-MarkThis.h"


int main()
{   
            
    std::cout << "\n-----------------------MerklerExchange-----------------------" << std::endl;
    
/* ======================================= Bot or Manual Selector ======================================== */


    int userOption = 0;
    bool flag = true;
    std::string line;
    std::cout << "Do you want to go into Manual Mode (1), Bot Mode (2) or Quit (3)" << std::endl;
    std::getline(std::cin, line);
    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception &e)
    {
        //
    }
        
    if (userOption == 1){
        MerkelMain MerkelMainapp{};
        MerkelMainapp.init();
    }
    if (userOption == 2){
        TradeBotMain TradeBotMainapp{};
        TradeBotMainapp.init();
    }
    if (userOption == 3){
    }
    else{
        std::cout << "Wrong Input" << std::endl;
    }
}

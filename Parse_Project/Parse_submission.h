#pragma once

#include <string>

enum class PragmaType
{
    bid,
    ask
};

class PragmaEntry
{
public:
    PragmaEntry(double _price,
                double _amount,
                std::string _timestamp,
                std::string _product,
                PragmaType _PragmaType);

    // : price(_price),
    //   amount(_amount),
    //   timestamp(_timestamp),
    //   product(_product),
    //   orderType(_orderType)
    // price = _price;
    // amount = _amount;
    // timestamp = _timestamp;
    // product = _product;
    // orderType = _orderType;
    // this->price = _price;

    double price;
    double amount;
    std::string timestamp;
    std::string product;
    PragmaType PragmaType;
};

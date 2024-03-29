/*
 * Copyright (C) 2021  Simeon Huang <symeon@librehat.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CARTERA_FINANCIALINSTRUMENT_H
#define CARTERA_FINANCIALINSTRUMENT_H

#include <string>
#include <optional>
#include <chrono>
#include <ostream>

#include "feedsource.h"

namespace cartera {

using datetime = std::chrono::time_point<std::chrono::system_clock>;

enum class asset_class
{
    Equity,
    ETF,
    Index,
    MutualFund,
    Cash,  // cash or cash equivalent
    Future,
    Other,  // This list should be expanded
    FX,
    CryptoCurrency,
    Option,

    // New asset class should only be added right before k_END
    k_END,
};
std::ostream& operator<<(std::ostream& os, const asset_class& rhs);


/**
 * Defines the structure that contains basic information about a symbol
 */
struct symbol_search_result
{
    asset_class type;
    std::string symbol;
    feed_source source;
    std::string exchange_code;
    std::string name;  // long name, or short name if long name doesn't exist
};
std::ostream& operator<<(std::ostream& os, const symbol_search_result& rhs);
bool operator==(const symbol_search_result& lhs, const symbol_search_result& rhs);
bool operator!=(const symbol_search_result& lhs, const symbol_search_result& rhs);


/**
 * Defines the structure of financial instruments data
 */
struct financial_instrument
{
    asset_class type;
    
    // TODO: Yahoo Finance doesn't give this data.
    // std::string isin;  //  ISIN identifies a security (which might be traded at different exchanges under different symbols)
    
    std::string symbol;  // symbol is unique at exchange-level
    std::string currency;  // ISO4217 currency code (most of the time with exception for crypto currency)
    std::string exchange_code;  // shortened exchange code (e.g. LSE, FRA)
    std::string long_name;
    std::string short_name;
    int price_decimal_places;
};
std::ostream& operator<<(std::ostream& os, const financial_instrument& rhs);
bool operator==(const financial_instrument& lhs, const financial_instrument& rhs);
bool operator!=(const financial_instrument& lhs, const financial_instrument& rhs);


/**
 * Defines the real-time quote data
 */
struct quote
{
    datetime updated_time;
    std::string symbol;
    feed_source source;
    std::optional<double> day_low_price;
    std::optional<double> day_high_price;
    std::optional<double> day_open_price;
    double prev_day_close_price;
    double current_price;
    std::optional<double> volume;
    double is_market_open;
    std::optional<double> market_cap;  // nullable double
};


struct symbol_detail
{
    std::optional<double> day_low_price;
    std::optional<double> day_high_price;
    std::optional<double> day_open_price;
    double prev_day_close_price;
    std::optional<double> bid_price;
    std::optional<double> bid_qty;
    std::optional<double> ask_price;
    std::optional<double> ask_qty;
    std::optional<double> volume;
    std::optional<double> average_ten_day_volume;
    std::optional<double> fifty_two_week_high_price;
    std::optional<double> fifty_two_week_low_price;
    std::optional<double> market_cap;
    std::optional<double> dividend_rate;
    std::optional<double> dividend_yield;
    std::optional<datetime> ex_dividend_date;
    std::optional<double> beta;
};

template<typename T>
std::ostream operator<<(std::ostream& os, const std::optional<T>& val)
{
    os << val.has_value() ? val.value() : "null";
    return os;
}

}

#endif // CARTERA_FINANCIALINSTRUMENT_H

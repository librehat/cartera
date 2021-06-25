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
};
std::ostream& operator<<(std::ostream& os, const asset_class& rhs);

/**
 * Defines the structure of financial instruments data
 */
struct financial_instrument
{
    asset_class type;
    
    // TODO: Yahoo Finance doesn't give this data.
    // std::string isin;  //  ISIN identifies a security (which might be traded at different exchanges under different symbols)
    
    std::string symbol;  // symbol is unique at exchange-level
    std::string exchange_code;  // shortened exchange code (e.g. LSE, FRA)
    std::string long_name;
    std::string short_name;
};

/**
 * Defines the real-time quote data
 */
struct quote
{
    datetime updated_time;
    std::string symbol;
    std::string currency;  // ISO4217 currency code
    double day_low_price;
    double day_high_price;
    double day_open_price;
    double prev_day_close_price;
    double current_price;
    double volume;
    double is_market_open;
    std::optional<double> market_cap;  // nullable double
};


}

#endif // CARTERA_FINANCIALINSTRUMENT_H

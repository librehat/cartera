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

#ifndef CARTERA_JSONPARSER_H
#define CARTERA_JSONPARSER_H

#include "types/financialinstrument.h"
#include "types/feedsource.h"

#include <vector>

namespace cartera {

template<feed_source SOURCE>
struct json_parser {
};

template<>
struct json_parser<feed_source::YahooFinance> {
    static financial_instrument parse_financial_instrument(const std::string& data);
    static quote parse_quote(const std::string& data);
    static std::vector<symbol_search_result> parse_search_quote(const std::string& data);
};

template<>
struct json_parser<feed_source::Binance> {
    static financial_instrument parse_financial_instrument(const std::string& data);
    static quote parse_quote(const std::string& data);
    // Binance doesn't have search endpoint
};

}

#endif // CARTERA_JSONPARSER_H

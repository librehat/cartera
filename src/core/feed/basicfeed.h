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

#ifndef CARTERA_FEED_BASICFEED_H
#define CARTERA_FEED_BASICFEED_H

#include "types/feedsource.h"
#include "types/financialinstrument.h"

#include "http/client.h"
#include "parser/jsonparser.h"

namespace cartera {

namespace details {
template<feed_source SOURCE>
struct symbol_url {};

template<feed_source SOURCE>
struct quote_url {};

template<feed_source SOURCE>
struct search_url {};
}  // close details namespace 

template<feed_source SOURCE>
struct basic_feed
{
    static financial_instrument resolve_symbol(const simple_http_client& client, const std::string& symbol)
    {
        const std::string resp = client.get(details::symbol_url<SOURCE>::get(symbol));
        return json_parser<SOURCE>::parse_financial_instrument(resp);
    }

    static quote resolve_quote(const simple_http_client& client, const std::string& symbol)
    {
        const std::string resp = client.get(details::quote_url<SOURCE>::get(symbol));
        return json_parser<SOURCE>::parse_quote(resp);
    }

    static std::vector<symbol_search_result> search_symbols(const simple_http_client& client, const std::string& keyword)
    {
        const std::string resp = client.get(details::search_url<SOURCE>::get(keyword));
        return json_parser<SOURCE>::parse_search_quote(resp);
    }
};


namespace details {
template<>
struct symbol_url<feed_source::YahooFinance> {
    static std::string get(const std::string& symbol)
    {
        return "https://query1.finance.yahoo.com/v10/finance/quoteSummary/" + symbol + "?modules=price";
    }
};

template<>
struct quote_url<feed_source::YahooFinance> {
    static std::string get(const std::string& symbol)
    {
        return symbol_url<feed_source::YahooFinance>::get(symbol);
    }
};

template<>
struct search_url<feed_source::YahooFinance> {
    static std::string get(const std::string& keyword)
    {
        return "https://query2.finance.yahoo.com/v1/finance/search?q=" + keyword + "&quotesCount=10&newsCount=0";
    }
};

}  // close details namespace 

}  // close cartera namespace

#endif // CARTERA_FEED_BASICFEED_H

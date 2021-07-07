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
struct urls {};
}  // close details namespace 

struct basic_feed
{
    template<feed_source SOURCE>
    static financial_instrument resolve_symbol(const simple_http_client& client, const std::string& symbol)
    {
        const std::string resp = client.get(details::urls<SOURCE>::symbol(symbol));
        return json_parser<SOURCE>::parse_financial_instrument(resp);
    }

    template<feed_source SOURCE>
    static quote resolve_quote(const simple_http_client& client, const std::string& symbol)
    {
        const std::string resp = client.get(details::urls<SOURCE>::quote(symbol));
        return json_parser<SOURCE>::parse_quote(resp);
    }

    template<feed_source SOURCE>
    static std::vector<symbol_search_result> search_symbols(const simple_http_client& client, const std::string& keyword)
    {
        const std::string resp = client.get(details::urls<SOURCE>::search(keyword));
        return json_parser<SOURCE>::parse_search_quote(resp);
    }
};

template<>
std::vector<symbol_search_result> basic_feed::search_symbols<feed_source::Binance>(const simple_http_client& client, const std::string& keyword);


namespace details {
template<>
struct urls<feed_source::YahooFinance> {
    static std::string symbol(const std::string& symbol)
    {
        return "https://query1.finance.yahoo.com/v10/finance/quoteSummary/" + symbol + "?modules=price";
    }
    
    static std::string quote(const std::string& symbol)
    {
        return urls::symbol(symbol);
    }
    
    static std::string search(const std::string& keyword)
    {
        return "https://query2.finance.yahoo.com/v1/finance/search?q=" + keyword + "&quotesCount=10&newsCount=0";
    }
};

template<>
struct urls<feed_source::Binance> {
    static std::string symbol(const std::string& symbol)
    {
        return "https://api.binance.com/api/v3/exchangeInfo?symbol=" + symbol;
    }

    static std::string quote(const std::string& symbol)
    {
        return "https://api.binance.com/api/v3/ticker/24hr?symbol=" + symbol;
    }

    static std::string search(const std::string&)
    {
        return "";  // TODO
    }
};

}  // close details namespace 

}  // close cartera namespace

#endif // CARTERA_FEED_BASICFEED_H

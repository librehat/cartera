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
namespace feed {

namespace details {
template<feed_source SOURCE>
struct urls {};
}  // close details namespace 

struct basic_feed
{
    template<feed_source SOURCE>
    static financial_instrument resolve_symbol(const simple_http_client& client, const std::string_view& symbol)
    {
        const std::string resp = client.get(details::urls<SOURCE>::symbol(symbol));
        return json_parser<SOURCE>::parse_financial_instrument(resp);
    }

    template<feed_source SOURCE>
    static quote resolve_quote(const simple_http_client& client, const std::string_view& symbol)
    {
        const std::string resp = client.get(details::urls<SOURCE>::quote(symbol));
        return json_parser<SOURCE>::parse_quote(resp);
    }

    template<feed_source SOURCE>
    static std::vector<quote> resolve_quotes(const simple_http_client& client, const std::vector<std::string_view>& symbols)
    {
        // The default implementation simply calls 'resolve_quote' multiple times.
        // Any feeds that have more optimised endpoints should implement it with template specialisation.
        std::vector<quote> results;
        results.reserve(symbols.size());
        for (const auto& symbol : symbols) {
            results.push_back(resolve_quote<SOURCE>(client, symbol));
        }
        return results;
    }

    template<feed_source SOURCE>
    static std::vector<symbol_search_result> search_symbols(const simple_http_client& client, const std::string_view& keyword)
    {
        const std::string resp = client.get(details::urls<SOURCE>::search(keyword));
        return json_parser<SOURCE>::parse_search_quote(resp);
    }
};

template<>
std::vector<quote> basic_feed::resolve_quotes<feed_source::YahooFinance>(const simple_http_client& client, const std::vector<std::string_view>& symbols);

template<>
std::vector<symbol_search_result> basic_feed::search_symbols<feed_source::Binance>(const simple_http_client& client, const std::string_view& keyword);


namespace details {
template<>
struct urls<feed_source::YahooFinance> {
    static std::string symbol(const std::string_view& symbol)
    {
        return "https://query1.finance.yahoo.com/v10/finance/quoteSummary/" + std::string{ symbol } + "?modules=price";
    }

    static std::string quote(const std::string_view& symbol)
    {
        return urls::symbol(symbol);
    }

    static std::string search(const std::string_view& keyword)
    {
        return "https://query2.finance.yahoo.com/v1/finance/search?q=" + std::string{ keyword } + "&quotesCount=10&newsCount=0";
    }
};

template<>
struct urls<feed_source::Binance> {
    static std::string symbol(const std::string_view& symbol)
    {
        return "https://api.binance.com/api/v3/exchangeInfo?symbol=" + std::string{ symbol };
    }

    static std::string quote(const std::string_view& symbol)
    {
        return "https://api.binance.com/api/v3/ticker/24hr?symbol=" + std::string{ symbol };
    }

    static const std::string& search(const std::string_view&)
    {
        static const std::string url{ "https://api.binance.com/api/v3/exchangeInfo" };
        return url;
    }
};

}  // close details namespace 

}  // close feed namespace
}  // close cartera namespace

#endif // CARTERA_FEED_BASICFEED_H

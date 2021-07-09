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

#include "basicfeed.h"

#include <algorithm>
#include <iterator>
#include <mutex>
#include <sstream>

std::optional<std::vector<cartera::symbol_search_result>> g_binance_all_symbols;
std::mutex g_binance_all_symbols_mutex;

namespace cartera {
namespace feed {

template<>
std::vector<quote> basic_feed::resolve_quotes<feed_source::YahooFinance>(
    const simple_http_client& client, const std::vector<std::string_view>& symbols)
{
    std::ostringstream oss;
    oss << "https://query2.finance.yahoo.com/v7/finance/quote?symbols=";
    std::copy(symbols.begin(), symbols.end() - 1, std::ostream_iterator<std::string_view>(oss, ","));
    oss << symbols.back()
        << "&fields=symbol,regularMarketPrice,regularMarketTime,regularMarketVolume,regularMarketDayRange,regularMarketOpen,regularMarketPreviousClose,marketCap";
    const std::string resp = client.get(oss.str());
    return json_parser<feed_source::YahooFinance>::parse_quotes(resp);
}

template<>
std::vector<symbol_search_result> basic_feed::search_symbols<feed_source::Binance>(
    const simple_http_client& client, const std::string_view& keyword)
{
    // Binance doesn't have search API. We list all known symbols at the exchange and then search it
    // To improve the performance and to not exceed the rate limits, we cache the full list in-memory.

    std::lock_guard<std::mutex> lock_guard(g_binance_all_symbols_mutex);
    if (!g_binance_all_symbols.has_value()) {
        const std::string resp = client.get(details::urls<feed_source::Binance>::search(keyword));
        g_binance_all_symbols = json_parser<feed_source::Binance>::parse_search_quote(resp);
    }

    std::vector<symbol_search_result> results;
    std::copy_if(
        g_binance_all_symbols.value().begin(),
        g_binance_all_symbols.value().end(),
        std::back_inserter(results),
        [&keyword](const symbol_search_result& symbol) -> bool {
            return symbol.symbol.find(keyword) != std::string::npos;
        });
    return results;
}

}  // close feed namespace
}  // close cartera namespace

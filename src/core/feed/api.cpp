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

#include "api.h"
#include "types/exceptions.h"

#include <array>
#include <unordered_map>
#include <sstream>

namespace cartera {

namespace {

template <feed_source S>
struct feed_aggregator {
    static void search_symbols(std::vector<symbol_search_result>& output, const simple_http_client& http_client, const std::string_view& keyword) {
        auto res = feed::basic_feed::search_symbols<S>(http_client, keyword);
        output.insert(
            output.end(),
            std::make_move_iterator(res.begin()),
            std::make_move_iterator(res.end())
        );
        feed_aggregator<static_cast<feed_source>(static_cast<int>(S) + 1)>::search_symbols(output, http_client, keyword);
    }
};

template<>
struct feed_aggregator<feed_source::k_END> {
    static void search_symbols(std::vector<symbol_search_result>&, const simple_http_client&, const std::string_view&) {
        return;
    }
};


template<feed_source S>
struct feed_finder {
    static financial_instrument get_financial_instrument(feed_source source, const simple_http_client& http_client, const std::string_view& symbol) {
        if (S != source) {
            return feed_finder<static_cast<feed_source>(static_cast<int>(S) + 1)>::get_financial_instrument(source, http_client, symbol);
        }
        return feed::basic_feed::resolve_symbol<S>(http_client, symbol);
    }

    static quote get_quote(feed_source source, const simple_http_client& http_client, const std::string_view& symbol) {
        if (S != source) {
            return feed_finder<static_cast<feed_source>(static_cast<int>(S) + 1)>::get_quote(source, http_client, symbol);
        }
        return feed::basic_feed::resolve_quote<S>(http_client, symbol);
    }

    static std::vector<quote> get_quotes(feed_source source, const simple_http_client& http_client, const std::vector<std::string_view>& symbols) {
        if (S != source) {
            return feed_finder<static_cast<feed_source>(static_cast<int>(S) + 1)>::get_quotes(source, http_client, symbols);
        }
        return feed::basic_feed::resolve_quotes<S>(http_client, symbols);
    }
};

template<>
struct feed_finder<feed_source::k_END> {
    static financial_instrument get_financial_instrument(feed_source source, const simple_http_client&, const std::string_view&) {
        std::ostringstream oss;
        oss << "Cannot find the feed source=" << source;
        throw cartera_exception(oss.str());
    }

    static quote get_quote(feed_source source, const simple_http_client&, const std::string_view&) {
        std::ostringstream oss;
        oss << "Cannot find the feed source=" << source;
        throw cartera_exception(oss.str());
    }

    static std::vector<quote> get_quotes(feed_source source, const simple_http_client&, const std::vector<std::string_view>&) {
        std::ostringstream oss;
        oss << "Cannot find the feed source=" << source;
        throw cartera_exception(oss.str());
    }
};

}  // close unnamed namespace

namespace feed {
api::api()
{}

std::vector<symbol_search_result> api::search_symbols(const std::string_view& keyword) const
{
    std::vector<symbol_search_result> out;
    feed_aggregator<feed_source::YahooFinance>::search_symbols(out, m_http_client, keyword);
    return out;
}

financial_instrument api::get_financial_instrument(const std::string_view& symbol, feed_source source)
{
    std::lock_guard<std::mutex> mutex_guard(m_fi_cache_mutex);
    auto& cache = m_financial_instruments_cache[static_cast<int>(source)];
    const std::string key{ symbol };
    auto it = cache.find(key);
    if (it == cache.end()) {
        auto res = feed_finder<feed_source::YahooFinance>::get_financial_instrument(source, m_http_client, symbol);
        cache.insert(it, { key, res });
        return res;
    }
    return it->second;
}

quote api::get_quote(const std::string_view& symbol, feed_source source) const
{
    return feed_finder<feed_source::YahooFinance>::get_quote(source, m_http_client, symbol);
}

std::vector<quote> api::get_quotes(const std::vector<std::pair<std::string_view, feed_source>>& symbols) const
{
    std::vector<quote> out;
    out.resize(symbols.size());

    std::array<std::pair<std::vector<std::string_view>, std::vector<int>>, static_cast<int>(feed_source::k_END)> inputs_array;
    for (int i = 0; i < symbols.size(); ++i) {
        inputs_array[static_cast<int>(symbols[i].second)].first.push_back(symbols[i].first);
        inputs_array[static_cast<int>(symbols[i].second)].second.push_back(i);
    }

    for (int i = 0; i < static_cast<int>(feed_source::k_END); ++i) {
        if (inputs_array[i].first.empty())    continue;
        auto quotes = feed_finder<feed_source::YahooFinance>::get_quotes(static_cast<feed_source>(i), m_http_client, inputs_array[i].first);
        for (int j = 0; j < quotes.size(); ++j) {
            out[inputs_array[i].second[j]] = std::move(quotes[j]);
        }
    }

    return out;
}

}  // close feed namespace
}  // close cartera namespace

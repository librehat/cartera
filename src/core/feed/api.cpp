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
    static quote get_quote(feed_source source, const simple_http_client& http_client, const std::string_view& symbol) {
        if (S != source) {
            return feed_finder<static_cast<feed_source>(static_cast<int>(S) + 1)>::get_quote(source, http_client, symbol);
        }
        return feed::basic_feed::resolve_quote<S>(http_client, symbol);
    }
};

template<>
struct feed_finder<feed_source::k_END> {
    static quote get_quote(feed_source, const simple_http_client&, const std::string_view&) {
        throw cartera_exception("Cannot find the feed source");
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

quote api::get_quote(const std::string_view& symbol, feed_source source) const
{
    return feed_finder<feed_source::YahooFinance>::get_quote(source, m_http_client, symbol);
}

}  // close feed namespace
}  // close cartera namespace

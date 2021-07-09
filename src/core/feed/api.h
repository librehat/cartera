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

#ifndef CARTERA_FEED_API_H
#define CARTERA_FEED_API_H

#include "basicfeed.h"
#include "http/client.h"

namespace cartera {
namespace feed {

class api {
public:
    explicit api();

    // Search symbols across all available feed sources and aggregate the results
    std::vector<symbol_search_result> search_symbols(const std::string_view& keyword) const;

    quote get_quote(const std::string_view& symbol, feed_source source) const;

    std::vector<quote> get_quotes(const std::vector<std::pair<std::string_view, feed_source>>& symbols) const;

private:
    simple_http_client m_http_client;
};

}  // close feed namespace
}  // close cartera namespace

#endif

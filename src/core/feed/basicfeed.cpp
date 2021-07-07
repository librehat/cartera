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

namespace cartera {

template<>
std::vector<symbol_search_result> basic_feed::search_symbols<feed_source::Binance>(const simple_http_client& client, const std::string& keyword)
{
    // Binance doesn't have search API. We list all known symbols at the exchange and then search it
    // To improve the performance and to not exceed the rate limits, we cache the full list in-memory.
    return {}; // TODO
}

}  // close cartera namespace

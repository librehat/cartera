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
#ifndef CARTERA_FEEDSOURCE_H
#define CARTERA_FEEDSOURCE_H

#include <ostream>

namespace cartera {

enum class feed_source {
    YahooFinance,

    Binance,

    // New source should only be added right before k_END
    k_END,
};
std::ostream& operator<<(std::ostream& os, const feed_source& rhs);

}

#endif
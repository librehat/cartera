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
#ifndef CARTERA_TYPES_POSITIONIDENTIFIER_H
#define CARTERA_TYPES_POSITIONIDENTIFIER_H

#include "feedsource.h"

#include <ostream>
#include <string>

namespace cartera {

// Uniquely identifies a position (not necessarily a unique security)
struct position_identifier {
    feed_source source;
    std::string symbol;
};

bool operator==(const position_identifier& lhs, const position_identifier& rhs);
bool operator!=(const position_identifier& lhs, const position_identifier& rhs);

std::ostream& operator<<(std::ostream& os, const position_identifier& pi);

}

#endif
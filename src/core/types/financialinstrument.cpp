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

#include "financialinstrument.h"

#include <magic_enum.hpp>

namespace cartera {

std::ostream& operator<<(std::ostream& os, const asset_class& rhs) {
    os << magic_enum::enum_name(rhs);
    return os;
}

std::ostream& operator<<(std::ostream& os, const symbol_search_result& rhs)
{
    os << "{ type=" << rhs.type
       << " symbol=" << rhs.symbol
       << " source=" << rhs.source
       << " exchange=" << rhs.exchange_code
       << " name=" << rhs.name
       << " }";
    return os;
}

bool operator==(const symbol_search_result& lhs, const symbol_search_result& rhs)
{
    return lhs.source == rhs.source &&
        lhs.exchange_code == rhs.exchange_code &&
        lhs.type == rhs.type &&
        lhs.symbol == rhs.symbol &&
        lhs.name == rhs.name;
}

bool operator!=(const symbol_search_result& lhs, const symbol_search_result& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const financial_instrument& rhs) {
    os << "{ type=" << rhs.type;
    os << " symbol=" << rhs.symbol;
    os << " currency=" << rhs.currency;
    os << " exchange=" << rhs.exchange_code;
    os << " name=" << rhs.short_name << " (" << rhs.long_name << ")"
       << " priceDecimalPlaces=" << rhs.price_decimal_places << " }";
    return os;
}

bool operator==(const financial_instrument& lhs, const financial_instrument& rhs) {
    return lhs.type == rhs.type && lhs.symbol == rhs.symbol && lhs.currency == rhs.currency && lhs.exchange_code == rhs.exchange_code && lhs.short_name == rhs.short_name && lhs.long_name == rhs.long_name && lhs.price_decimal_places == rhs.price_decimal_places;
}

bool operator!=(const financial_instrument& lhs, const financial_instrument& rhs) {
    return !(lhs == rhs);
}

}

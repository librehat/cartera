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

#include "types.h"

#define GADGET_BOILER_PLATE_IMPL(CLASS, BASE) \
CLASS::CLASS(BASE&& data) : BASE(std::move(data)) {} \
CLASS::CLASS(const BASE& data) : BASE(data) {} \
CLASS& CLASS::operator=(const CLASS& rhs) noexcept { \
    dynamic_cast<BASE*>(this)->operator=(dynamic_cast<const BASE&>(rhs)); \
    return *this; \
} \
CLASS& CLASS::operator=(CLASS&& rhs) noexcept { \
    dynamic_cast<BASE*>(this)->operator=(dynamic_cast<BASE&&>(std::move(rhs))); \
    return *this; \
}


namespace cartera {

GADGET_BOILER_PLATE_IMPL(SymbolSearchResult, symbol_search_result)
GADGET_BOILER_PLATE_IMPL(FinancialInstrument, financial_instrument)
GADGET_BOILER_PLATE_IMPL(Quote, quote)

}  // close cartera namespace


#undef GADGET_BOILER_PLATE_IMPL

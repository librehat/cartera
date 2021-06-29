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

namespace cartera {
FinancialInstrument::FinancialInstrument(financial_instrument&& data) : financial_instrument(std::move(data)) {}
FinancialInstrument::FinancialInstrument(const financial_instrument& data) : financial_instrument(data) {}

FinancialInstrument& FinancialInstrument::operator=(const FinancialInstrument& rhs) noexcept
{
    dynamic_cast<financial_instrument*>(this)->operator=(dynamic_cast<const financial_instrument&>(rhs));
    return *this;
}

FinancialInstrument& FinancialInstrument::operator=(FinancialInstrument&& rhs) noexcept
{
    dynamic_cast<financial_instrument*>(this)->operator=(dynamic_cast<financial_instrument&&>(std::move(rhs)));
    return *this;
}


Quote::Quote(const quote& data) : quote(data) {}
Quote::Quote(quote&& data) : quote(std::move(data)) {}

Quote& Quote::operator=(const Quote& rhs) noexcept
{
    dynamic_cast<quote*>(this)->operator=(dynamic_cast<const quote&>(rhs));
    return *this;
}

Quote& Quote::operator=(Quote&& rhs) noexcept
{
    dynamic_cast<quote*>(this)->operator=(dynamic_cast<quote&&>(std::move(rhs)));
    return *this;
}

}  // close cartera namespace

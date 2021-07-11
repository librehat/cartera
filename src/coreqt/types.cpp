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

#include <sstream>

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


QString SymbolSearchResult::getTypeDisp() const
{
    std::ostringstream oss;
    oss << type;
    return QString::fromStdString(oss.str());
}

SymbolQuote::SymbolQuote()
    : m_source(0)
    , m_currentPrice(0)
    , m_prevDayClosePrice(0)
    , m_priceDecimalHint(2)
{}

SymbolQuote::SymbolQuote(quote&& quote, financial_instrument&& fi)
    : m_source(static_cast<int>(quote.source))
    , m_symbol(QString::fromStdString(quote.symbol))
    , m_longName(QString::fromStdString(fi.long_name))
    , m_shortName(QString::fromStdString(fi.short_name))
    , m_currency(QString::fromStdString(fi.currency))
    , m_currentPrice(quote.current_price)
    , m_prevDayClosePrice(quote.prev_day_close_price)
    , m_priceDecimalHint(2)  // FIXME
{}

SymbolQuote SymbolQuote::fromData(quote&& quote, financial_instrument&& fi)
{
    return SymbolQuote{ std::move(quote), std::move(fi) };
}

QString SymbolQuote::currentPriceDisp() const
{
    return QString("%1 %2").arg(
        m_locale.toString(m_currentPrice, 'f', m_priceDecimalHint)
    ).arg(m_currency);
}

QString SymbolQuote::priceChangeDisp() const
{
    return QString("%1%2")
        .arg(
            isPositivePriceChange() ? m_locale.positiveSign() : m_locale.negativeSign()
        ).arg(
            m_locale.toString(abs(priceChange()), 'f', m_priceDecimalHint)
        );
}

QString SymbolQuote::priceChangePercentDisp() const
{
    return QString("%1%2%3")
        .arg(
            isPositivePriceChange() ? m_locale.positiveSign() : m_locale.negativeSign()
        ).arg(
            m_locale.toString(abs(priceChange() / m_prevDayClosePrice * 100), 'f', 2)
        ).arg(
            m_locale.percent()
        );
}


}  // close cartera namespace


#undef GADGET_BOILER_PLATE_IMPL

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

#include "symbolquote.h"

namespace cartera {

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
    , m_priceDecimalHint(fi.price_decimal_places)
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

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

#include "symboldetails.h"

namespace cartera {

namespace {

template<typename T>
void add_price_value(
    const char* title, const T& value, int decimal, const QLocale& locale, QVariantList& out)
{
    QVariantMap item;
    item.insert("title", QString{ title });
    item.insert("display", locale.toString(value, 'f', decimal));
    out << item;
}

template<typename T>
void add_price_value(
    const char* title, const std::optional<T>& value, int decimal, const QLocale& locale, QVariantList& out)
{
    if (value.has_value()) {
        add_price_value<T>(title, value.value(), decimal, locale, out);
    }
}

}  // close unnamed namespace

SymbolDetails::SymbolDetails()
{}

SymbolDetails::SymbolDetails(financial_instrument&& basic, symbol_detail&& detail)
{
    add_price_value("Open", detail.day_open_price, basic.price_decimal_places, m_locale, m_details);
    add_price_value("Previous Close", detail.prev_day_close_price, basic.price_decimal_places, m_locale, m_details);
    add_price_value("Day High", detail.day_high_price, basic.price_decimal_places, m_locale, m_details);
    add_price_value("Day Low", detail.day_low_price, basic.price_decimal_places, m_locale, m_details);
    add_price_value("Bid", detail.bid_price, basic.price_decimal_places, m_locale, m_details);
    add_price_value("Ask", detail.ask_price, basic.price_decimal_places, m_locale, m_details);
    // TODO: volume should have a separate decimal places property
    add_price_value("Bid Size", detail.bid_qty, basic.price_decimal_places, m_locale, m_details);
    add_price_value("Ask Size", detail.bid_qty, basic.price_decimal_places, m_locale, m_details);
    add_price_value("Beta", detail.beta, basic.price_decimal_places, m_locale, m_details);
    add_price_value("52-Week High", detail.fifty_two_week_high_price, basic.price_decimal_places, m_locale, m_details);
    add_price_value("52-Week Low", detail.fifty_two_week_low_price, basic.price_decimal_places, m_locale, m_details);
    add_price_value("Volume", detail.volume, basic.price_decimal_places, m_locale, m_details);
    add_price_value("10-Day Average Volume", detail.average_ten_day_volume, basic.price_decimal_places, m_locale, m_details);
    add_price_value("Market Cap", detail.market_cap, basic.price_decimal_places, m_locale, m_details);
    add_price_value("Dividend Yield", detail.dividend_yield, basic.price_decimal_places, m_locale, m_details);
    // TODO: add dividend rate (%)
    if (detail.ex_dividend_date.has_value()) {
        auto date = QDateTime::fromMSecsSinceEpoch(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                detail.ex_dividend_date.value().time_since_epoch()
                ).count()
        );
        QVariantMap item;
        item.insert("title", QString{ "Ex-Dividend Date" });
        item.insert("display", m_locale.toString(date));
    }
}

}  // close cartera namespace


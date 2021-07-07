/*
 * The core types are extended here so they can be used by QMetaObject
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
#ifndef CARTERA_COREQT_TYPES_H
#define CARTERA_COREQT_TYPES_H

#define STL_STRING_PROP_IMPL(READ_FUNC, MEM_NAME) \
    inline QString READ_FUNC() const { return QString::fromStdString( MEM_NAME ); }

#include <QObject>

#include "types/financialinstrument.h"

namespace cartera {

class FinancialInstrument : public financial_instrument {
    Q_GADGET

    //TODO: asset_class not able to be registered
    // Q_PROPERTY(asset_class type MEMBER type)
    Q_PROPERTY(QString symbol READ getSymbol)
    Q_PROPERTY(QString currency READ getCurrency)
    Q_PROPERTY(QString exchangeNode READ getExchangeCode)
    Q_PROPERTY(QString shortName READ getShortName)
    Q_PROPERTY(QString longName READ getLongName)

public:
    FinancialInstrument() = default;
    FinancialInstrument(const FinancialInstrument&) = default;
    FinancialInstrument(FinancialInstrument&&) = default;

    FinancialInstrument(financial_instrument&& data);
    FinancialInstrument(const financial_instrument& data);

    FinancialInstrument& operator=(const FinancialInstrument& rhs) noexcept;
    FinancialInstrument& operator=(FinancialInstrument&& rhs) noexcept;

    STL_STRING_PROP_IMPL(getSymbol, symbol)
    STL_STRING_PROP_IMPL(getCurrency, currency)
    STL_STRING_PROP_IMPL(getExchangeCode, exchange_code)
    STL_STRING_PROP_IMPL(getShortName, short_name)
    STL_STRING_PROP_IMPL(getLongName, long_name)
};


class Quote : public quote {
    Q_GADGET
    
    // TODO: datetime updated_time
    Q_PROPERTY(QString symbol READ getSymbol)
    Q_PROPERTY(double dayLowPrice MEMBER day_low_price)
    Q_PROPERTY(double dayHighPrice MEMBER day_high_price)
    Q_PROPERTY(double dayOpenPrice MEMBER day_open_price)
    Q_PROPERTY(double prevDayClosePrice MEMBER prev_day_close_price)
    Q_PROPERTY(double currentPrice MEMBER current_price)
    Q_PROPERTY(double volume MEMBER volume)
    Q_PROPERTY(double isMarketOpen MEMBER is_market_open)
    // TODO: optional<double> market_cap

public:
    Quote() = default;
    Quote(const Quote&) = default;
    Quote(Quote&&) = default;

    Quote(const quote&);
    Quote(quote&&);

    Quote& operator=(const Quote& rhs) noexcept;
    Quote& operator=(Quote&& rhs) noexcept;

    STL_STRING_PROP_IMPL(getSymbol, symbol)
};


}  // close cartera namespace

Q_DECLARE_METATYPE(cartera::FinancialInstrument)
Q_DECLARE_METATYPE(cartera::Quote)

#undef STL_STRING_PROP_IMPL
#endif  // CARTERA_COREQT_TYPES_H
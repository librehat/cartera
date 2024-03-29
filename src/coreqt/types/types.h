/*
 * Types that can be passed from C++ to the QML side
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

#include <QDateTime>
#include <QObject>
#include <QVariant>

#include "types/financialinstrument.h"
#include "types/positionidentifier.h"

#define STL_STRING_PROP_IMPL(READ_FUNC, MEM_NAME) \
    inline QString READ_FUNC() const { return QString::fromStdString( MEM_NAME ); }

#define ENUM_CLASS_PROP_IMPL(READ_FUNC, MEM_NAME) \
    inline int READ_FUNC() const { return static_cast<int>( MEM_NAME ); }

#define OPTIONAL_PROP_IMPL(READ_FUNC, MEM_NAME) \
    inline QVariant READ_FUNC() const { \
        QVariant res; \
        if (MEM_NAME.has_value()) { res.setValue(MEM_NAME.value()); } \
        return res; \
    }

#define GADGET_BOILER_PLATE_DECL(CLASS, BASE) \
public: \
    CLASS() = default; \
    CLASS(const CLASS&) = default; \
    CLASS(CLASS&&) = default; \
    CLASS(BASE&& data); \
    CLASS(const BASE& data); \
    CLASS& operator=(const CLASS& rhs) noexcept; \
    CLASS& operator=(CLASS&& rhs) noexcept; \

namespace cartera {
 
//----------------------------------------------------------------------------
// Classes below are simply extended from the core library
//----------------------------------------------------------------------------
class SymbolSearchResult : public symbol_search_result {
    Q_GADGET

    Q_PROPERTY(int type READ getType)
    Q_PROPERTY(QString typeDisp READ getTypeDisp)
    Q_PROPERTY(QString symbol READ getSymbol)
    Q_PROPERTY(int source READ getSource)
    Q_PROPERTY(QString exchangeCode READ getExchangeCode)
    Q_PROPERTY(QString name READ getName)

    GADGET_BOILER_PLATE_DECL(SymbolSearchResult, symbol_search_result)

    ENUM_CLASS_PROP_IMPL(getType, type)
    STL_STRING_PROP_IMPL(getSymbol, symbol)
    ENUM_CLASS_PROP_IMPL(getSource, source)
    STL_STRING_PROP_IMPL(getExchangeCode, exchange_code)
    STL_STRING_PROP_IMPL(getName, name)

    QString getTypeDisp() const;
};

class FinancialInstrument : public financial_instrument {
    Q_GADGET

    Q_PROPERTY(int type READ getType)
    Q_PROPERTY(QString symbol READ getSymbol)
    Q_PROPERTY(QString currency READ getCurrency)
    Q_PROPERTY(QString exchangeNode READ getExchangeCode)
    Q_PROPERTY(QString shortName READ getShortName)
    Q_PROPERTY(QString longName READ getLongName)

    GADGET_BOILER_PLATE_DECL(FinancialInstrument, financial_instrument)
    
    ENUM_CLASS_PROP_IMPL(getType, type)
    STL_STRING_PROP_IMPL(getSymbol, symbol)
    STL_STRING_PROP_IMPL(getCurrency, currency)
    STL_STRING_PROP_IMPL(getExchangeCode, exchange_code)
    STL_STRING_PROP_IMPL(getShortName, short_name)
    STL_STRING_PROP_IMPL(getLongName, long_name)
};

class Quote : public quote {
    Q_GADGET
    
    Q_PROPERTY(QDateTime updatedTime READ getUpdatedTime)
    Q_PROPERTY(QString symbol READ getSymbol)
    Q_PROPERTY(int source READ getSource)
    Q_PROPERTY(QVariant dayLowPrice READ getDayLow)
    Q_PROPERTY(QVariant dayHighPrice READ getDayHigh)
    Q_PROPERTY(QVariant dayOpenPrice READ getDayOpen)
    Q_PROPERTY(double prevDayClosePrice MEMBER prev_day_close_price)
    Q_PROPERTY(double currentPrice MEMBER current_price)
    Q_PROPERTY(QVariant volume READ getVolume)
    Q_PROPERTY(double isMarketOpen MEMBER is_market_open)
    Q_PROPERTY(QVariant marketCap READ getMarketCap)

    GADGET_BOILER_PLATE_DECL(Quote, quote)

    STL_STRING_PROP_IMPL(getSymbol, symbol)
    ENUM_CLASS_PROP_IMPL(getSource, source)
    OPTIONAL_PROP_IMPL(getDayLow, day_low_price)
    OPTIONAL_PROP_IMPL(getDayHigh, day_high_price)
    OPTIONAL_PROP_IMPL(getDayOpen, day_open_price)
    OPTIONAL_PROP_IMPL(getVolume, volume)
    OPTIONAL_PROP_IMPL(getMarketCap, market_cap)

    inline QDateTime getUpdatedTime() const {
        return QDateTime::fromMSecsSinceEpoch(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                updated_time.time_since_epoch()
            ).count()
        );
    }
};

class PositionIdentifier : public position_identifier {
    Q_GADGET

    Q_PROPERTY(QString symbol READ getSymbol)
    Q_PROPERTY(int source READ getSource)

    GADGET_BOILER_PLATE_DECL(PositionIdentifier, position_identifier)

    STL_STRING_PROP_IMPL(getSymbol, symbol)
    ENUM_CLASS_PROP_IMPL(getSource, source)
};

}  // close cartera namespace

Q_DECLARE_METATYPE(cartera::FinancialInstrument)
Q_DECLARE_METATYPE(cartera::Quote)
Q_DECLARE_METATYPE(cartera::PositionIdentifier)

#undef STL_STRING_PROP_IMPL
#undef ENUM_CLASS_PROP_IMPL
#undef OPTIONAL_PROP_IMPL
#undef GADGET_BOILER_PLATE_DECL
#endif  // CARTERA_COREQT_TYPES_H

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
#include <QLocale>
#include <QObject>
#include <QVariant>

#include "types/financialinstrument.h"

#define STL_STRING_PROP_IMPL(READ_FUNC, MEM_NAME) \
    inline QString READ_FUNC() const { return QString::fromStdString( MEM_NAME ); }

#define ENUM_CLASS_PROP_IMPL(READ_FUNC, MEM_NAME) \
    inline int READ_FUNC() const { return static_cast<int>( MEM_NAME ); }

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
    Q_PROPERTY(double dayLowPrice MEMBER day_low_price)
    Q_PROPERTY(double dayHighPrice MEMBER day_high_price)
    Q_PROPERTY(double dayOpenPrice MEMBER day_open_price)
    Q_PROPERTY(double prevDayClosePrice MEMBER prev_day_close_price)
    Q_PROPERTY(double currentPrice MEMBER current_price)
    Q_PROPERTY(double volume MEMBER volume)
    Q_PROPERTY(double isMarketOpen MEMBER is_market_open)
    Q_PROPERTY(QVariant marketCap READ getMarketCap)

    GADGET_BOILER_PLATE_DECL(Quote, quote)

    STL_STRING_PROP_IMPL(getSymbol, symbol)
    ENUM_CLASS_PROP_IMPL(getSource, source)

    inline QDateTime getUpdatedTime() const {
        return QDateTime::fromMSecsSinceEpoch(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                updated_time.time_since_epoch()
            ).count()
        );
    }

    inline QVariant getMarketCap() const {
        QVariant res;
        if (market_cap.has_value()) {
            res.setValue(market_cap.value());
        }
        return res;
    }
};

//----------------------------------------------------------------------------
// Classes below are created specifically for easy UI building
//----------------------------------------------------------------------------

// This class contains all necessary data fields to be displayed for a symbol
class SymbolQuote {
    Q_GADGET

    Q_PROPERTY(QString symbol MEMBER m_symbol)
    Q_PROPERTY(QString shortName MEMBER m_shortName)
    Q_PROPERTY(QString longName MEMBER m_longName)
    Q_PROPERTY(QString currentPriceDisp READ currentPriceDisp)
    Q_PROPERTY(QString priceChangeDisp READ priceChangeDisp)
    Q_PROPERTY(QString priceChangePercentDisp READ priceChangePercentDisp)
    Q_PROPERTY(bool isPositivePriceChange READ isPositivePriceChange)

public:
    explicit SymbolQuote();
    explicit SymbolQuote(quote&& quote, financial_instrument&& fi);
    static SymbolQuote fromData(quote&& quote, financial_instrument&& fi);

    QString currentPriceDisp() const;
    QString priceChangeDisp() const;
    QString priceChangePercentDisp() const;
    inline bool isPositivePriceChange() const { return priceChange() > 0; }
    inline double priceChange() const { return m_currentPrice - m_prevDayClosePrice; }

private:
    QString m_symbol;
    QString m_longName;
    QString m_shortName;
    QString m_currency;
    double m_currentPrice;
    double m_prevDayClosePrice;  // old price
    int m_priceDecimalHint;  // how many decimal places

    QLocale m_locale;  // current system locale
};

}  // close cartera namespace

Q_DECLARE_METATYPE(cartera::FinancialInstrument)
Q_DECLARE_METATYPE(cartera::Quote)
Q_DECLARE_METATYPE(cartera::SymbolQuote)

#undef STL_STRING_PROP_IMPL
#undef ENUM_CLASS_PROP_IMPL
#undef GADGET_BOILER_PLATE_DECL
#endif  // CARTERA_COREQT_TYPES_H

/*
 * <one line to give the library's name and an idea of what it does.>
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

#ifndef CARTERA_FINANCIALINSTRUMENT_H
#define CARTERA_FINANCIALINSTRUMENT_H

#include <QtCore>

namespace cartera {

    
enum class AssetClass
{
    Equity,
    ETF,
    Index,
    MutualFund,
    Cash,  // cash or cash equivalent
    Future,
    Other,  // This list should be expanded
};

/**
 * Defines the structure of financial instruments data
 */
struct FinancialInstrument
{
    AssetClass type;
    
    // TODO: Yahoo Finance doesn't give this data.
    // QString isin;  //  ISIN identifies a security (which might be traded at different exchanges under different symbols)
    
    QString symbol;  // symbol is unique at exchange-level
    QString exchangeCode;  // shortened exchange code (e.g. LSE, FRA)
    QString longName;
    QString shortName;
};

/**
 * Defines the real-time quote data
 */
struct Quote
{
    QDateTime updatedTime;
    QString symbol;
    QString currency;  // ISO4217 currency code
    double dayLowPrice;
    double dayHighPrice;
    double dayOpenPrice;
    double prevDayClosePrice;
    double currentPrice;
    double volume;
    double isMarketOpen;
    QVariant marketCap;  // nullable double
};


}

#endif // CARTERA_FINANCIALINSTRUMENT_H

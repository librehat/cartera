/*
 * A class contains all necessary data fields for a watch list
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
#ifndef CARTERA_COREQT_SYMBOLQUOTE_H
#define CARTERA_COREQT_SYMBOLQUOTE_H

#include <QLocale>
#include <QObject>

#include "types/financialinstrument.h"
#include "types/positionidentifier.h"

namespace cartera {

class SymbolQuote {
    Q_GADGET

    Q_PROPERTY(QString symbol MEMBER m_symbol)
    Q_PROPERTY(int source MEMBER m_source)
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
    int m_source;  // must be a valid feed_source enum value
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

Q_DECLARE_METATYPE(cartera::SymbolQuote)

#endif  // CARTERA_COREQT_SYMBOLQUOTE_H

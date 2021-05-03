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

#ifndef POSITION_H
#define POSITION_H

#include <qobject.h>
#include "feed/feedrepository.h"

namespace cartera {

struct PositionIdentifier {
    FeedIdentifier feedIdentifier;
    QString symbol;
};
bool operator<(const PositionIdentifier& a, const PositionIdentifier& b);


class Position : public QObject
{
    Q_OBJECT

public:
    explicit Position(const FeedIdentifier& feed, const QString& symbol, QObject *parent = nullptr);
    explicit Position(const FeedIdentifier& feed, const QString& symbol, qreal quantity, qreal avgPrice, QObject *parent = nullptr);
    explicit Position(const PositionIdentifier& ident, qreal quantity, qreal avgPrice, QObject *parent = nullptr);
    
    explicit Position(const Position&);
    
    virtual ~Position() = default;
    
    // GETTERS
    const FeedIdentifier& feedIdentifier() const;
    const QString& symbol() const;
    qreal quantity() const;
    qreal averagePrice() const;
    bool isShort() const;

    // SETTERS
    void setQuantity(qreal quantity);
    void setAveragePrice(qreal avgPrice);
    
    Position& operator=(const Position&);

private:
    // Feed identifier to find the appropriate feed to get data for the symbol
    const FeedIdentifier m_feedId;
    
    // Feed-specific symbol
    const QString m_symbol;
    
    /**
     * The number of units (identified by the symbol)
     * Positive for buy, negative for sell (short)
     */
    qreal m_quantity;
    
    /**
     * The average price per unit in terms of total costs, in local (feed-specified) currency
     */
    qreal m_avgPrice;
};

}  // close namespace cartera

#endif // POSITION_H

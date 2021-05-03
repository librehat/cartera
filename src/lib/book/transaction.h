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

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <qobject.h>

#include "position.h"

namespace cartera {
    
/**
 * Transaction represents a fulfilled "order"
 */
class Transaction : public QObject
{
    Q_OBJECT

public:
    explicit Transaction(const PositionIdentifier& posId, QObject *parent = nullptr);
    
    virtual ~Transaction() = default;
    
    // SETTERS
    void setQuantity(qreal quantity);
    void setPrice(qreal price);
    void setFees(qreal fees);  // We sum up all kinds of fees for now
    
    // GETTERS
    const PositionIdentifier& positionIdentifier() const;
    qreal quantity() const;
    qreal price() const;
    qreal fees() const;
    
    /**
     * The average price of this transaction, taking all fees into consideration
     */
    qreal averagePrice() const;
    
private:
    const PositionIdentifier m_positionIdentifier;
    
    qreal m_quantity;
    qreal m_price;
    qreal m_fees;
};


}  // close namespace cartera

#endif // TRANSACTION_H

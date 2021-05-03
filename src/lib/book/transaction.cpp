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

#include "transaction.h"


namespace cartera {

Transaction::Transaction(const PositionIdentifier& posId, QObject *parent)
: QObject(parent)
, m_positionIdentifier(posId)
, m_quantity(0)
, m_price(0)
, m_fees(0)
{
}

void Transaction::setQuantity(qreal quantity)
{
    m_quantity = quantity;
}

void Transaction::setPrice(qreal price)
{
    m_price = price;
}

void Transaction::setFees(qreal fees)
{
    m_fees = fees;
}

const PositionIdentifier& Transaction::positionIdentifier() const
{
    return m_positionIdentifier;
}

qreal Transaction::quantity() const
{
    return m_quantity;
}

qreal Transaction::price() const
{
    return m_price;
}

qreal Transaction::fees() const
{
    return m_fees;
}

qreal Transaction::averagePrice() const
{
    if (m_quantity == 0) {
        return 0;
    }
    return (m_price * m_quantity + (m_quantity < 0 ? -1 : 1) * m_fees) / m_quantity;
}

}  // close namespace cartera

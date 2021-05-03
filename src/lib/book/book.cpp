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

#include "book.h"

namespace cartera {

Book::Book(QObject *parent)
: QObject(parent)
{}

void Book::updatePosition(const Transaction& trn)
{
    auto it = m_positions.find(trn.positionIdentifier());
    if (it == m_positions.end()) {
        m_positions.insert(it, trn.positionIdentifier(), Position(trn.positionIdentifier(), trn.quantity(), trn.averagePrice()));
        return;
    }
    // otherwise, calculate the new average price for the existing position
    const qreal value = (it->quantity() * it->averagePrice()) + (trn.averagePrice() * trn.quantity());
    it->setQuantity(it->quantity() + trn.quantity());
    if (it->quantity() == 0) {
        // no more open position, removing it from the book
        m_positions.erase(it);
    } else {
        it->setAveragePrice(value / it->quantity());
    }
}

    
}  // close namespace cartera

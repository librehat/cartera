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

#include "position.h"


namespace cartera {
    
bool operator<(const PositionIdentifier& a, const PositionIdentifier& b)
{
    if (a.feedIdentifier < b.feedIdentifier) {
        return true;
    }
    if (a.feedIdentifier > b.feedIdentifier) {
        return false;
    }
    return a.symbol < b.symbol;
}


Position::Position(const FeedIdentifier& feed, const QString& symbol, QObject *parent)
: Position(feed, symbol, 0, 0, parent)
{}

Position::Position(const FeedIdentifier& feed, const QString& symbol, qreal quantity, qreal avgPrice, QObject *parent)
: QObject(parent)
, m_feedId(feed)
, m_symbol(symbol)
, m_quantity(quantity)
, m_avgPrice(avgPrice)
{
}

Position::Position(const PositionIdentifier& ident, qreal quantity, qreal avgPrice, QObject *parent)
: Position(ident.feedIdentifier, ident.symbol, quantity, avgPrice, parent)
{}

Position::Position(const Position& other)
: Position(other.m_feedId, other.m_symbol, other.m_quantity, other.m_avgPrice, other.parent())
{}

Position& Position::operator=(const Position& other)
{
    const_cast<FeedIdentifier&>(m_feedId) = other.m_feedId;
    const_cast<QString&>(m_symbol) = other.m_symbol;
    m_quantity = other.m_quantity;
    m_avgPrice = other.m_avgPrice;
    return *this;
}

const FeedIdentifier& Position::feedIdentifier() const
{
    return m_feedId;
}

const QString& Position::symbol() const
{
    return m_symbol;
}

qreal Position::quantity() const
{
    return m_quantity;
}

qreal Position::averagePrice() const
{
    return m_avgPrice;
}

bool Position::isShort() const
{
    return m_quantity < 0;
}

void Position::setQuantity(qreal quantity)
{
    m_quantity = quantity;
}

void Position::setAveragePrice(qreal averagePrice)
{
    m_avgPrice = averagePrice;
}

}  // close namespace cartera

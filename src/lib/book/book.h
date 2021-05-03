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

#ifndef BOOK_H
#define BOOK_H

#include <qmap.h>
#include <qobject.h>

#include "position.h"
#include "transaction.h"

namespace cartera {
    
/**
 * Book is a logical group of multiple positions
 * 
 * This can be used to represent an "account" for example
 */
class Book : public QObject
{
    Q_OBJECT

public:
    explicit Book(QObject *parent = nullptr);

    virtual ~Book() = default;
    
    /**
     * Updates the position in this book by a transaction
     */
    void updatePosition(const Transaction& trn);
    
private:
    // TODO: use QHash
    QMap<PositionIdentifier, Position> m_positions;
};

}  // close namespace cartera

#endif // BOOK_H

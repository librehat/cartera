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

#ifndef CARTERA_ABSTRACTFEED_H
#define CARTERA_ABSTRACTFEED_H

#include <QtCore>

#include "financialinstrument.h"
#include "feedreplyprivate.h"

namespace cartera {

/**
 * Defines the protocol to retrieve financial instruments data
 */
class AbstractFeed
{
public:
    virtual ~AbstractFeed() = default;
    
    /**
     * Resolves the financial instrument identified by the symbol.
     */
    virtual FeedReply<FinancialInstrument> resolveSymbol(const QString& symbol) = 0;
    
    /**
     * Resolves the quote for the financial instrument identified by the symbol.
     */
    virtual FeedReply<Quote> resolveQuote(const QString& symbol) = 0;
    
    /**
     * Searches possible financial instruments by keywords.
     */
    virtual FeedReply<QList<FinancialInstrument>> searchSymbols(const QString& keyword) = 0;
};

}

#endif // CARTERA_ABSTRACTFEED_H

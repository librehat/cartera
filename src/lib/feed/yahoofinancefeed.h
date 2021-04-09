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

#ifndef CARTERA_YAHOOFINANCEFEED_H
#define CARTERA_YAHOOFINANCEFEED_H

#include <qnetworkaccessmanager.h>

#include "abstractfeed.h"

namespace cartera {

/**
 * This class implements the feed protocol using Yahoo Finance as the source
 */
class YahooFinanceFeed : public AbstractFeed
{
public:
    explicit YahooFinanceFeed();
    
    /**
     * Constructs a 'YahooFinanceFeed' instance with the specified access manager.
     * 'YahooFinanceFeed' will take over the ownership of the access manager.
     * 
     * This constructor allows the usage of 'KIO::AccessManager' on desktop Linux.
     */
    explicit YahooFinanceFeed(QNetworkAccessManager *netman);
    
    virtual ~YahooFinanceFeed();
    
    virtual FeedReply<FinancialInstrument> resolveSymbol(const QString& symbol) override;
    virtual FeedReply<Quote> resolveQuote(const QString& symbol) override;
    virtual FeedReply<QList<FinancialInstrument>> searchSymbols(const QString& keyword) override;
    
protected:
    QNetworkAccessManager *m_netman;
};

}

#endif // CARTERA_YAHOOFINANCEFEED_H

/*
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

#include "yahoofinancefeed.h"

namespace cartera {

YahooFinanceFeed::YahooFinanceFeed()
{    
}

YahooFinanceFeed::~YahooFinanceFeed()
{
}

/**
 * example:
 *  https://query1.finance.yahoo.com/v10/finance/quoteSummary/IBM?modules=price
 */
FeedReply<FinancialInstrument> YahooFinanceFeed::resolveSymbol(const std::string& symbol)
{
    /*QNetworkRequest req(QUrl("https://query1.finance.yahoo.com/v10/finance/quoteSummary/" + symbol + "?modules=price"));
    QNetworkReply *reply = m_netman->get(req);
    return FeedReply<FinancialInstrument>(reply, s_parseSymbolFunc);*/
}

FeedReply<Quote> YahooFinanceFeed::resolveQuote(const std::string& symbol)
{
    /*QNetworkRequest req(QUrl("https://query1.finance.yahoo.com/v10/finance/quoteSummary/" + symbol + "?modules=price"));
    QNetworkReply *reply = m_netman->get(req);
    return FeedReply<Quote>(reply, s_parseQuoteFunc);*/
}

FeedReply<std::vector<FinancialInstrument>> YahooFinanceFeed::searchSymbols(const std::string& keyword)
{
    /*QNetworkRequest req(QUrl("https://query2.finance.yahoo.com/v1/finance/search?q=" + keyword + "&quotesCount=6&newsCount=0"));
    QNetworkReply *reply = m_netman->get(req);
    return FeedReply<QList<FinancialInstrument>>(reply, s_parseSearchFunc);*/
}

}  // close cartera namespace

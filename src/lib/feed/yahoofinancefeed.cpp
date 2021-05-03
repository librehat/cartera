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

#include "yahoofinancefeed.h"

#include <qdatetime.h>
#include <qnetworkrequest.h>
#include <qjsondocument.h>

namespace cartera {

namespace {
    
AssetClass assetClassFromQuoteType(const QString& quoteType)
{
    if (quoteType == "EQUITY") {
        return AssetClass::Equity;
    }
    if (quoteType == "ETF") {
        return AssetClass::ETF;
    }
    if (quoteType == "INDEX") {
        return AssetClass::Index;
    }
    // TODO: complete the mapping
    return AssetClass::Other;
}
    
FinancialInstrument s_parseSymbolFunc(const QByteArray& data)
{
    const QJsonDocument document = QJsonDocument::fromJson(data);
    const QJsonValue quoteSummary = document["quoteSummary"];
    if (quoteSummary.isUndefined()) {
        throw FeedException("quoteSummary is undefined");
    }
    const QJsonValue error = quoteSummary["error"];
    if (!error.isUndefined() && !error.isNull()) {
        throw FeedException(error.toString());
    }
    const QJsonValue result = quoteSummary["result"];
    if (result.isUndefined() || !result.isArray()) {
        throw FeedException("quoteSummary result is unparse-able");
    }
    const QJsonValue quoteResult = result.toArray().first()["price"];
    return FinancialInstrument{
        assetClassFromQuoteType(quoteResult["quoteType"].toString()),
        quoteResult["symbol"].toString(),
        quoteResult["exchange"].toString(),
        quoteResult["longName"].toString(),
        quoteResult["shortname"].toString(),
    };
}

Quote s_parseQuoteFunc(const QByteArray& data)
{
    const QJsonDocument document = QJsonDocument::fromJson(data);
    const QJsonValue quoteSummary = document["quoteSummary"];
    if (quoteSummary.isUndefined()) {
        throw FeedException("quoteSummary is undefined");
    }
    const QJsonValue error = quoteSummary["error"];
    if (!error.isUndefined() && !error.isNull()) {
        throw FeedException(error.toString());
    }
    const QJsonValue result = quoteSummary["result"];
    if (result.isUndefined() || !result.isArray()) {
        throw FeedException("quoteSummary result is unparse-able");
    }
    
    const QJsonValue quoteResult = result.toArray().first()["price"];
    const QJsonValue marketCap = quoteResult["marketCap"];
    
    return Quote{
        QDateTime::fromSecsSinceEpoch(quoteResult["regularMarketTime"].toInt()),
        quoteResult["symbol"].toString(),
        quoteResult["currency"].toString(),
        quoteResult["regularMarketDayLow"]["raw"].toDouble(),
        quoteResult["regularMarketDayHigh"]["raw"].toDouble(),
        quoteResult["regularMarketOpen"]["raw"].toDouble(),
        quoteResult["regularMarketPreviousClose"]["raw"].toDouble(),
        quoteResult["regularMarketPrice"]["raw"].toDouble(),
        quoteResult["regularMarketVolume"]["raw"].toDouble(),
        true, // FIXME
        (marketCap.isObject() && marketCap["raw"].isDouble()) ? marketCap["raw"].toDouble() : QVariant(),
    };
}

QList<FinancialInstrument> s_parseSearchFunc(const QByteArray& data)
{
    QList<FinancialInstrument> results;
    
    const QJsonDocument document = QJsonDocument::fromJson(data);
    const QJsonValue quotes = document["quotes"];
    if (quotes.isUndefined() || !quotes.isArray()) {
        throw FeedException("Search result is unparse-able");
    }
    const QJsonArray quotesArray = quotes.toArray();
    for (auto qIt = quotesArray.begin(); qIt != quotesArray.end(); ++qIt) {
        const QJsonValue val = *qIt;
        results.append(
            FinancialInstrument{
                assetClassFromQuoteType(val["quoteType"].toString()),
                val["symbol"].toString(),
                val["exchange"].toString(),
                val["longname"].toString(),
                val["shortname"].toString(),
            }
        );
    }
    
    return results;
}

}  // close unnamed namespace


YahooFinanceFeed::YahooFinanceFeed()
: YahooFinanceFeed(new QNetworkAccessManager())
{    
}

YahooFinanceFeed::YahooFinanceFeed(QNetworkAccessManager *netman)
: m_netman(netman)
{
}

YahooFinanceFeed::~YahooFinanceFeed()
{
    m_netman->deleteLater();
}

/**
 * example:
 *  https://query1.finance.yahoo.com/v10/finance/quoteSummary/IBM?modules=price
 */
FeedReply<FinancialInstrument> YahooFinanceFeed::resolveSymbol(const QString& symbol)
{
    QNetworkRequest req(QUrl("https://query1.finance.yahoo.com/v10/finance/quoteSummary/" + symbol + "?modules=price"));
    QNetworkReply *reply = m_netman->get(req);
    return FeedReply<FinancialInstrument>(reply, s_parseSymbolFunc);
}

FeedReply<Quote> YahooFinanceFeed::resolveQuote(const QString& symbol)
{
    QNetworkRequest req(QUrl("https://query1.finance.yahoo.com/v10/finance/quoteSummary/" + symbol + "?modules=price"));
    QNetworkReply *reply = m_netman->get(req);
    return FeedReply<Quote>(reply, s_parseQuoteFunc);
}

FeedReply<QList<FinancialInstrument>> YahooFinanceFeed::searchSymbols(const QString& keyword)
{
    QNetworkRequest req(QUrl("https://query2.finance.yahoo.com/v1/finance/search?q=" + keyword + "&quotesCount=6&newsCount=0"));
    QNetworkReply *reply = m_netman->get(req);
    return FeedReply<QList<FinancialInstrument>>(reply, s_parseSearchFunc);
}

}  // close cartera namespace

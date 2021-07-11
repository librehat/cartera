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
#include "backend.h"

#include "types.h"

#include <algorithm>
#include <boost/range/combine.hpp>

#include <QtConcurrent>
#include <QFutureWatcher>
#include <QJSEngine>
#include <QtDebug>

#define CREATE_WATCHER(RTYPE, CALLBACK, ERROR_CALLBACK) \
    auto* watcher = new QFutureWatcher< RTYPE >(this->parent()); \
    connect(watcher, &QFutureWatcher< RTYPE >::finished, [this, watcher, CALLBACK, ERROR_CALLBACK]() { \
        try { \
            QJSValue cb( CALLBACK ); \
            cb.call(QJSValueList{ m_jsEngine->toScriptValue(watcher->result()) }); \
        } \
        catch (const std::exception& e) { \
            QJSValue eCb( ERROR_CALLBACK ); \
            eCb.call(QJSValueList{ m_jsEngine->toScriptValue(QString(e.what())) }); \
        } \
        watcher->deleteLater(); \
        });


namespace cartera {

Backend::Backend(QJSEngine* engine, QObject* parent)
    : QObject(parent)
    , m_jsEngine(engine)
{}

void Backend::searchSymbols(const QString& keyword, const QJSValue& callback, const QJSValue& errorCb) const
{
    using ResultType = QVector<SymbolSearchResult>;

    CREATE_WATCHER(ResultType, callback, errorCb)
    watcher->setFuture(
        // capture by value, the lifespan of &keyword is shorter than the lambda
        QtConcurrent::run([keyword, this]() -> ResultType {
            auto res = m_feedApi.search_symbols(keyword.toStdString());
            ResultType out;
            out.reserve(res.size());
            for (auto&& item : res) {
                out.push_back(std::move(item));
            }
            return out;
        })
    );
}

void Backend::getQuote(const QString& symbol, int source, const QJSValue& callback, const QJSValue& errorCb) const
{
    CREATE_WATCHER(Quote, callback, errorCb)
    watcher->setFuture(
        QtConcurrent::run([symbol, source, this]() -> Quote {
            return m_feedApi.get_quote(symbol.toStdString(), static_cast<feed_source>(source));
        })
    );
}

void Backend::getSymbolQuotes(const QStringList& symbols, const QList<int>& sources, const QJSValue& callback, const QJSValue& errorCb)
{
    if (sources.length() != symbols.length()) {
        QJSValue eCb(errorCb);
        eCb.call(QJSValueList{ QString("The length of symbols mismatches the length of sources")});
    }

    using ResultType = QVector<SymbolQuote>;

    CREATE_WATCHER(ResultType, callback, errorCb)
    watcher->setFuture(
        QtConcurrent::run([symbols, sources, this]() -> ResultType {
            std::vector<std::string> symbols_strs;  // we need to keep the underlying string for string_view
            std::vector<std::pair<std::string_view, feed_source>> inputs;
            std::vector<financial_instrument> instruments_data;
            symbols_strs.reserve(symbols.length());
            inputs.reserve(symbols.length());
            instruments_data.reserve(symbols.length());
            for (int i = 0; i < symbols.length(); ++i) {
                symbols_strs.push_back(symbols[i].toStdString());
                inputs.emplace_back(symbols_strs[i], static_cast<feed_source>(sources[i]));
                instruments_data.push_back(m_feedApi.get_financial_instrument(symbols_strs[i], static_cast<feed_source>(sources[i])));
            }
            auto quotes = m_feedApi.get_quotes(inputs);

            ResultType out;
            out.reserve(quotes.size());
            for (auto&& [quote, fi] : boost::combine(quotes, instruments_data)) {
                out.push_back(SymbolQuote::fromData(std::move(quote), std::move(fi)));
            }
            return out;
        })
    );
}

void Backend::getSymbolQuotesForWatchList(const QString& listName, const QJSValue& callback, const QJSValue& errorCb)
{
    // TODO
}

void Backend::saveWatchList(const QString& listName, const QStringList& symbols, const QList<int>& sources, const QJSValue& errorCb)
{
    // TODO
}

}  // close cartera namespace

#undef CREATE_WATCHER

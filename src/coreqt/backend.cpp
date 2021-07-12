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
#include <QStandardPaths>
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

namespace {
QVector<SymbolQuote> doFetchSymbolQuotes(const std::vector<position_identifier>& pis, feed::api& api) {
    std::vector<financial_instrument> instruments_data;
    instruments_data.reserve(pis.size());
    for (const auto& pi : pis) {
        instruments_data.push_back(api.get_financial_instrument(pi.symbol, pi.source));
    }
    auto quotes = api.get_quotes(pis);

    QVector<SymbolQuote> out;
    out.reserve(quotes.size());
    for (auto&& [quote, fi] : boost::combine(quotes, instruments_data)) {
        out.push_back(SymbolQuote::fromData(std::move(quote), std::move(fi)));
    }
    return out;
}

}  // close unnamed namespace

Backend::Backend(QJSEngine* engine, QObject* parent)
    : QObject(parent)
    , m_config(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation).toStdString())
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
            std::vector<position_identifier> inputs;
            inputs.reserve(symbols.length());
            for (int i = 0; i < symbols.length(); ++i) {
                inputs.emplace_back(
                    position_identifier{ static_cast<feed_source>(sources[i]), symbols[i].toStdString() }
                );
            }
            return doFetchSymbolQuotes(inputs, m_feedApi);
        })
    );
}
    
void Backend::getAllWatchListNames(const QJSValue& callback, const QJSValue& errorCb)
{
    CREATE_WATCHER(QStringList, callback, errorCb)
    watcher->setFuture(
        QtConcurrent::run([this]() -> QStringList {
            const auto allLists = m_config.list_watch_lists();
            QStringList out;
            out.reserve(allLists.size());
            for (const auto& listName : allLists) {
                out.push_back(QString::fromStdString(listName));
            }
            return out;
        })
    );
}

void Backend::getSymbolQuotesForWatchList(const QString& listName, const QJSValue& callback, const QJSValue& errorCb)
{
    using ResultType = QVector<SymbolQuote>;
    CREATE_WATCHER(ResultType, callback, errorCb)
    watcher->setFuture(
        QtConcurrent::run([listName, this]() -> ResultType {
            const auto listItems = m_config.read_watch_list_symbols(listName.toStdString());
            return doFetchSymbolQuotes(listItems, m_feedApi);
        })
    );
}

void Backend::saveWatchList(const QString& listName, const QStringList& symbols, const QList<int>& sources, const QJSValue& errorCb)
{
    if (sources.length() != symbols.length()) {
        QJSValue eCb(errorCb);
        eCb.call(QJSValueList{ QString("The length of symbols mismatches the length of sources")});
    }

    std::vector<config::watch_list_item> listItems;
    listItems.reserve(symbols.size());
    for (const auto& [symbol, source] : boost::combine(symbols, sources)) {
        listItems.emplace_back(
            config::watch_list_item{
                static_cast<feed_source>(source),
                symbol.toStdString()
            }
        );
    }
    try {
        m_config.save_watch_list_symbols(listName.toStdString(), listItems);
    }
    catch (const std::exception& e) {
        QJSValue eCb(errorCb);
        eCb.call(QJSValueList{ m_jsEngine->toScriptValue(QString(e.what())) });
    }
}

}  // close cartera namespace

#undef CREATE_WATCHER

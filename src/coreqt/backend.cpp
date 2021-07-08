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
#include "feed/basicfeed.h"

#include <algorithm>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QJSEngine>
#include <QtDebug>

namespace cartera {

Backend::Backend(QJSEngine* engine, QObject* parent)
    : QObject(parent)
    , m_jsEngine(engine)
{}

void Backend::searchSymbols(const QString& keyword, const QJSValue& callback) const
{
    using ResultType = QVector<SymbolSearchResult>;

    auto* watcher = new QFutureWatcher<ResultType>(this->parent());
    connect(watcher, &QFutureWatcher<ResultType>::finished, [this, watcher, callback]() {
        QJSValue cb(callback);
        cb.call(QJSValueList{ m_jsEngine->toScriptValue(watcher->result()) });
        watcher->deleteLater();
        });
    watcher->setFuture(
        // capture by value, the lifespan of &keyword is shorter than the lambda
        QtConcurrent::run([keyword, this]() -> ResultType {
            auto res = basic_feed::search_symbols<feed_source::YahooFinance>(m_httpClient, keyword.toStdString());
            ResultType out;
            for (auto&& item : res) {
                out.push_back(std::move(item));
            }
            return out;
        })
    );
}

void Backend::getQuote(const QString& symbol, const QJSValue& callback) const
{
    auto* watcher = new QFutureWatcher<Quote>(this->parent());
    connect(watcher, &QFutureWatcher<Quote>::finished, [this, watcher, callback]() {
        QJSValue cb(callback);
        cb.call(QJSValueList{ m_jsEngine->toScriptValue(watcher->result()) });
        watcher->deleteLater();
        });
    watcher->setFuture(
        QtConcurrent::run([symbol, this]() {
            return Quote{ basic_feed::resolve_quote<feed_source::YahooFinance>(m_httpClient, symbol.toStdString()) };
        })
    );
}

}  // close cartera namespace

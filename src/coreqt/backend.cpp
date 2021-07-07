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

Backend::Backend(QObject* parent)
    : QObject(parent)
{}

void Backend::hello() const
{
    qDebug() << "Hello from backend";
}

void Backend::searchSymbols(const QString& keyword, const QJSValue& callback) const
{
    /* FIXME
    using ResultType = QVector<FinancialInstrument>;

    auto* watcher = new QFutureWatcher<ResultType>(this->parent());
    connect(watcher, &QFutureWatcher<ResultType>::finished, [this, watcher, callback]() {
        QJSValue cb(callback);
        // FIXME: QJSValue::engine() is deprecated but there is no replacement
        // Using a newly instantiated QJSEngine would trigger an exception when invoking the callback
        cb.call(QJSValueList{ callback.engine()->toScriptValue(watcher->result()) });
        watcher->deleteLater();
        });
    watcher->setFuture(
        // capture by value, the lifespan of &keyword is shorter than the lambda
        QtConcurrent::run([keyword, this]() {
            auto res = basic_feed<feed_source::YahooFinance>::search_symbols(m_http_client, keyword.toStdString());
            return ResultType{ res.begin(), res.end() };
            })
    );
    */
}

void Backend::getQuote(const QString& symbol, const QJSValue& callback) const
{
    auto* watcher = new QFutureWatcher<Quote>(this->parent());
    connect(watcher, &QFutureWatcher<Quote>::finished, [this, watcher, callback]() {
        QJSValue cb(callback);
        // FIXME: QJSValue::engine() is deprecated but there is no replacement
        // Using a newly instantiated QJSEngine would trigger an exception when invoking the callback
        cb.call(QJSValueList{ callback.engine()->toScriptValue(watcher->result()) });
        watcher->deleteLater();
        });
    watcher->setFuture(
        QtConcurrent::run([symbol, this]() {
            return Quote{ basic_feed::resolve_quote<feed_source::YahooFinance>(m_http_client, symbol.toStdString()) };
            })
    );
}

}  // close cartera namespace

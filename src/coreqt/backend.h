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
#ifndef CARTERA_COREQT_BACKEND_H
#define CARTERA_COREQT_BACKEND_H

#include <QObject>
#include <QJSValue>

#include "feed/api.h"

namespace cartera {

/*
 * The bridge between the QML code and the core.
 * This class is designed to run as a singleton and handles all requests
 * from the GUI.
 */
class Backend : public QObject {
Q_OBJECT

public:
    explicit Backend (QJSEngine* engine, QObject* parent = nullptr);

    // The 'callback' must take a single parameter of type QVector<SearchSymbolResult>
    // The 'errorCb' must take a single parameter of QString that is the error message
    Q_INVOKABLE void searchSymbols(const QString& keyword, const QJSValue& callback, const QJSValue& errorCb) const;

    // The 'callback' must take a single parameter of type Quote
    // The 'errorCb' must take a single parameter of QString that is the error message
    Q_INVOKABLE void getQuote(const QString& symbol, int source, const QJSValue& callback, const QJSValue& errorCb) const;

    // Get multiples quotes. Similar to 'getQuote' but works in bulk
    // This allows the usage of more efficient APIs to get the quotes for multiple symbols
    Q_INVOKABLE void getQuotes(const QStringList& symbols, const QList<int>& sources, const QJSValue& callback, const QJSValue& errorCb) const;

private:
    feed::api m_feedApi;
    QJSEngine* m_jsEngine;
};

}  // close cartera namespace

#endif  // CARTERA_COREQT_BACKEND_H

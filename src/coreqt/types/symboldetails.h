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
#ifndef CARTERA_COREQT_SYMBOLDETAILS_H
#define CARTERA_COREQT_SYMBOLDETAILS_H

#include <QDateTime>
#include <QObject>
#include <QLocale>
#include <QVariant>

#include "types/financialinstrument.h"

namespace cartera {
// This class contains all necessary data fields to be displayed on the details page for a symbol
class SymbolDetails {
    Q_GADGET

    // details are a list of QVariantMap that have 'title' and 'display' properties
    Q_PROPERTY(QVariantList details MEMBER m_details)
    // TODO: add historical price data for charts

public:
    explicit SymbolDetails();
    explicit SymbolDetails(financial_instrument&& basic, symbol_detail&& detail);

private:
    QVariantList m_details;
    QLocale m_locale;
};

}  // close cartera namespace

Q_DECLARE_METATYPE(cartera::SymbolDetails)

#endif  // CARTERA_COREQT_SYMBOLDETAILS_H

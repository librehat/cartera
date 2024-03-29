/*
 * Promisify the APIs from the C++ backend instance.
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

.pragma library
.import com.librehat.cartera 1.0 as Cartera


function searchSymbols(keyword) {
    return new Promise((resolve, reject) => {
        Cartera.Backend.searchSymbols(keyword, resolve, (errorMsg) => {
            reject(new Error(errorMsg));
        });
    });
}

function getSymbolQuotes(symbols, sources) {
    return new Promise((resolve, reject) => {
        Cartera.Backend.getSymbolQuotes(symbols, sources, resolve, (errorMsg) => {
            reject(new Error(errorMsg));
        });
    });
}

function getSymbolQuotesForList(listName) {
    return new Promise((resolve, reject) => {
        Cartera.Backend.getSymbolQuotesForWatchList(listName, resolve, (errorMsg) => {
            reject(new Error(errorMsg));
        });
    });
}

function getSymbolDetails(symbol, source) {
    return new Promise((resolve, reject) => {
        Cartera.Backend.getSymbolDetails(symbol, source, resolve, (errorMsg) => {
            reject(new Error(errorMsg));
        });
    });
}

function getAllWatchListNames() {
    return new Promise((resolve, reject) => {
        Cartera.Backend.getAllWatchListNames(resolve, (errorMsg) => {
            reject(new Error(errorMsg));
        });
    });
}

function saveWatchList(listName, symbols, sources) {
    return new Promise((resolve, reject) => {
        Cartera.Backend.saveWatchList(listName, symbols, sources, (errorMsg) => {
            reject(new Error(errorMsg));
        });
        resolve();
    });
}

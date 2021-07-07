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

#define BOOST_TEST_MODULE jsonparsertest
#include <boost/test/included/unit_test.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <string>

#include "parser/jsonparser.h"
#include "types/financialinstrument.h"

namespace fs = boost::filesystem;
using yahoo_parser = cartera::json_parser<cartera::feed_source::YahooFinance>;
using binance_parser = cartera::json_parser<cartera::feed_source::Binance>;

static std::string read_file(fs::path path)
{
    fs::ifstream f(path, std::ios::in | std::ios::binary);
    const auto sz = fs::file_size(path);
    std::string result(sz, '\0');
    f.read(result.data(), sz);
    return result;
}

BOOST_AUTO_TEST_CASE(parse_financial_instrument_empty)
{
    BOOST_CHECK_THROW(yahoo_parser::parse_financial_instrument(""), boost::exception);
}

BOOST_AUTO_TEST_CASE(parse_financial_instrument_IBM)
{
    const std::string json_filepath{ "./fixture/IBM.json" };
    const auto result = yahoo_parser::parse_financial_instrument(read_file(json_filepath));
    
    BOOST_CHECK_EQUAL(result.symbol, "IBM");
    BOOST_CHECK_EQUAL(result.currency, "USD");
    BOOST_CHECK_EQUAL(result.exchange_code, "NYQ");
    BOOST_CHECK_EQUAL(result.type, cartera::asset_class::Equity);
    BOOST_CHECK_EQUAL(result.long_name, "International Business Machines Corporation");
    BOOST_CHECK_EQUAL(result.short_name, "International Business Machines");
}

BOOST_AUTO_TEST_CASE(parse_financial_instrument_ETH_USD)
{
    const std::string json_filepath{ "./fixture/ETH-USD.json" };
    const auto result = yahoo_parser::parse_financial_instrument(read_file(json_filepath));
    
    BOOST_CHECK_EQUAL(result.symbol, "ETH-USD");
    BOOST_CHECK_EQUAL(result.currency, "USD");
    BOOST_CHECK_EQUAL(result.exchange_code, "CCC");
    BOOST_CHECK_EQUAL(result.type, cartera::asset_class::CryptoCurrency);
    BOOST_CHECK_EQUAL(result.long_name, "Ethereum USD");
    BOOST_CHECK_EQUAL(result.short_name, "Ethereum USD");
}

BOOST_AUTO_TEST_CASE(parse_quote_IBM)
{
    const std::string json_filepath{ "./fixture/IBM.json" };
    const auto result = yahoo_parser::parse_quote(read_file(json_filepath));

    BOOST_CHECK_EQUAL(result.symbol, "IBM");
    BOOST_CHECK_EQUAL(result.current_price, 136.38);
    BOOST_CHECK_EQUAL(result.day_high_price, 136.48);
    BOOST_CHECK_EQUAL(result.day_low_price, 133.12);
    BOOST_CHECK_EQUAL(result.day_open_price, 133.29);
    BOOST_CHECK(result.is_market_open);
    BOOST_CHECK(result.market_cap.has_value());
    if (result.market_cap.has_value()) {
        BOOST_CHECK_EQUAL(result.market_cap.value(), double(121868353536));
    }
    BOOST_CHECK_EQUAL(result.prev_day_close_price, 133.07);
    BOOST_CHECK_EQUAL(result.volume, 5567592);
    BOOST_CHECK_EQUAL(result.updated_time.time_since_epoch().count(), 1616788802);
}

BOOST_AUTO_TEST_CASE(parse_quote_ETH_USD)
{
    const std::string json_filepath{ "./fixture/ETH-USD.json" };
    const auto result = yahoo_parser::parse_quote(read_file(json_filepath));

    BOOST_CHECK_EQUAL(result.symbol, "ETH-USD");
    BOOST_CHECK_EQUAL(result.current_price, 2358.923);
    BOOST_CHECK_EQUAL(result.day_high_price, 2396.8665);
    BOOST_CHECK_EQUAL(result.day_low_price, 2298.4412);
    BOOST_CHECK_EQUAL(result.day_open_price, 2317.4314);
    BOOST_CHECK(result.is_market_open);
    BOOST_CHECK(result.market_cap.has_value());
    if (result.market_cap.has_value()) {
        BOOST_CHECK_EQUAL(result.market_cap.value(), double(275044761600));
    }
    BOOST_CHECK_EQUAL(result.prev_day_close_price, 2317.4314);
    BOOST_CHECK_EQUAL(result.volume, 23386800128);
    BOOST_CHECK_EQUAL(result.updated_time.time_since_epoch().count(), 1625680262);
}

BOOST_AUTO_TEST_CASE(parse_financial_instrument_ETHUSDT_Binance)
{
    const std::string json_filepath{ "./fixture/ETHUSDT_exchangeInfo.json" };
    const auto result = binance_parser::parse_financial_instrument(read_file(json_filepath));

    BOOST_CHECK_EQUAL(result.symbol, "ETHUSDT");
    BOOST_CHECK_EQUAL(result.currency, "USDT");
    BOOST_CHECK_EQUAL(result.short_name, "ETH/USDT");
    BOOST_CHECK_EQUAL(result.long_name, "ETH/USDT");
    BOOST_CHECK_EQUAL(result.type, cartera::asset_class::CryptoCurrency);
}

BOOST_AUTO_TEST_CASE(parse_quote_BTCUSDT_Binance)
{
    const std::string json_filepath{ "./fixture/BTCUSDT_24hr.json" };
    const auto result = binance_parser::parse_quote(read_file(json_filepath));

    BOOST_CHECK_EQUAL(result.current_price, 34617.7);
    BOOST_CHECK_EQUAL(result.day_high_price, 35059.09);
    BOOST_CHECK_EQUAL(result.day_low_price, 33532.0);
    BOOST_CHECK_EQUAL(result.day_open_price, 34112.66);
    BOOST_CHECK(result.is_market_open);
    BOOST_CHECK(!result.market_cap.has_value());
    BOOST_CHECK_EQUAL(result.prev_day_close_price, 34110.19);
    BOOST_CHECK_EQUAL(result.symbol, "BTCUSDT");
    BOOST_CHECK_EQUAL(result.volume, 53496.190443);
}

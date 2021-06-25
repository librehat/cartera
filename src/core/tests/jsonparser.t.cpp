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

#include <filesystem>
#include <fstream>
#include <string>

#include "parser/jsonparser.h"
#include "types/financialinstrument.h"

namespace fs = std::filesystem;
using yahoo_parser = cartera::json_parser<cartera::feed_source::YahooFinance>;

static std::string read_file(fs::path path)
{
    std::ifstream f(path, std::ios::in | std::ios::binary);
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
    // FIXME: use working directory and the relative path 
    const std::string json_filepath{ "C:\\cygwin64\\home\\libre\\Projects\\cartera\\src\\core\\tests\\fixture\\IBM.json" };
    const auto result = yahoo_parser::parse_financial_instrument(read_file(json_filepath));
    
    BOOST_CHECK_EQUAL(result.symbol, "IBM");
    BOOST_CHECK_EQUAL(result.exchange_code, "NYQ");
    BOOST_CHECK_EQUAL(result.type, cartera::asset_class::Equity);
    BOOST_CHECK_EQUAL(result.long_name, "International Business Machines Corporation");
    BOOST_CHECK_EQUAL(result.short_name, "International Business Machines");
}

BOOST_AUTO_TEST_CASE(parse_quote_IBM)
{
    // FIXME: use working directory and the relative path 
    const std::string json_filepath{ "C:\\cygwin64\\home\\libre\\Projects\\cartera\\src\\core\\tests\\fixture\\IBM.json" };
    const auto result = yahoo_parser::parse_quote(read_file(json_filepath));

    BOOST_CHECK_EQUAL(result.symbol, "IBM");
    BOOST_CHECK_EQUAL(result.currency, "USD");
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
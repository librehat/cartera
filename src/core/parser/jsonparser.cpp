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

#include "jsonparser.h"
#include "types/exceptions.h"

#include <boost/json.hpp>
#include <sstream>

namespace json = boost::json;

namespace cartera {

namespace {
template <class STRING>
asset_class asset_class_from_quote_type(const STRING& quote_type)
{
    if (quote_type == "EQUITY") {
        return asset_class::Equity;
    }
    if (quote_type == "ETF") {
        return asset_class::ETF;
    }
    if (quote_type == "INDEX") {
        return asset_class::Index;
    }
    if (quote_type == "OPTION") {
        return asset_class::Option;
    }
    if (quote_type == "CRYPTOCURRENCY") {
        return asset_class::CryptoCurrency;
    }
    // TODO: complete the mapping
    return asset_class::Other;
}

double json_value_as_double(const json::value& val)
{
    // Boost.JSON treats it as an integer when there is no decimal place in the number
    if (val.is_double()) {
        return val.get_double();
    }
    if (val.is_int64()) {
        return val.get_int64();
    }
    if (val.is_uint64()) {
        return val.get_uint64();
    }
    // Try to parse the string if that's the type
    if (val.is_string()) {
        return std::stod(val.get_string().data());
    }
    throw cartera_exception("Cannot retrieve double from the JSON value");
}

}

financial_instrument json_parser<feed_source::YahooFinance>::parse_financial_instrument(const std::string& data)
{
    const json::value document = json::parse(data);
    const json::object& root = document.as_object();
    const json::object& quote_summary = root.at("quoteSummary").as_object();
    if (quote_summary.contains("error") && !quote_summary.at("error").is_null()) {
        throw cartera_exception(quote_summary.at("error").as_string().data());
    }
    const json::array& result = quote_summary.at("result").as_array();
    const json::object& quote_result = result.at(0).at("price").as_object();

    const auto& long_name = quote_result.contains("longName") && quote_result.at("longName").is_string() ? quote_result.at("longName") : quote_result.at("shortName");

    return financial_instrument{
        asset_class_from_quote_type(quote_result.at("quoteType").as_string()),
        quote_result.at("symbol").as_string().data(),
        quote_result.at("currency").as_string().data(),
        quote_result.at("exchange").as_string().data(),
        long_name.as_string().data(),
        quote_result.at("shortName").as_string().data(),
    };
}

quote json_parser<feed_source::YahooFinance>::parse_quote(const std::string& data)
{
    const json::value document = json::parse(data);
    const json::object& root = document.as_object();
    const json::object& quote_summary = root.at("quoteSummary").as_object();
    if (quote_summary.contains("error") && !quote_summary.at("error").is_null()) {
        throw cartera_exception(quote_summary.at("error").as_string().data());
    }
    const json::array& result = quote_summary.at("result").as_array();
    const json::object& quote_result = result.at(0).at("price").as_object();
    
    const json::value *market_cap = quote_result.if_contains("marketCap");
    std::optional<double> market_cap_val;
    if (market_cap) {
        market_cap = market_cap->if_object() ? market_cap->as_object().if_contains("raw") : nullptr;
        if (market_cap && market_cap->is_number()) {
            market_cap_val = json_value_as_double(*market_cap);
        }
    }
    const int64_t epoch_s = quote_result.at("regularMarketTime").as_int64();
    const json::value& market_volume = quote_result.at("regularMarketVolume").at("raw");

    return quote{
        datetime(datetime::clock::duration(epoch_s)),
        quote_result.at("symbol").as_string().data(),
        json_value_as_double(quote_result.at("regularMarketDayLow").at("raw")),
        json_value_as_double(quote_result.at("regularMarketDayHigh").at("raw")),
        json_value_as_double(quote_result.at("regularMarketOpen").at("raw")),
        json_value_as_double(quote_result.at("regularMarketPreviousClose").at("raw")),
        json_value_as_double(quote_result.at("regularMarketPrice").at("raw")),
        json_value_as_double(market_volume),
        true, // FIXME
        market_cap_val,
    };
}

std::vector<symbol_search_result> json_parser<feed_source::YahooFinance>::parse_search_quote(const std::string& data)
{
    std::vector<symbol_search_result> results{};
    
    const json::value document = json::parse(data);
    const json::array& quotes = document.at("quotes").as_array();
    for (const auto& quote : quotes) {
        const auto name =
            quote.as_object().contains("longname") ?
            quote.at("longname").as_string() :
            (quote.as_object().contains("shortname") ? quote.at("shortname").as_string() : json::string{""});
        results.emplace_back(
            symbol_search_result{
                asset_class_from_quote_type(quote.at("quoteType").as_string()),
                quote.at("symbol").as_string().data(),
                feed_source::YahooFinance,
                quote.at("exchange").as_string().data(),
                name.data(),
            }
        );
    }
    
    return results;
}


financial_instrument json_parser<feed_source::Binance>::parse_financial_instrument(const std::string& data)
{
    const json::value document = json::parse(data);
    const json::object& symbol = document.at("symbols").as_array()[0].as_object();
    std::ostringstream oss;
    oss << symbol.at("baseAsset").as_string().data() << "/" << symbol.at("quoteAsset").as_string().data();
    const auto name = oss.str();
    return financial_instrument{
        asset_class::CryptoCurrency,
        symbol.at("symbol").as_string().data(),
        symbol.at("quoteAsset").as_string().data(),
        {"BIN"}, // FIXME, a proper exchange code for Binance
        name,
        name,
    };
}

quote json_parser<feed_source::Binance>::parse_quote(const std::string& data)
{
    const json::value document = json::parse(data);

    return quote{
        datetime::clock::now(),
        document.at("symbol").as_string().data(),
        json_value_as_double(document.at("lowPrice")),
        json_value_as_double(document.at("highPrice")),
        json_value_as_double(document.at("openPrice")),
        json_value_as_double(document.at("prevClosePrice")),
        json_value_as_double(document.at("lastPrice")),
        json_value_as_double(document.at("volume")),
        true, // FIXME
        std::optional<double>(),
    };
}

std::vector<symbol_search_result> json_parser<feed_source::Binance>::parse_search_quote(const std::string& data)
{
    const json::value document = json::parse(data);
    const json::array& symbols = document.at("symbols").as_array();

    std::vector<symbol_search_result> results{};
    for (const auto& symbol : symbols) {
        std::ostringstream oss;
        oss << symbol.at("baseAsset").as_string().data() << "/" << symbol.at("quoteAsset").as_string().data();
        results.emplace_back(
            symbol_search_result{
                asset_class::CryptoCurrency,
                symbol.at("symbol").as_string().data(),
                feed_source::Binance,
                {"BIN"}, // FIXME, a proper exchange code for Binance
                oss.str(),
            }
        );
    }
    return results;
}

}  // close cartera namespace

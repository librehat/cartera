#define BOOST_TEST_MODULE basicfeedtest
#include <boost/test/included/unit_test.hpp>

#include "feed/basicfeed.h"
#include "http/client.h"

using namespace cartera;

BOOST_AUTO_TEST_CASE(basic_feed_yahoo_finance_symbol)
{
    simple_http_client client;
    const financial_instrument res = basic_feed<feed_source::YahooFinance>::resolve_symbol(client, "IBM");
    BOOST_CHECK_EQUAL(res.symbol, "IBM");
    BOOST_CHECK_EQUAL(res.exchange_code, "NYQ");
}

BOOST_AUTO_TEST_CASE(basic_feed_yahoo_finance_quote)
{
    simple_http_client client;
    const quote res = basic_feed<feed_source::YahooFinance>::resolve_quote(client, "IBM");
    BOOST_CHECK_EQUAL(res.symbol, "IBM");
    BOOST_CHECK(res.market_cap.has_value());
    if (res.market_cap.has_value()) {
        BOOST_CHECK_GT(res.market_cap.value(), 1);
    }
    BOOST_CHECK_GT(res.current_price, 0);
    BOOST_CHECK_GT(res.volume, 0);
}

BOOST_AUTO_TEST_CASE(basic_feed_yahoo_finance_search)
{
    simple_http_client client;
    const auto results = basic_feed<feed_source::YahooFinance>::search_symbols(client, "IBM");
    BOOST_CHECK_GT(results.size(), 2);
    // Just check IBM itself is the first one and the two are different for now
    BOOST_CHECK_EQUAL(results[0].symbol, "IBM");
    BOOST_CHECK_EQUAL(results[0].exchange_code, "NYQ");
    BOOST_CHECK_EQUAL(results[0].source, cartera::feed_source::YahooFinance);
    BOOST_CHECK_EQUAL(results[0].type, cartera::asset_class::Equity);
    BOOST_CHECK_EQUAL(results[0].name, "International Business Machines Corporation");
    BOOST_CHECK_NE(results[0], results[1]);
}

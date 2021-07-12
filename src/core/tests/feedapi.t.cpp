#define BOOST_TEST_MODULE feedapitest
#include <boost/test/included/unit_test.hpp>

#include "feed/api.h"

using namespace cartera;

feed::api api;

BOOST_AUTO_TEST_CASE(get_bulk_quotes_in_order)
{
    const auto results = api.get_quotes({
        position_identifier{feed_source::YahooFinance, {"VOD.L"}},
        position_identifier{feed_source::Binance, {"ETHBTC"}},
        position_identifier{feed_source::YahooFinance, {"IBM"}},
        position_identifier{feed_source::Binance, {"BTCUSDT"}},
        position_identifier{feed_source::YahooFinance, {"MA"}}
        });
    BOOST_REQUIRE_EQUAL(results.size(), 5);
    BOOST_CHECK_EQUAL(results[0].symbol, "VOD.L");
    BOOST_CHECK_EQUAL(results[1].symbol, "ETHBTC");
    BOOST_CHECK_EQUAL(results[2].symbol, "IBM");
    BOOST_CHECK_EQUAL(results[3].symbol, "BTCUSDT");
    BOOST_CHECK_EQUAL(results[4].symbol, "MA");
}

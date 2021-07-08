#define BOOST_TEST_MODULE httpclienttest
#include <boost/test/included/unit_test.hpp>

#include "http/client.h"

using namespace cartera;

simple_http_client client;

BOOST_AUTO_TEST_CASE(http_client_simple_get_basic)
{
    const std::string resp = client.get("https://query1.finance.yahoo.com/v10/finance/quoteSummary/IBM?modules=price");
    BOOST_CHECK_GT(resp.size(), 1);
    BOOST_CHECK_EQUAL(resp[0], '{');
}

BOOST_AUTO_TEST_CASE(http_client_simple_get_basic_redirect)
{
    const std::string resp = client.get("http://query1.finance.yahoo.com/v10/finance/quoteSummary/IBM?modules=price");
    BOOST_CHECK_GT(resp.size(), 1);
    BOOST_CHECK_EQUAL(resp[0], '{');
}

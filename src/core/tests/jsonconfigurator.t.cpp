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

#define BOOST_TEST_MODULE jsonconfiguratortest
#include <boost/test/included/unit_test.hpp>

#include <boost/filesystem.hpp>

#include "config/jsonconfigurator.h"

namespace fs = boost::filesystem;


BOOST_AUTO_TEST_CASE(list_all_watch_lists)
{
    cartera::config::json_configurator configurator("./fixture/sample_config");
    const auto all_lists = configurator.list_watch_lists();
    BOOST_CHECK_EQUAL(all_lists.size(), 3);
}

BOOST_AUTO_TEST_CASE(get_existing_list)
{
    cartera::config::json_configurator configurator("./fixture/sample_config");
    const auto list1_symbols = configurator.read_watch_list_symbols("List 1");
    BOOST_REQUIRE_EQUAL(list1_symbols.size(), 2);
    BOOST_CHECK_EQUAL(list1_symbols[0].source, static_cast<cartera::feed_source>(0));
    BOOST_CHECK_EQUAL(list1_symbols[0].symbol, "IBM");
    BOOST_CHECK_EQUAL(list1_symbols[1].source, static_cast<cartera::feed_source>(0));
    BOOST_CHECK_EQUAL(list1_symbols[1].symbol, "VOD.L");
}

BOOST_AUTO_TEST_CASE(get_existing_empty_list)
{
    cartera::config::json_configurator configurator("./fixture/sample_config");
    const auto list_symbols = configurator.read_watch_list_symbols("Empty List");
    BOOST_CHECK(list_symbols.empty());
}

BOOST_AUTO_TEST_CASE(start_in_empty_directory_and_save)
{
    const auto temp_config_dir = fs::temp_directory_path() / fs::unique_path();
    cartera::config::json_configurator configurator(temp_config_dir.string());
    const auto all_lists = configurator.list_watch_lists();
    BOOST_CHECK(all_lists.empty());
    
    const std::vector<cartera::config::watch_list_item> items{
        {cartera::feed_source::YahooFinance, "GOOG"},
        {cartera::feed_source::Binance, "ETHUSDT"}
    };
    configurator.save_watch_list_symbols("My Test List", items);
    const auto saved_list = configurator.read_watch_list_symbols("My Test List");

    BOOST_CHECK_EQUAL_COLLECTIONS(saved_list.begin(), saved_list.end(), items.begin(), items.end());

    // clean up
    fs::remove_all(temp_config_dir);
}
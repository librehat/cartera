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

#include "jsonconfigurator.h"

#include <boost/filesystem.hpp>
#include <boost/json.hpp>
#include <stdexcept>

namespace json = boost::json;
namespace fs = boost::filesystem;

namespace cartera {
namespace config {

namespace {
static std::string read_file(const fs::path& path)
{
    fs::ifstream f(path, std::ios::in | std::ios::binary);
    const auto sz = fs::file_size(path);
    std::string result(sz, '\0');
    f.read(result.data(), sz);
    return result;
}

static void write_file(const fs::path& path, const std::string& content)
{
    fs::ofstream f(path, std::ios::out | std::ios::binary);
    f << content;
}
}  // close unnamed namespace

json_configurator::json_configurator(const std::string& config_path)
    : m_config_path(config_path)
    , m_watch_list_config_file_path(config_path + "/watchlists.json")
{
    initialise();
}

json_configurator::json_configurator(std::string&& config_path)
    : m_config_path(std::move(config_path))
    , m_watch_list_config_file_path(m_config_path + "/watchlists.json")
{
    initialise();
}

void json_configurator::initialise()
{
    const fs::path config_dir(m_config_path);
    if (!fs::exists(config_dir)) {
        fs::create_directory(config_dir);
    }
    else if (!fs::is_directory(config_dir)) {
        throw std::runtime_error(m_config_path + " exists but is not a directory");
    }

    // create empty valid configuration files if they don't exist
    const fs::path watch_list_file_path(m_watch_list_config_file_path);
    if (!fs::exists(watch_list_file_path)) {
        write_file(watch_list_file_path, "[]");
    }
}

std::vector<std::string> json_configurator::list_watch_lists()
{
    const auto document = json::parse(read_file(m_watch_list_config_file_path));
    const json::array& watch_lists = document.as_array();
    std::vector<std::string> names;
    names.reserve(watch_lists.size());
    for (const auto& item : watch_lists) {
        names.emplace_back(item.at("name").as_string().data());
    }
    return names;
}

std::vector<watch_list_item> json_configurator::read_watch_list_symbols(const std::string_view& list_name)
{
    const auto document = json::parse(read_file(m_watch_list_config_file_path));
    const json::array& watch_lists = document.as_array();
    for (const auto& item : watch_lists) {
        if (list_name.compare(item.at("name").as_string().data()) == 0) {
            // found the list
            const json::array& list_mems = item.at("members").as_array();
            std::vector<watch_list_item> out;
            out.reserve(list_mems.size());
            for (const auto& mem : list_mems) {
                out.emplace_back(
                    watch_list_item{
                        static_cast<feed_source>(mem.at("source").as_int64()),
                        mem.at("symbol").as_string().data()
                    }
                );
            }
            return out;
        }
    }
    throw std::out_of_range("Cannot find the specified list=" + std::string{ list_name });
}

void json_configurator::save_watch_list_symbols(const std::string_view& list_name, const std::vector<watch_list_item>& items)
{
    json::array json_mems;
    json_mems.reserve(items.size());
    for (const auto& list_item : items) {
        json_mems.emplace_back(
            json::object{ { "source", static_cast<int>(list_item.source) }, { "symbol", list_item.symbol } }
        );
    }

    auto document = json::parse(read_file(m_watch_list_config_file_path));
    json::array& watch_lists = document.as_array();
    bool exists = false;
    for (auto& item : watch_lists) {
        if (list_name.compare(item.at("name").as_string().data()) == 0) {
            item.as_object().at("members") = std::move(json_mems);
            exists = true;
        }
    }
    if (!exists) {
        watch_lists.emplace_back(
            json::object{ { "name", list_name }, { "members", std::move(json_mems) } }
        );
    }
    write_file(m_watch_list_config_file_path, json::serialize(watch_lists));
}


}  // close config namespace
}  // close cartera namespace
/*
 * Manages the cartera configurations in JSON format
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

#ifndef CARTERA_CONFIG_JSONCONFIGURATOR_H
#define CARTERA_CONFIG_JSONCONFIGURATOR_H

#include "types/positionidentifier.h"

#include <string>
#include <vector>
#include <mutex>

namespace cartera {
namespace config {

using watch_list_item = position_identifier;

// JSON files backed configurations manager
class json_configurator {
public:
    // the current user must have read and write permission in 'config_path'
    explicit json_configurator(const std::string& config_path);
    explicit json_configurator(std::string&& config_path);

    // get all watch lists' names in the configured order
    std::vector<std::string> list_watch_lists();

    // read data of a given watch list
    // e.g. a valid JSON structure:
    // { "name": "My Watch List", "members": [ { "symbol": "IBM", "source": 0 }, ...] }
    std::vector<watch_list_item> read_watch_list_symbols(const std::string_view& list_name);

    // save a watch list. if the list doesn't exist, a new one will be created and inserted to the end
    void save_watch_list_symbols(const std::string_view& list_name, const std::vector<watch_list_item>& items);

private:
    const std::string m_config_path;
    const std::string m_watch_list_config_file_path;
    std::mutex m_mutex;

    
    void initialise();
    std::string read_file(const std::string& path);
    void write_file(const std::string& path, const std::string& content);
};

}
}

#endif // CARTERA_CONFIGURATOR_JSONCONFIGURATOR_H

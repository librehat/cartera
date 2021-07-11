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

namespace cartera {
namespace config {

class json_configurator {
public:
    explicit json_configurator(const std::string& config_path);

private:
    const std::string m_config_path;
};

}
}

#endif // CARTERA_CONFIGURATOR_JSONCONFIGURATOR_H

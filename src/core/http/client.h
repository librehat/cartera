/*
 * A wrapper around underlying networking library implementation
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

#ifndef CARTERA_HTTP_CLIENT_H
#define CARTERA_HTTP_CLIENT_H
#include <future>
#include <string>

#ifndef CARTERACORE_HTTP_CLIENT_TIMEOUT
// The default timeout would be 60s
#define CARTERACORE_HTTP_CLIENT_TIMEOUT 60000
#endif

namespace cartera {

class simple_http_client {
public:
    explicit simple_http_client();

    virtual ~simple_http_client();

    std::string get(const std::string& url) const;

    std::future<std::string> get_future(const std::string& url) const;
};

}

#endif

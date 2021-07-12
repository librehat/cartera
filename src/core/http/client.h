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

#include <string>

#ifndef CARTERACORE_HTTP_CLIENT_TIMEOUT
// The default timeout would be 60s
#define CARTERACORE_HTTP_CLIENT_TIMEOUT 60000
#endif

// Forward declare the context class to keep boost private
namespace boost {
namespace asio {
namespace ssl {
class context;
}  // close ssl namespace
class io_context;
}  // close asio namespace
}  // close boost namespace

namespace cartera {

class simple_http_client {
public:
    explicit simple_http_client();

    virtual ~simple_http_client();

    // FIXME
    // TODO: `url` should be encoded here instead of relying on the caller 
    std::string get(const std::string& url) const;

protected:
    boost::asio::io_context* m_io_ctx;
    boost::asio::ssl::context* m_ssl_ctx;
};

}  // close cartera namespace

#endif

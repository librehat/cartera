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

#include "client.h"
#include "types/exceptions.h"

#include <memory>
#include <cpr/cpr.h>

namespace cartera {

namespace {

std::string extract_response(const cpr::Response& r) {
    if (r.error.code != cpr::ErrorCode::OK) {
        throw cartera_exception(r.error.message);
    }
    return r.text;
}

}

simple_http_client::simple_http_client()
{
}

simple_http_client::~simple_http_client()
{
}

std::string simple_http_client::get(const std::string& url)
{
    const cpr::Response r = cpr::Get(
        cpr::Url{ url },
        cpr::Timeout{CARTERACORE_HTTP_CLIENT_TIMEOUT}
    );
    return extract_response(r);
}

std::future<std::string> simple_http_client::get_future(const std::string& url)
{
    auto response_text_p = std::make_shared<std::promise<std::string>>();
    const auto fut = cpr::GetCallback(
        [response_text_p](const cpr::Response& r) {
            try {
                response_text_p->set_value(extract_response(r));
            }
            catch (...) {
                response_text_p->set_exception(std::current_exception());
            }
        },
        cpr::Url{ url },
        cpr::Timeout{ CARTERACORE_HTTP_CLIENT_TIMEOUT }
    );
    return response_text_p->get_future();
}

}
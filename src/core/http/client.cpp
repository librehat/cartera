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
 * 
 * The implementation is largely based on the boost example code.
 * https://www.boost.org/doc/libs/1_76_0/libs/beast/example/http/client/sync-ssl/http_client_sync_ssl.cpp
 */

#include "client.h"
#include "types/exceptions.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/algorithm/string.hpp> // for case-insensitive string comparison

#include <root_certificates.hpp>

#include <regex>

namespace beast = boost::beast;
namespace http = beast::http;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

namespace cartera {

namespace {

// TODO: replacing the regex with a standardised URI library
// Based on https://github.com/boostorg/beast/issues/787#issuecomment-376259849
struct ParsedURI {
    std::string protocol;
    std::string domain;  // only domain must be present
    std::string port;
    std::string resource;
    std::string query;   // everything after '?', possibly nothing
};

ParsedURI parseURI(const std::string& url) {
    ParsedURI result;
    auto value_or = [](const std::string& value, std::string&& deflt) -> std::string {
        return (value.empty() ? deflt : value);
    };
    // Note: only "http", "https", "ws", and "wss" protocols are supported
    static const std::regex PARSE_URL{ R"((([httpsw]{2,5})://)?([^/ :]+)(:(\d+))?(/([^ ?]+)?)?/?\??([^/ ]+\=[^/ ]+)?)",
                                       std::regex_constants::ECMAScript | std::regex_constants::icase };
    std::smatch match;
    if (std::regex_match(url, match, PARSE_URL) && match.size() == 9) {
        result.protocol = value_or(boost::algorithm::to_lower_copy(std::string(match[2])), "http");
        result.domain = match[3];
        const bool is_sequre_protocol = (result.protocol == "https" || result.protocol == "wss");
        result.port = value_or(match[5], (is_sequre_protocol) ? "443" : "80");
        result.resource = value_or(match[6], "/");
        result.query = match[8];
        if (result.domain.empty()) {
            throw cartera_exception("Empty domain");
        }
    }
    return result;
}

template<class STREAM>
http::response<http::string_body> send_http_request(STREAM& stream, http::request<http::string_body>& req)
{
    http::write(stream, req);
    beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(stream, buffer, res);
    return res;
}

http::response<http::string_body> do_http(
    boost::asio::io_context& io_ctx,
    const boost::asio::ip::basic_resolver_results<tcp>& resolv_results,
    http::request<http::string_body>& req
)
{
    beast::tcp_stream stream(io_ctx);
    stream.connect(resolv_results);
    auto res = send_http_request(stream, req);
    stream.socket().shutdown(tcp::socket::shutdown_both);
    return res;
}

http::response<http::string_body> do_https(
    boost::asio::io_context& io_ctx,
    ssl::context& ssl_ctx,
    const std::string& domain,
    const boost::asio::ip::basic_resolver_results<tcp>& resolv_results,
    http::request<http::string_body>& req
)
{
    beast::ssl_stream<beast::tcp_stream> stream(io_ctx, ssl_ctx);

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (!SSL_set_tlsext_host_name(stream.native_handle(), domain.data()))
    {
        beast::error_code ec{ static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category() };
        throw beast::system_error{ ec };
    }

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(stream).connect(resolv_results);

    // Perform the SSL handshake
    stream.handshake(ssl::stream_base::client);

    auto res = send_http_request(stream, req);

    beast::error_code ec;
    stream.shutdown(ec);
    if (ec == boost::asio::error::eof || ec == ssl::error::stream_truncated) {
        ec = {};
    }
    if (ec) {
        throw beast::system_error{ ec };
    }

    return res;
}

}

simple_http_client::simple_http_client()
    : m_io_ctx(new boost::asio::io_context())
    , m_ssl_ctx(new ssl::context(ssl::context::tlsv13_client))
{
    load_root_certificates(*m_ssl_ctx);
    m_ssl_ctx->set_verify_mode(ssl::verify_peer);
}

simple_http_client::~simple_http_client()
{
    delete m_ssl_ctx;
    delete m_io_ctx;
}

std::string simple_http_client::get(const std::string& url) const
{
    const auto uri = parseURI(url);

    tcp::resolver resolver(*m_io_ctx);
    // Look up the domain name
    const auto resolv_results = resolver.resolve(uri.domain, uri.port);

    // Set up an HTTP GET request message
    const std::string target = uri.resource + (uri.query.empty() ? "" : ("?" + uri.query));
    http::request<http::string_body> req{ http::verb::get, target, 11 };
    req.set(http::field::host, uri.domain);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    http::response<http::string_body> res;
    if (uri.protocol.compare("https") == 0) {
        res = do_https(*m_io_ctx, *m_ssl_ctx, uri.domain, resolv_results, req);
    }
    else if (uri.protocol.compare("http") == 0) {
        res = do_http(*m_io_ctx, resolv_results, req);
    }
    else {
        throw cartera_exception("Unsupported protocol");
    }

    if (res.result() == http::status::moved_permanently) {
        return get(res.base().at("Location").to_string());
    }
    if (res.result() != http::status::ok) {
        throw cartera_exception(res.reason().data());
    }

    return res.body();
}

}

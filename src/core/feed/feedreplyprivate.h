/*
 * <one line to give the library's name and an idea of what it does.>
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

#ifndef CARTERA_FEEDREPLYPRIVATE_H
#define CARTERA_FEEDREPLYPRIVATE_H

#include <exception>
#include <functional>
#include <variant>
#include <string>

#include <boost/beast/http.hpp>

namespace http = boost::beast::http;

namespace cartera {

using string_response = http::response<http::string_body>;

class FeedException : public std::exception {
public:
    explicit FeedException(const std::string& error_message);
    
    virtual const char* what() const noexcept override;
    
private:
    const std::string m_error_message;
};


template<typename TYPE>
class FeedReply {
public:
    /**
     * Function that converts the basic string from the response to the value type
     */
    using ValueFn = std::function<TYPE(std::string)>;
    
    explicit FeedReply(string_response *resp, ValueFn func)
    : m_data(std::string())
    , m_func(std::move(func))
    {
    }
    
    ~FeedReply() = default;
    
    bool hasError() const
    {  // TODO check the response
        return m_data.index() == 1;
    }
    
    const std::string& errorMessage() const
    {
        return std::get<std::string>(m_data);
    }
    
    const TYPE& value() const
    {
        return std::get<TYPE>(m_data);
    }

protected:/*
    virtual void handleReplyData() override
    {
        if (m_reply->error() != QNetworkReply::NoError) {
            m_data = m_reply->errorString();
        } else {
            try {
                m_data = m_func(m_reply->readAll());
            } catch (const FeedException& e) {
                qDebug() << "Error while handling the reply data" << e.what();
                m_data = QString("Unknown Error");
            }
        }
    }*/
    
private:
    std::variant<TYPE, std::string> m_data;
    ValueFn m_func;
};

}

#endif // CARTERA_FEEDREPLYPRIVATE_H

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
#ifndef CARTERA_EXCEPTIONS_H
#define CARTERA_EXCEPTIONS_H

#include <exception>

namespace cartera {

class cartera_exception : public std::exception {
public:
    explicit cartera_exception(const std::string& msg) : m_message(msg)
    {}

    explicit cartera_exception(const char* msg) : m_message({ msg })
    {}

    virtual const char* what() const noexcept override
    {
        return m_message.data();
    }

protected:
    const std::string m_message;
};

}

#endif
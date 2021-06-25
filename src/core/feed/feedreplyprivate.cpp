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

#include "feedreplyprivate.h"

namespace cartera {


// -------------------
// class FeedException
// -------------------

FeedException::FeedException(const std::string& error_message)
: std::exception()
, m_error_message(error_message)
{
}

const char* FeedException::what() const noexcept
{
    return m_error_message.data();
}

}  // close cartera namespace

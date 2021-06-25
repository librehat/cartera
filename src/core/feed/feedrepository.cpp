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

#include "feedrepository.h"

namespace cartera {

FeedRepository::FeedRepository()
{}

void FeedRepository::addFeedHandler(const feed_source& identifier, AbstractFeed *feed)
{
    m_feeds[identifier] = feed;
}

AbstractFeed* FeedRepository::getFeedHanlder(const feed_source& identifier)
{
    if (identifier == feed_source::k_END) {
        return nullptr;
    }
    return m_feeds[identifier];
}

}  // close cartera namespace

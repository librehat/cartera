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

using namespace cartera;


// -------------------
// class FeedException
// -------------------

FeedException::FeedException(const QString& errorMessage)
: std::exception()
, m_errorMessage(errorMessage)
{
}

const char* FeedException::what() const noexcept
{
    return m_errorMessage.toStdString().data();
}

// ----------------------
// Class FeedReplyPrivate
// ----------------------

FeedReplyPrivate::FeedReplyPrivate(QNetworkReply *reply, QObject *parent)
: QObject(parent)
, m_reply(reply)
{
    connect(m_reply, &QNetworkReply::finished, this, &FeedReplyPrivate::handleFinished);
}

bool FeedReplyPrivate::isFinished() const
{
    return m_reply->isFinished();
}

bool FeedReplyPrivate::isRunning() const
{
    return m_reply->isRunning();
}

void FeedReplyPrivate::handleFinished()
{
    this->handleReplyData();
    emit finished();
}

FeedReplyPrivate::~FeedReplyPrivate()
{
    m_reply->deleteLater();
}

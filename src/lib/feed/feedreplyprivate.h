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

#include <qobject.h>
#include <qnetworkreply.h>

#include <exception>
#include <functional>
#include <variant>

namespace cartera {
    
class FeedException : public std::exception {
public:
    explicit FeedException(const QString& errorMessage);
    
    virtual const char* what() const noexcept override;
    
private:
    const QString m_errorMessage;
};

/**
 * A private class in order to support templated 'FeedReply'
 */
class FeedReplyPrivate : public QObject
{
    Q_OBJECT

public:
    explicit FeedReplyPrivate(QNetworkReply *reply, QObject *parent = nullptr);
    virtual ~FeedReplyPrivate();
    
    bool isFinished() const;
    bool isRunning() const;
    
signals:
    void finished();
    
protected:
    bool m_isFinished;
    QNetworkReply *m_reply;
    
    virtual void handleReplyData() = 0;
    
private slots:
    void handleFinished();
};


template<typename TYPE>
class FeedReply : public FeedReplyPrivate {
public:
    /**
     * Function that converts the byte array from the I/O device to the value type
     */
    using ValueFn = std::function<TYPE(QByteArray)>;
    
    explicit FeedReply(QNetworkReply *reply, ValueFn func)
    : FeedReplyPrivate(reply)
    , m_data(QString())
    , m_func(std::move(func))
    {
    }
    
    ~FeedReply() = default;
    
    bool hasError() const
    {
        return isFinished() && m_data.index() == 1;
    }
    
    const QString& errorMessage() const
    {
        return std::get<QString>(m_data);
    }
    
    const TYPE& value() const
    {
        return std::get<TYPE>(m_data);
    }

protected:
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
    }
    
private:
    std::variant<TYPE, QString> m_data;
    ValueFn m_func;
};

}

#endif // CARTERA_FEEDREPLYPRIVATE_H

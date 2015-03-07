#include "networkconnection.h"

#include <QDebug>

namespace terminus
{

NetworkConnection::NetworkConnection(QObject *parent)
: NetworkConnection(nullptr, parent)
{

}

NetworkConnection::NetworkConnection(QTcpSocket* socket, QObject *parent)
: QObject(parent)
{
    m_socket = socket != nullptr ? socket : new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::connected, this, &NetworkConnection::onSocketConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkConnection::onSocketReadyRead);
    connect(m_socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &NetworkConnection::onSocketError);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkConnection::onSockedDisconnected);
}

NetworkConnection* NetworkConnection::fromTcpSocket(QTcpSocket* socket, QObject* parent)
{
    return new NetworkConnection(socket, parent);
}

void NetworkConnection::connectToHost(const QString &address, quint16 port)
{
    m_socket->connectToHost(address, port);
}

void NetworkConnection::abort()
{
    m_socket->abort();
}

void NetworkConnection::close()
{
    m_socket->close();
}

qint64 NetworkConnection::bytesAvailable() const
{
    return m_socket->bytesAvailable();
}

std::unique_ptr<QDataStream> NetworkConnection::inDataStream()
{
    auto in = std::unique_ptr<QDataStream>(new QDataStream(m_socket));
    in->setVersion(QDataStream::Qt_5_4);
    return in;
}

qint64 NetworkConnection::write(const QByteArray &data)
{
    if (m_socket->state() != QAbstractSocket::ConnectedState)
    {
        qWarning() << "socket is not in ConnectedState but in " << m_socket->state();

    }

    if(isConnected())
    {
        int ret = m_socket->write(data);
        if (ret != -1)
        {
            bool flushSuccess = m_socket->flush();
            if(!flushSuccess)
            {
                qWarning() << "flush() returned false";
            }
        }
        else
        {
            qWarning() << "write yielded -1";
        }
        return ret;
    }
    else
    {
        // schedule data to be sent when socket is connected
        if (m_dataToBeSent.isEmpty())
        {
            QByteArray debugByteArray(data);
            QDataStream debugInStream(debugByteArray);
            debugInStream.setVersion(QDataStream::Qt_5_2);

            quint16 debugMsgSize;
            QString debugMsgString;

            debugInStream >> debugMsgSize >> debugMsgString;

            qDebug() << "... holding back data for " <<  peerAddress().toString() << ":" << peerPort() << " ('" << debugMsgString << "', " << debugMsgSize << " bytes)";
            m_dataToBeSent = data;
            return debugMsgSize;
        }
        else
        {
            QByteArray debugByteArray(data);
            QDataStream debugInStream(debugByteArray);
            debugInStream.setVersion(QDataStream::Qt_5_4);

            quint16 debugMsgSize;
            QString debugMsgString;

            debugInStream >> debugMsgSize >> debugMsgString;
            qWarning() << "there is already data waiting to be sent to " <<  peerAddress().toString() << ":" << peerPort() << ", discarding this packet! ('" << debugMsgString << "', " << debugMsgSize << " bytes)";
            return -1;
        }
    }
}

bool NetworkConnection::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

QHostAddress NetworkConnection::localAddress() const
{
    return m_socket->localAddress();
}

QHostAddress NetworkConnection::peerAddress() const
{
    return m_socket->peerAddress();
}

quint16 NetworkConnection::localPort() const
{
    return m_socket->localPort();
}

quint16 NetworkConnection::peerPort() const
{
    return m_socket->peerPort();
}

void NetworkConnection::disconnectFromHost()
{
    m_socket->disconnectFromHost();
}

void NetworkConnection::onSocketConnected()
{
    qDebug() << "Socket to " << peerAddress().toString() << ":" << peerPort() << " connected!";

    // send data that was held back, if any
    if (!m_dataToBeSent.isEmpty())
    {
        QByteArray debugByteArray(m_dataToBeSent);
        QDataStream debugInStream(debugByteArray);
        debugInStream.setVersion(QDataStream::Qt_5_4);

        quint16 debugMsgSize;
        QString debugMsgString;

        debugInStream >> debugMsgSize >> debugMsgString;
        qDebug() << "sending data that was held back for " << peerAddress().toString() << ":" << peerPort() << "('" << debugMsgString << "', " << debugMsgSize << " bytes)";

        write(m_dataToBeSent);
        m_dataToBeSent.clear();
    }

    emit connected();
}

void NetworkConnection::onSocketReadyRead()
{
    emit readyRead();
}

void NetworkConnection::onSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "socket error! " << socketError;
    emit error(socketError);
}

void NetworkConnection::onSockedDisconnected()
{
    emit disconnected();
}
}

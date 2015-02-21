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
    int ret = m_socket->write(data);
    if (ret == -1)
    {
        qWarning() << "write yielded -1";
    }
    return ret;
}

bool NetworkConnection::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void NetworkConnection::disconnectFromHost()
{
    m_socket->disconnectFromHost();
}

void NetworkConnection::onSocketConnected()
{
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

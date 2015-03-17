#include "networkclient.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTcpSocket>

namespace terminus
{

NetworkClient::NetworkClient(QObject * parent)
: NetworkEndpoint(parent)
{
    setSocket(new QTcpSocket(this));
    connect(m_socket, &QTcpSocket::connected,    this, &NetworkClient::onSocketConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkClient::onSocketDisconnected);
    connect(m_socket, &QTcpSocket::readyRead,    this, &NetworkClient::onDataReceived);
}

void NetworkClient::connectClient(QString host, unsigned short port) {
    qDebug() << "Connecting to " << host << ":" << port;
    m_socket->connectToHost(host, port);
}

void NetworkClient::onSocketConnected()
{
    enterState(State::Connected);
}

void NetworkClient::onSocketDisconnected()
{
    enterState(State::Disconnected);
}

} //namespace terminus

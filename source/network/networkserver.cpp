#include "networkserver.h"

#include <cassert>

#include <QDebug>
#include <QHostAddress>

#include <network/commands/abstractcommand.h>
#include <network/commands/preparenewgamecommand.h>
#include "networkconnection.h"

namespace terminus
{

NetworkServer::NetworkServer(QObject* parent)
: NetworkEndpoint(parent)
, m_server(new QTcpServer(this))
{
    connect(m_server, &QTcpServer::newConnection, this, &NetworkServer::onClientConnected);
}

NetworkServer::~NetworkServer()
{
    m_server->close();
}

bool NetworkServer::listen(unsigned short port)
{
    if(!m_server->listen(QHostAddress::Any, port)) {
        qDebug() << "Could not start server";
        return false;
    }

    qDebug() << "Listening on Port" << port;

    enterState(State::Listening);

    return true;
}

void NetworkServer::onClientConnected()
{
    m_socket = m_server->nextPendingConnection();
    m_server->close();

    connect(m_socket, &QTcpSocket::disconnected, m_socket, &QTcpSocket::deleteLater);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkServer::onClientDisconnected);
    connect(m_socket, &QTcpSocket::readyRead,    this, &NetworkServer::onDataReceived);

    qDebug() << "Client connected!";

    enterState(State::Connected);
}

void NetworkServer::onClientDisconnected() {
    m_socket = nullptr;
    enterState(State::Disconnected);
}

} //namespace terminus

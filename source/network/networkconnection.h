#pragma once

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>

#include <memory>

namespace terminus
{
	class NetworkConnection : public QObject
	{
		Q_OBJECT
	public:
		NetworkConnection(QObject *parent = 0);

		virtual void connectToHost(const QString &address, quint16 port);
		virtual void abort();
		virtual void close();
		virtual void disconnectFromHost();
		virtual qint64 bytesAvailable() const;
		virtual std::unique_ptr<QDataStream> inDataStream();
		virtual qint64 write(const QByteArray &data);
        virtual bool isConnected() const;

        virtual QHostAddress localAddress() const;
        virtual QHostAddress peerAddress() const;
        virtual quint16 localPort() const;
        virtual quint16 peerPort() const;

		static NetworkConnection* fromTcpSocket(QTcpSocket* socket, QObject* parent = 0);
	signals:
		void readyRead();
		void connected();
		void error(QAbstractSocket::SocketError);
		void disconnected();

	private:
		NetworkConnection(QTcpSocket* socket, QObject *parent = 0);
		QTcpSocket* m_socket;
        QByteArray m_dataToBeSent;

	private slots:
		void onSocketReadyRead();
		void onSocketConnected();
		void onSocketError(QAbstractSocket::SocketError);
		void onSockedDisconnected();
	};
}

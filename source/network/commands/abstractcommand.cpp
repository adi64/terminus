#include <QDebug>

#include "abstractcommand.h"


namespace terminus
{
    AbstractCommand::AbstractCommand(QObject* parent)
		: QObject(parent)
	{

	}

	NetworkConnection* AbstractCommand::clientConnection() {
		return m_clientConnection;
	}

	void AbstractCommand::run() {
		doWork();
	}
}

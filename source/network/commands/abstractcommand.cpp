#include <QDebug>

#include "abstractcommand.h"


namespace PCLIB
{
    AbstractCommand::AbstractCommand(NetworkConnection* clientConnection, QObject* parent)
		: QObject(parent)
		, m_clientConnection(clientConnection)
	{

	}

	int AbstractCommand::percentComplete() {
		return m_percentComplete;
	}

	QString AbstractCommand::status() {
		return m_status;
	}

	NetworkConnection* AbstractCommand::clientConnection() {
		return m_clientConnection;
	}

	int AbstractCommand::defaultProgressInterval() {
		return 1000;
	}

	void AbstractCommand::run() {
		emit started();
		doWork();
		
		// command spawns new thread via classificationInterface, so return of doWork() does not mean that the job is done
		//emit done();
	}

	void AbstractCommand::setProgress(int total, int current) {
		m_percentComplete = ((float) current / total) * 100;
	}

	void AbstractCommand::maxProgress(int maxProgress) {
		m_maxProgress = maxProgress;
	}

	void AbstractCommand::updateProgress(int progress) {
		setProgress(m_maxProgress, progress);
	}

	void AbstractCommand::updateProgressLabel(QString progress) {
		m_status = progress;
	}

	void AbstractCommand::classificationInterfaceFinished() {
		emit done();
	}

	bool AbstractCommand::exitOnComplete() {
		return true;
	}
}

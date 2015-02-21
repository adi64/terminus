#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <memory>

namespace PCLIB
{
    class NetworkServer;
	class NetworkConnection;

	class AbstractCommand : public QObject
	{
		Q_OBJECT
	public:
        AbstractCommand(NetworkConnection* clientConnection, QObject* parent = 0);
        virtual ~AbstractCommand() {}
		int percentComplete();
		QString status();
		void run();
		virtual void doWork() = 0;
		virtual void initializeFromJson(QJsonObject) = 0;
		NetworkConnection* clientConnection();
	signals:
		void started();
		void done();
	protected:
		NetworkConnection* m_clientConnection;
	};

	enum Commands {
		Command_Hello = 0,

		Command_TopologicalAnalysis = 1,
		Command_DifferenceAnalysis = 2,
		Command_BuildingAnalysis = 3,
		Command_TreeDetection = 4,
		Command_GroundDetection = 5,
		Command_PCManipulator = 6,
		Command_TreefileCreator = 7,
		Command_ChangeDetection = 8,
		Command_TrainprofileAnalysis = 9,
		Command_FootprintClassification = 10,
		Command_TIFFCreator = 11,
		Command_ShapeFileTools = 12,

		Command_ListDirectory = 13,

		Command_Notification = 14
	};

	enum ServerMessages {
		Progress = 0,
		Result = 1,
		Log = 2
	};
}

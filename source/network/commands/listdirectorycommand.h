#pragma once

#include <QDir>
#include <QJsonArray>

#include "abstractcommand.h"
namespace terminus
{

class ListDirectoryCommand : public AbstractCommand
{
	Q_OBJECT
public:
    ListDirectoryCommand(QObject* parent = 0);
	void initializeFromJson(QJsonObject) override;
	void doWork() override;

private:
	QDir m_currentDirectory;
	QString m_changeDirectory;
	QJsonArray m_files;
	QStringList m_fileFilter;
};
}

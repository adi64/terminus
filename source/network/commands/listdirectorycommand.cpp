#include "listdirectorycommand.h"

#include <QDebug>
namespace terminus
{
    ListDirectoryCommand::ListDirectoryCommand(QObject* parent) :
        AbstractCommand(parent)
		, m_currentDirectory()
		, m_files()
		, m_fileFilter()
	{
		
	}

	void ListDirectoryCommand::initializeFromJson(QJsonObject json) {
		QString currentDirectory = json.value("currentDirectory").toString();
		QFileInfo fileInfo = QFileInfo(currentDirectory);
		if (!fileInfo.exists())
			m_currentDirectory = QDir();
		else if (fileInfo.isDir())
			m_currentDirectory = QDir(currentDirectory);
		else
			m_currentDirectory = fileInfo.absoluteDir();

		m_changeDirectory = json.value("changeDirectory").toString();
		
		auto fileFilter = json.value("fileFilter").toArray();
		for (auto f : fileFilter)
			m_fileFilter << f.toString();
	}

	void ListDirectoryCommand::doWork() {
		m_currentDirectory.cd(m_changeDirectory);
		auto fileInfoList = m_currentDirectory.entryInfoList(m_fileFilter, QDir::AllDirs | QDir::Files);
		for (auto fileInfo : fileInfoList) {
			QJsonObject file;
			file.insert("fileName", fileInfo.fileName());
			file.insert("size", fileInfo.size());
			file.insert("isDir", fileInfo.isDir());

			m_files.append(file);
		}

		emit done();
	}

}

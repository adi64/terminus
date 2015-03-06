#pragma once
#include <QObject>

namespace terminus
{

class IPProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString localIP READ localIP NOTIFY localIPChanged())
public:
    explicit IPProvider(QObject *parent = 0);
    ~IPProvider();

    QString localIP() const;

signals:
    void localIPChanged();

public slots:
};

} // namespace terminus

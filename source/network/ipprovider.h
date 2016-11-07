#pragma once
#include <QObject>

namespace terminus
{

/*!
 * \brief A small helper class to determine the local IP address
 *
 * This class is used on QML side to display the local IP address when hosting
 * a multiplayer game.
 */
class IPProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString localIP READ localIP NOTIFY localIPChanged)
public:
    explicit IPProvider(QObject *parent = nullptr);
    ~IPProvider();

    QString localIP() const;

signals:
    void localIPChanged();

public slots:
};

} // namespace terminus

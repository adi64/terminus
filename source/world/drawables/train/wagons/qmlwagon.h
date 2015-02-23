#pragma once

#include <QString>
#include <QObject>

namespace terminus
{

class AbstractWagon;

/*!
 * \brief QMLWagon class is contains data from an AbstractWagon to be used in the UI
 */
class QMLWagon : public QObject
{

    Q_OBJECT
    Q_PROPERTY(bool isDisabled READ isDisabled NOTIFY isDisabledChanged)
    Q_PROPERTY(QString wagonType READ wagonType NOTIFY wagonTypeChanged)
    Q_PROPERTY(float currentHealth READ currentHealth NOTIFY currentHealthChanged)
    Q_PROPERTY(float maxHealth READ maxHealth NOTIFY maxHealthChanged)
    //Q_PROPERTY(float currentCooldown READ currentCooldown NOTIFY currentCooldownChanged)
    //Q_PROPERTY(float maxCooldown READ maxCooldown NOTIFY maxCooldownChanged)

public:
    QMLWagon(AbstractWagon *wagon = nullptr);
    ~QMLWagon();

    AbstractWagon *wagon() const;
    void setWagon(AbstractWagon *wagon);
    Q_INVOKABLE float maxHealth() const;
    Q_INVOKABLE float currentHealth() const;
    Q_INVOKABLE bool isDisabled() const;
    Q_INVOKABLE QString wagonType() const;

signals:
    void wagonChanged();
    void isDisabledChanged();
    void wagonTypeChanged();
    void currentHealthChanged();
    void maxHealthChanged();
    //void currentCooldownChanged();
    //void maxCooldownChanged();

protected:
    AbstractWagon *m_wagon;
};

}

#include "qmlwagon.h"

#include <assert.h>

#include "abstractwagon.h"

namespace terminus
{

QMLWagon::QMLWagon(AbstractWagon *wagon)
    : m_wagon(wagon)
{

}

QMLWagon::~QMLWagon()
{
}

AbstractWagon *QMLWagon::wagon() const
{
    return m_wagon;
}

void QMLWagon::setWagon(AbstractWagon *wagon)
{
    m_wagon = wagon;
}

float QMLWagon::maxHealth() const
{
    assert(m_wagon != nullptr);
    return m_wagon->maxHealth();
}

float QMLWagon::currentHealth() const
{
    assert(m_wagon != nullptr);
    return m_wagon->currentHealth();
}

bool QMLWagon::isDisabled() const
{
    assert(m_wagon != nullptr);
    return m_wagon->isDisabled();
}

QString QMLWagon::wagonType() const
{
    assert(m_wagon != nullptr);
    return m_wagon->wagonType();
}

}

#include "abstractplayer.h"

#include <assert.h>

#include <world/camera.h>
#include <world/drawables/train/wagons/abstractwagon.h>
#include <world/drawables/train/train.h>

namespace terminus
{

AbstractPlayer::AbstractPlayer(World & world, Train * train)
: m_world(world)
, m_train(train)
, m_selectedWagonIndex(0)
{
    assert(train);

    m_train->setPlayer(this);
    m_camera.bindTo(selectedWagon());
}

Camera & AbstractPlayer::camera()
{
    return m_camera;
}

void AbstractPlayer::toggleCameraLock()
{
    if(m_camera.isBound())
    {
        m_camera.unbind();
    }
    else
    {
        m_camera.bindTo(selectedWagon());
    }
}

unsigned int AbstractPlayer::selectedWagonIndex() const
{
    return m_selectedWagonIndex;
}

void AbstractPlayer::switchToNextWagon()
{
    if(m_selectedWagonIndex + 1 < m_train->size())
    {
        ++m_selectedWagonIndex;
        if(m_camera.isBound())
        {
            m_camera.bindTo(selectedWagon());
        }
    }
}

void AbstractPlayer::switchToPreviousWagon()
{
    if((m_selectedWagonIndex > 0))
    {
        --m_selectedWagonIndex;
        if(m_camera.isBound())
        {
            m_camera.bindTo(selectedWagon());
        }
    }
}

void AbstractPlayer::switchToWagon(unsigned int wagonIndex)
{
    assert(wagonIndex < m_train->size());
    m_selectedWagonIndex = wagonIndex;
}

void AbstractPlayer::primaryAction()
{
    m_train->wagonAt(m_selectedWagonIndex)->primaryAction();
}

void AbstractPlayer::primaryActionDebug()
{
    qDebug() << "sending primary action debug to wagon " << m_selectedWagonIndex;
    m_train->wagonAt(m_selectedWagonIndex)->primaryActionDebug();
}

void AbstractPlayer::update()
{
}

AbstractWagon * AbstractPlayer::selectedWagon()
{
    return m_train->wagonAt(m_selectedWagonIndex);
}

} // namespace terminus

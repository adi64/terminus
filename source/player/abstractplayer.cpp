#include "abstractplayer.h"

#include <cassert>

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
    bindCamera();
}

Camera & AbstractPlayer::camera()
{
    return m_camera;
}

void AbstractPlayer::bindCamera()
{
    m_camera.bindTo(selectedWagon());
}

void AbstractPlayer::releaseCamera()
{
    m_camera.unbind();
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

void AbstractPlayer::primaryAction()
{
    m_train->wagonAt(m_selectedWagonIndex)->primaryAction();
}

void AbstractPlayer::primaryActionDebug()
{
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

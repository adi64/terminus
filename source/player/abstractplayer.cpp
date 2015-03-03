#include "abstractplayer.h"

#include <world/camera.h>
#include <world/drawables/train/train.h>

namespace terminus
{

AbstractPlayer::AbstractPlayer(World & world, Train * train)
: m_world(world)
, m_train(train)
, m_selectedWagonIndex(0)
{
    m_train->setPlayer(this);
    //!!FIXME!!
    //m_camera.lockToObject(train->wagonAt(m_selectedWagonIndex));
}

Camera & AbstractPlayer::camera()
{
    return m_camera;
}

unsigned int AbstractPlayer::selectedWagonIndex() const
{
    return m_selectedWagonIndex;
}

void AbstractPlayer::switchToNextWagon()
{
    //!!FIXME!!
//    if(m_camera.isLocked() && (m_selectedWagonIndex + 1 < m_train->size()))
//    {
//        m_selectedWagonIndex++;
//        m_camera.lockToObject(m_train->wagonAt(m_selectedWagonIndex));
//    }
}

void AbstractPlayer::switchToPreviousWagon()
{
    //!!FIXME!!
//    if(m_camera.isLocked() && (m_selectedWagonIndex > 0))
//    {
//        m_selectedWagonIndex--;
//        m_camera.lockToObject(m_train->wagonAt(m_selectedWagonIndex));
//    }
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

} // namespace terminus

#include "abstractplayer.h"

#include <assert.h>

#include <QQuaternion>

#include <util/mathutil.h>
#include <world/camera.h>
#include <world/drawables/train/wagons/abstractwagon.h>
#include <world/drawables/train/train.h>

namespace terminus
{

AbstractPlayer::AbstractPlayer(World & world, Train * train)
: m_world(world)
, m_train(train)
, m_animationInProgress(false)
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

void AbstractPlayer::primaryAction()
{
    m_train->wagonAt(m_selectedWagonIndex)->primaryAction();
}

void AbstractPlayer::primaryActionDebug()
{
    m_train->wagonAt(m_selectedWagonIndex)->primaryActionDebug();
}

void AbstractPlayer::animateHit()
{
    if(m_animationInProgress)
    {
        m_world.timer().adjust(m_animationTimer, 0);
    }
    else
    {
        m_animationTimer = m_world.timer().allocateTimer();
    }
    m_animationInProgress = true;
}

void AbstractPlayer::update()
{
    const int animationTime = 750;
    if(m_animationInProgress)
    {
        int time = m_world.timer().get(m_animationTimer);

        auto progress = MathUtil::linstep(0, animationTime, time);
        auto angle = sin(progress * 10.f * MathUtil::PI) * MathUtil::mix(3.f, 0.f, progress);
        auto newUp = QQuaternion::fromAxisAndAngle(m_camera.normalizedLook(), angle).rotatedVector({0.f, 1.f, 0.f});
        m_camera.setUp(newUp);

        if(time > animationTime)
        {
            m_camera.setUp({0.f, 1.f, 0.f});
            m_world.timer().releaseTimer(m_animationTimer);
            m_animationInProgress = false;
        }
    }
}

AbstractWagon * AbstractPlayer::selectedWagon()
{
    return m_train->wagonAt(m_selectedWagonIndex);
}

} // namespace terminus

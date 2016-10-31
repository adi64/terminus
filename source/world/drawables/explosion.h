#pragma once

#include <memory>

#include <util/timer.h>
#include <world/drawables/abstractgraphicsobject.h>

class QVector3D;

namespace terminus
{

/*!
 * \brief The Explosion class renders an explosion effect
 *
 * Explosions will be spawned by Projectiles that collide with a Wagon
 *
 * \sa Projectile
 */
class Explosion : public AbstractGraphicsObject
{
public:
    Explosion(World & world, const QVector3D & position);
    virtual ~Explosion();

protected:
    virtual void localUpdate() override;

    virtual void localRenderSetup(QOpenGLFunctions & gl, Program & program) const override;
    virtual void localRenderCleanup(QOpenGLFunctions & gl, Program & program) const override;

protected:
    Timer::TimerID m_timer;
};

}

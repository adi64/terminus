#pragma once

#include <memory>

#include <util/timer.h>
#include <world/drawables/abstractgraphicsobject.h>

class QVector3D;

namespace terminus
{

/*!
 * \brief The Explosion class provides a renderable explosion effect, which is triggered when a projectile hits a weagon.
 *
 * It sole purpose is simulating an explosion without influencing any game logic.
 * The Explosion spawns at the position the weagon is hit by a projectile.
 */
class Explosion : public AbstractGraphicsObject
{
public:
    Explosion(World & world, const QVector3D & position);
    virtual ~Explosion();

protected:
    virtual void localUpdate() override;

    virtual void localRenderSetup(QOpenGLFunctions & gl, Program & program) const;
    virtual void localRenderCleanup(QOpenGLFunctions & gl, Program & program) const;

protected:
    Timer::TimerID m_timer;
};

}

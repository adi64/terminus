#pragma once

#include <memory>

#include <util/timer.h>
#include <world/drawables/abstractgraphicsobject.h>

class QVector3D;

namespace terminus
{

class Explosion : public AbstractGraphicsObject
{
public:
    Explosion(World & world, const QVector3D & position);
    virtual ~Explosion();

protected:
    virtual void localUpdate() override;

protected:
    Timer::TimerID m_timer;
};

}

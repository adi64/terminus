#pragma once

#include <memory>

#include <QVector3D>

#include <util/timer.h>
#include <world/camera.h>
#include <world/world.h>

namespace terminus
{

class AbstractWagon;
class Train;

/*!
 * \brief Base class for interacting with a train and a camera
 */
class AbstractPlayer
{
public:
    AbstractPlayer(World & world, Train * train);

    Camera & camera();

    Train * train();

    bool hasWon();
    bool hasLost();

    void toggleCameraLock();

    unsigned int selectedWagonIndex() const;
    void switchToNextWagon();
    void switchToPreviousWagon();
    void switchToWagon(unsigned int wagonIndex);
    virtual void primaryAction();
    virtual void primaryActionDebug();
    void primaryActionCharge(bool charge = true);

    virtual void animateHit();

    virtual void update();

protected:
    AbstractWagon * selectedWagon();

protected:
    World & m_world;

    Camera m_camera;
    Train *m_train;

    bool m_animationInProgress;
    Timer::TimerID m_animationTimer;

    unsigned int m_selectedWagonIndex;
};
}

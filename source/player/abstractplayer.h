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
 * \brief The AbstractPlayer class is the base class for interacting with a
 * train and a camera.
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

    /*!
     * \brief Triggers the primary action of the currently selected wagon
     */
    virtual void primaryAction();

    /*!
     * \brief Triggers the primary debug action of the currently selected wagon
     * (typically the primary action without cooldown limitations etc.)
     */
    virtual void primaryActionDebug();

    virtual void animateHit();

    virtual void update();

protected:
    AbstractWagon * selectedWagon();

protected:
    World & m_world;

    Camera m_camera;
    Train * m_train;

    bool m_animationInProgress;
    Timer::TimerID m_animationTimer;

    unsigned int m_selectedWagonIndex;
};
}

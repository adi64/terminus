#pragma once

#include <memory>

#include <QVector3D>

#include <world/camera.h>
#include <world/world.h>

namespace terminus
{

class Train;

/*!
 * \brief Base class for interacting with a train and a camera
 */
class AbstractPlayer
{
public:
    AbstractPlayer(World & world, Train * train);

    Camera & camera();
    unsigned int selectedWagonIndex() const;
    void switchToNextWagon();
    void switchToPreviousWagon();
    void primaryAction();
    void primaryActionDebug();
    void primaryActionCharge(bool charge = true);

    virtual void update();
protected:
    World & m_world;

    Camera m_camera;
    Train *m_train;

    unsigned int m_selectedWagonIndex;
};
}

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
    AbstractPlayer(World & world, std::shared_ptr<Train> train);

    Camera & camera();
    Train & train();
    unsigned int selectedWagonIndex() const;
    void switchToNextWagon();
    void switchToPreviousWagon();
    void primaryAction();
    void primaryActionDebug();

    virtual void update();

protected:
    World & m_world;

    Camera m_camera;
    std::shared_ptr<Train> m_train;

    unsigned int m_selectedWagonIndex;
};
}

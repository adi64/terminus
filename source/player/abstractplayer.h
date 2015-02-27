#pragma once

#include <memory>

#include <QVector3D>

#include <world/camera.h>

namespace terminus
{

class Train;

/*!
 * \brief Base class for interacting with a train and a camera
 */
class AbstractPlayer
{
public:
    AbstractPlayer(Train *train);

    Camera & camera();
    unsigned int selectedWagonIndex() const;
    void switchToNextWagon();
    void switchToPreviousWagon();
    void primaryAction();
    void primaryActionDebug();
    void primaryActionCharge(bool charge = true);

    virtual void update(int elapsedMilliseconds);

protected:
    Camera m_camera;
    Train *m_train;

    unsigned int m_selectedWagonIndex;
};
}

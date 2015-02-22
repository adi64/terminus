#pragma once

#include <memory>

#include <QVector3D>

#include <world/camera.h>

namespace terminus
{

class Train;
class NetworkEndpoint;

/*!
 * \brief Base class for interacting with a train and a camera
 */
class AbstractPlayer
{
public:
    AbstractPlayer(std::shared_ptr<Train> train);

    std::shared_ptr<Camera> camera();
    const Train &train();
    unsigned int selectedWagonIndex() const;
    void switchToNextWagon();
    void switchToPreviousWagon();
    void primaryAction();
    void primaryActionDebug();

    void setNetworkEndpoint(std::shared_ptr<NetworkEndpoint> networkEndpoint);

    virtual void update(int elapsedMilliseconds);

protected:
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<Train> m_train;

    std::shared_ptr<NetworkEndpoint> m_networkEndpoint;

    unsigned int m_selectedWagonIndex;
};
}

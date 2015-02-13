#pragma once

#include <memory>

#include <world/camera.h>

namespace terminus
{

class Train;

class AbstractPlayer
{
public:
    AbstractPlayer(std::shared_ptr<Train> train);

    Camera &camera();
    const Train &train();
    unsigned int selectedWagonIndex() const;

protected:
    Camera m_camera;
    std::shared_ptr<Train> m_train;

    unsigned int m_selectedWagonIndex;
};
}

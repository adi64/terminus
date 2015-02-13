#include "abstractplayer.h"

#include <world/camera.h>
#include <world/drawables/train/train.h>

terminus::AbstractPlayer::AbstractPlayer(std::shared_ptr<Train> train)
    : m_train(train)
    , m_selectedWagon(0)
{
    m_camera.lockToObject(train->wagonAt(m_selectedWagon));
}

terminus::Camera &terminus::AbstractPlayer::camera()
{
    return m_camera;
}

const terminus::Train &terminus::AbstractPlayer::train()
{
    return (*m_train);
}

unsigned int terminus::AbstractPlayer::selectedWagon() const
{
    return m_selectedWagon;
}

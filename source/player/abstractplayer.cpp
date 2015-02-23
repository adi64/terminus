#include "abstractplayer.h"

#include <world/camera.h>
#include <world/drawables/train/train.h>

terminus::AbstractPlayer::AbstractPlayer(std::shared_ptr<Train> train)
    : m_camera(std::shared_ptr<Camera>(new Camera))
    , m_train(train)
    , m_selectedWagonIndex(0)
{
    m_train->setPlayerCamera(m_camera);
    m_camera->bindTo(train->wagonAt(m_selectedWagonIndex));
}

std::shared_ptr<terminus::Camera> terminus::AbstractPlayer::camera()
{
    return m_camera;
}

const terminus::Train &terminus::AbstractPlayer::train()
{
    return (*m_train);
}

unsigned int terminus::AbstractPlayer::selectedWagonIndex() const
{
    return m_selectedWagonIndex;
}

void terminus::AbstractPlayer::switchToNextWagon()
{
    if(m_camera->isBound() && (m_selectedWagonIndex + 1 < m_train->size()))
    {
        m_selectedWagonIndex++;
        m_camera->bindTo(m_train->wagonAt(m_selectedWagonIndex));
    }
}

void terminus::AbstractPlayer::switchToPreviousWagon()
{
    if(m_camera->isBound() && (m_selectedWagonIndex > 0))
    {
        m_selectedWagonIndex--;
        m_camera->bindTo(m_train->wagonAt(m_selectedWagonIndex));
    }
}

void terminus::AbstractPlayer::primaryAction()
{
    m_train->wagonAt(m_selectedWagonIndex)->primaryAction();
}

void terminus::AbstractPlayer::primaryActionDebug()
{
    m_train->wagonAt(m_selectedWagonIndex)->primaryActionDebug();
}

void terminus::AbstractPlayer::update(int elapsedMilliseconds)
{
}

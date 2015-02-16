#include "abstractplayer.h"

#include <world/camera.h>
#include <world/drawables/train/train.h>

namespace terminus
{

AbstractPlayer::AbstractPlayer(std::shared_ptr<Train> train)
    : m_camera(std::shared_ptr<Camera>(new Camera))
    , m_train(train)
    , m_selectedWagonIndex(0)
{
    m_train->setPlayerCamera(m_camera);
    m_camera->lockToObject(train->wagonAt(m_selectedWagonIndex));
}

std::shared_ptr<Camera> AbstractPlayer::camera()
{
    return m_camera;
}

const Train &AbstractPlayer::train()
{
    return (*m_train);
}

unsigned int AbstractPlayer::selectedWagonIndex() const
{
    return m_selectedWagonIndex;
}

void AbstractPlayer::switchToNextWagon()
{
    if(m_camera->isLocked() && (m_selectedWagonIndex + 1 < m_train->size()))
    {
        m_selectedWagonIndex++;
        m_camera->lockToObject(m_train->wagonAt(m_selectedWagonIndex));
    }
}

void AbstractPlayer::switchToPreviousWagon()
{
    if(m_camera->isLocked() && (m_selectedWagonIndex > 0))
    {
        m_selectedWagonIndex--;
        m_camera->lockToObject(m_train->wagonAt(m_selectedWagonIndex));
    }
}

void AbstractPlayer::primaryAction()
{
    m_train->wagonAt(m_selectedWagonIndex)->primaryAction();
}

void AbstractPlayer::primaryActionDebug()
{
    m_train->wagonAt(m_selectedWagonIndex)->primaryActionDebug();
}

void AbstractPlayer::update(int elapsedMilliseconds)
{
    m_camera->update();
}

}

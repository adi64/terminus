#include "abstractplayer.h"

#include <world/camera.h>
#include <world/drawables/train/train.h>

terminus::AbstractPlayer::AbstractPlayer(std::shared_ptr<Train> train)
    : m_train(train)
    , m_selectedWagonIndex(0)
{
    m_camera.lockToObject(train->wagonAt(m_selectedWagonIndex));
}

terminus::Camera &terminus::AbstractPlayer::camera()
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
    if(m_camera.isLocked() && (m_selectedWagonIndex + 1 < m_train->size()))
    {
        m_selectedWagonIndex++;
        m_camera.lockToObject(m_train->wagonAt(m_selectedWagonIndex));
    }
}

void terminus::AbstractPlayer::switchToPreviousWagon()
{
    if(m_camera.isLocked() && (m_selectedWagonIndex > 0))
    {
        m_selectedWagonIndex--;
        m_camera.lockToObject(m_train->wagonAt(m_selectedWagonIndex));
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

QVector3D terminus::AbstractPlayer::normalizedAimVector() const
{
    return (m_camera.center() - m_camera.eye()).normalized();
}

void terminus::AbstractPlayer::update(int elapsedMilliseconds)
{
    m_camera.update();
}

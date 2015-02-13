#include "train.h"

#include <memory>
#include <QDebug>
#include <QOpenGLFunctions>

#include <world/drawables/track.h>
#include <world/drawables/train/wagons/enginewagon.h>
#include <util/mathutil.h>

namespace terminus
{
const float Train::base_velocity = 0.02;

Train::Train(std::shared_ptr<Scene> scene, Track *track, bool playerControlled)
    : AbstractGraphicsObject(scene)
    , m_hasEngine(false)
    , m_velocity(base_velocity)
    , m_travelledDistance(0.0f)
    , m_track(track)
    , m_playerControlled(playerControlled)
{
    // Every train needs an engine
    addWagon<EngineWagon>();
}

Train::~Train()
{

}

void Train::removeWagon(unsigned int index)
{
    if(index >= m_wagons.size())
    {
        qDebug() << index << " > " << m_wagons.size();
        return;
    }

    if(index <= 0)
    {
        qDebug() << "you cannot delete the engine!";
        return;
    }

    m_wagons.erase(m_wagons.begin() + index);

    calculateWagonOffset();
}

void Train::moveWagon(unsigned int wagonPos, unsigned int targetPos)
{
    if(wagonPos >= m_wagons.size())
    {
        qDebug() << "wagonPos >= " << m_wagons.size();
        return;
    }

    if(wagonPos <= 0)
    {
        qDebug() << "Cannot move engine";
        return;
    }

    if(targetPos >= m_wagons.size())
    {
        qDebug() << "targetPos >= " << m_wagons.size();
        return;
    }

    if(targetPos <= 0)
    {
        qDebug() << "Cannot move wagon in front of engine";
        return;
    }

    if(targetPos == wagonPos)
    {
        qDebug() << "Wagon did not move";
        return;
    }

    auto wagon = std::move(m_wagons.at(wagonPos));
    m_wagons.erase(m_wagons.begin() + wagonPos);

    if(targetPos > wagonPos)
    {
        m_wagons.insert(m_wagons.begin() + targetPos - 1, std::move(wagon));
    }
    else
    {
        m_wagons.insert(m_wagons.begin() + targetPos, std::move(wagon));
    }

    calculateWagonOffset();
}

void Train::update(int elapsedMilliseconds)
{
    if(m_followedTrain)
    {
        float dX = m_followedTrain->headPosition().x() - headPosition().x();
        m_velocity = MathUtil::mix(base_velocity * 0.5, base_velocity * 1.5, MathUtil::linstep(-50.f, 50.f, dX));
    }

    // move forward
    m_travelledDistance += m_velocity * elapsedMilliseconds;

    // TODO FIXME - this wraps the train
    if(m_travelledDistance > m_track->length())
    {
        m_travelledDistance = 0.0;
    }

    for(auto& wagon : m_wagons)
    {
        wagon->update(elapsedMilliseconds);
    }
}

void Train::render(QOpenGLFunctions& gl) const
{
    for(auto& wagon : m_wagons)
    {
        wagon->render(gl);
    }
}

AbstractWagon *Train::wagonAt(unsigned int index) const
{
    if(index >= m_wagons.size())
    {
        qDebug() << index << " > " << m_wagons.size();
        return nullptr;
    }

    return m_wagons.at(index).get();
}

Track *Train::track() const
{
    return m_track;
}

void Train::follow(std::shared_ptr<Train> train)
{
    m_followedTrain = train;
}

float Train::velocity() const
{
    return m_velocity;
}
void Train::setVelocity(float velocity)
{
    m_velocity = velocity;
}

float Train::travelledDistance() const
{
    return m_travelledDistance;
}

QVector3D Train::headPosition() const
{
    return m_track->positionAt(travelledDistance());
}

unsigned int Train::size() const
{
    return m_wagons.size();
}

bool Train::isPlayerControlled() const
{
    return m_playerControlled;
}

void Train::calculateWagonOffset()
{
    const auto wagonGap = 0.3f;

    float accumulatedOffset = -3.0f;

    for(auto& wagon : m_wagons)
    {
        accumulatedOffset += 0.5f * wagon->length() + wagonGap;
        wagon->setPositionOffset(accumulatedOffset);
        accumulatedOffset += 0.5f * wagon->length();
    }
}

}

#include "train.h"

#include <memory>

#include <assert.h>

#include <QDebug>
#include <QOpenGLFunctions>

#include <util/mathutil.h>
#include <util/timer.h>
#include <world/drawables/track.h>
#include <world/drawables/train/wagons/enginewagon.h>

namespace terminus
{
const float Train::base_velocity = 0.02;

Train::Train(World & world, Track * track)
: AbstractGraphicsObject(world)
, m_hasEngine(false)
, m_velocity(base_velocity)
, m_followedTrain(nullptr)
, m_travelledDistance(0.0f)
, m_track(track)
{
    // Every train needs an engine
    addWagon<EngineWagon>();
}

Train::~Train()
{

}

void Train::removeWagon(unsigned int index)
{
    assert(index < m_wagons.size());
    assert(index > 0);//can not delete engine at index 0

    m_wagons.erase(m_wagons.begin() + index);

    calculateWagonOffset();
}

void Train::moveWagon(unsigned int wagonPos, unsigned int targetPos)
{
    assert(wagonPos < m_wagons.size());
    assert(wagonPos > 0);//can not move engine at index 0
    assert(targetPos < m_wagons.size());
    assert(targetPos > 0);//can not replace engine at index 0
    if(targetPos == wagonPos)
    {
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

void Train::localUpdate()
{
    if(m_followedTrain)
    {
        float dX = m_followedTrain->headPosition().x() - headPosition().x();
        m_velocity = MathUtil::mix(base_velocity * 0.5, base_velocity * 1.5, MathUtil::linstep(-50.f, 50.f, dX));
    }

    // move forward
    m_travelledDistance += m_velocity * m_world.timer().get("frameTimer");

    // TODO FIXME - this wraps the train
    if(m_travelledDistance > m_track->length())
    {
        m_travelledDistance = 0.0;
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

void Train::follow(Train *train)
{
    assert(train);
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

void Train::setPlayer(AbstractPlayer *player)
{
    assert(player);
    m_player = player;
}

AbstractPlayer &Train::player() const
{
    assert(m_player);
    return *m_player;
}

bool Train::localRenderEnabled() const
{
    return false;
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

void Train::doForAllChildren(std::function<void (AbstractGraphicsObject &)> callback)
{
    for(auto & wagon : m_wagons)
    {
        if(wagon)
        {
            callback(*wagon);
        }
    }
}

}

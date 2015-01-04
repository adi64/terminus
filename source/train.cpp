#include "train.h"

#include <memory>
#include <QDebug>
#include <QOpenGLFunctions>

#include "wagons/enginewagon.h"

namespace terminus
{


Train::Train(Scene *scene, Track *track)
    : AbstractGraphicsObject(scene)
    , m_hasEngine(false)
    , m_velocity(0.02)
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
    // move forward
    m_travelledDistance += m_velocity * elapsedMilliseconds;

    // TODO FIXME - this wraps the train
    if(m_travelledDistance > 400.0)
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

float Train::travelledDistance() const
{
    return m_travelledDistance;
}

unsigned int Train::size() const
{
    return m_wagons.size();
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

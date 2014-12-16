#pragma once

#include <memory>
#include <vector>

#include "abstractgraphicsobject.h"
#include "wagons/abstractwagon.h"
#include "scene.h"

namespace terminus
{

class Track;

class Train : public AbstractGraphicsObject
{
public:
    Train(Scene *scene, Track *track);
    ~Train();

    template<typename WagonType> void addWagon();
    template<typename WagonType> void insertWagon(int targetPos);

    void removeWagon(unsigned int index);
    void moveWagon(unsigned int wagonPos, unsigned int targetPos);

    void render(QOpenGLFunctions &gl, int elapsedMilliseconds);

    Track *track() const;
    float travelledDistance() const;

    void shoot();

private:
    void calculateWagonOffset();

    std::vector<std::unique_ptr<AbstractWagon>> m_wagons;
    bool m_isRunning;
    bool m_hasEngine;
    float m_velocity;
    float m_travelledDistance;
    Track *m_track;
};

}

#include "train.hpp"

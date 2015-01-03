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

    void update(int elapsedMilliseconds);
    void render(QOpenGLFunctions &gl) const override;

    AbstractWagon *wagonAt(unsigned int index) const;
    Track *track() const;
    float travelledDistance() const;

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

#pragma once

#include <memory>
#include <vector>
#include "wagons/abstractwagon.h"
#include "scene.h"

namespace terminus
{

class Train
{
public:
    Train(Scene *scene);
    ~Train();

    template<typename WagonType> void addWagon();
    template<typename WagonType> void insertWagon(int targetPos);

    void removeWagon(int index);
    void moveWagon(int wagonPos, int targetPos);
private:
    void calculateWagonOffset();

    std::vector<std::unique_ptr<AbstractWagon>> m_wagons;
    bool m_isRunning;
    bool m_hasEngine;
    float m_velocity;
    float m_travelledDistance;
    Scene *m_scene;
};

}

#include "train.hpp"

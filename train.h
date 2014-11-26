#pragma once

#include <memory>
#include <vector>
#include "abstractwagon.h"

namespace terminus
{

class Train
{
public:
    Train();
    ~Train();

    template<typename WagonType> void addWagon();
    template<typename WagonType> void insertWagon(int targetPos);

    void removeWagon(int index);
    void moveWagon(int wagonPos, int targetPos);
private:
    void calculateWagonOffset();

    std::vector<std::unique_ptr<AbstractWagon>> m_wagons;
    bool m_isRunning;
    float m_velocity;
    float m_travelledDistance;
};

}

#include "train.hpp"

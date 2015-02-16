#pragma once

#include "abstractwagon.h"

namespace terminus
{

class EngineWagon : public AbstractWagon
{


public:
    EngineWagon(std::shared_ptr<Scene> scene, Train *train);
    virtual ~EngineWagon();

    float length() const;

    void playSound() const;
};

}

#pragma once

#include "abstractwagon.h"

namespace terminus
{

class EngineWagon : public AbstractWagon
{
public:
    EngineWagon();
    void render();
    static float length();
};

}

#pragma once

#include <QMatrix4x4>

#include <world/postprocessing/abstracteffect.h>

namespace terminus
{

class Vignette : public AbstractEffect
{
public:
    Vignette(DisabledBehaviour behaviour = DisabledBehaviour::PASS);
    virtual ~Vignette();

protected:
};

}

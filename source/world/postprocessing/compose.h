#pragma once

#include <QMatrix4x4>

#include <world/postprocessing/abstracteffect.h>

namespace terminus
{

class Compose : public AbstractEffect
{
public:
    Compose(World & world, DisabledBehaviour behaviour = DisabledBehaviour::PASS);
    virtual ~Compose();

    void localRenderSetup(Program & program) const override;
protected:
};

}

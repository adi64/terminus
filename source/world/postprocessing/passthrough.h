#pragma once

#include <QMatrix4x4>

#include <world/postprocessing/abstracteffect.h>

namespace terminus
{

class Passthrough : public AbstractEffect
{
public:
    Passthrough(const World & world, DisabledBehaviour behaviour = DisabledBehaviour::PASS);
    virtual ~Passthrough();

    void localRenderSetup(const Camera & camera, Program & program) const override;
protected:
};

}

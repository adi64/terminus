#pragma once

#include <QMatrix4x4>

#include <world/postprocessing/abstracteffect.h>

namespace terminus
{

class Passthrough : public AbstractEffect
{
public:
    Passthrough(DisabledBehaviour behaviour = DisabledBehaviour::PASS);
    virtual ~Passthrough();

    void localRenderSetup(Program & program) const override;
protected:
};

}

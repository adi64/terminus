#pragma once

#include <QMatrix4x4>

#include <world/postprocessing/abstracteffect.h>

namespace terminus
{

class Compose : public AbstractEffect
{
public:
    Compose(const World & world, DisabledBehaviour behaviour = DisabledBehaviour::PASS);
    virtual ~Compose();

    virtual unsigned int requiredNumberOfInputTextures() const override { return 5; }

    void localRenderSetup(Program & program) const override;
protected:
};

}

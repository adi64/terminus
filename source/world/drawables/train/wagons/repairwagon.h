#pragma once

#include "abstractwagon.h"

namespace terminus
{

class RepairWagon : public AbstractWagon
{
public:
    RepairWagon(World & world, Train * train);
    virtual ~RepairWagon();

    virtual void primaryAction() override;
    float cooldownRate() const override;

    void localUpdate() override;
    void localRenderSetup(QOpenGLFunctions& gl, Program & program) const override;
    float length() const override;
    WagonType wagonType() const override;
};

}

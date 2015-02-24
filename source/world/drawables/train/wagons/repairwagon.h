#pragma once

#include "abstractwagon.h"

namespace terminus
{

class RepairWagon : public AbstractWagon
{
public:
    RepairWagon(World & world, Train * train);
    virtual ~RepairWagon();

    void primaryAction() override;

    void preRender(QOpenGLFunctions& gl, Program & program) const override;
    float length() const;
};

}

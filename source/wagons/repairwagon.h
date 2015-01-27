#pragma once

#include "abstractwagon.h"

namespace terminus
{

class RepairWagon : public AbstractWagon
{
public:
    RepairWagon(std::shared_ptr<Scene> scene, Train *train);
    virtual ~RepairWagon();

    void primaryAction() override;

    void preRender(QOpenGLFunctions& gl, Program & program) const override;
    float length() const;
};

}

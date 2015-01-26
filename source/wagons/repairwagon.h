#pragma once

#include "abstractwagon.h"

namespace terminus
{

class RepairWagon : public AbstractWagon
{
public:
    RepairWagon(std::shared_ptr<Scene> scene, Train *train);

    void primaryAction() override;

    void render(QOpenGLFunctions& gl) const override;
    float length() const;
};

}

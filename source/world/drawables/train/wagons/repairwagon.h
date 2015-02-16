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

    float length() const;
};

}

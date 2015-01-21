#pragma once

#include "abstractwagon.h"

namespace terminus
{

class WeaponWagon : public AbstractWagon
{
public:
    WeaponWagon(std::shared_ptr<Scene> scene, Train *train);

    void primaryAction() override;
    void setAccumulate(bool accumulate);

    void update(int elapsedMilliseconds) override;
    void render(QOpenGLFunctions& gl) const override;
    float length() const;

protected:
    int m_elapsedMilliseconds;
    bool m_accumulate;
    QVector3D m_force;
};

}

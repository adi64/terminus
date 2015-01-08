#pragma once

#include <memory>

#include <bullet/btBulletDynamicsCommon.h>

#include "abstractgraphicsobject.h"

namespace terminus
{

class Scene;

class AbstractPhysicsObject : public AbstractGraphicsObject
{
public:
    AbstractPhysicsObject(Scene* scene);
    ~AbstractPhysicsObject();

    virtual void update(int elapsedMilliseconds) override;

    void applyForce(const QVector3D &force);

    virtual void setPosition(const QVector3D& newPosition) override;
    virtual QVector3D position() const override;
    virtual QVector3D eulerAngles() const override;
protected:
    std::unique_ptr<btRigidBody> m_bullet_rigidBody;
};

}

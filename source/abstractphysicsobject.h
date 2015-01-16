#pragma once

#include <memory>

#include <bullet/btBulletDynamicsCommon.h>

#include "abstractgraphicsobject.h"

namespace terminus
{

class AbstractPhysicsObject : public AbstractGraphicsObject
{
public:
    AbstractPhysicsObject(std::shared_ptr<Scene> scene);
    ~AbstractPhysicsObject();

    virtual void moveTo(const QVector3D & newPosition);

protected:
    std::unique_ptr<btRigidBody> m_btRigidBody;
    std::unique_ptr<btCollisionShape> m_btCollisionShape;
};

}

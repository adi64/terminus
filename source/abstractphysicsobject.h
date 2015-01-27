#pragma once

#include <memory>

#include "bullet/btBulletDynamicsCommon.h"

#include "abstractgraphicsobject.h"

namespace terminus
{

class AbstractPhysicsObject : public AbstractGraphicsObject
{    
public:
    AbstractPhysicsObject(std::shared_ptr<Scene> scene);

    virtual void moveTo(const QVector3D & newPosition);

protected:
    virtual void initializePhysics(btCollisionShape * collisionShape, btScalar mass);
    virtual void deallocatePhysics();

protected:
    std::unique_ptr<btRigidBody> m_btRigidBody;
};

}

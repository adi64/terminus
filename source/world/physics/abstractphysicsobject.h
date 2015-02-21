#pragma once

#include <memory>

#include <bullet/btBulletDynamicsCommon.h>

#include <world/drawables/abstractgraphicsobject.h>

namespace terminus
{

class AbstractPhysicsObject : public AbstractGraphicsObject
{    
public:
    AbstractPhysicsObject(std::shared_ptr<Scene> scene);

    virtual void moveTo(const QVector3D & newPosition);
    virtual void onCollisionWith(AbstractPhysicsObject* other);

protected:
    virtual void initializePhysics(btCollisionShape * collisionShape, btScalar mass);
    virtual void deallocatePhysics();

protected:
    std::unique_ptr<btRigidBody> m_btRigidBody;
};

}
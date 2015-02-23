#pragma once

#include "abstractphysicsobject.h"

namespace terminus
{

class DynamicPhysicsObject : public AbstractPhysicsObject
{
public:
    DynamicPhysicsObject(std::shared_ptr<Scene> scene);

    virtual void localUpdate(int elapsedMilliseconds) override;

    /*!
     * \brief Apply central force
     * \param force
     * This is just a wrapper around btRigidBody::applyCentralForce(const btVector3 &force)
     */
    virtual void applyForce(const QVector3D & force);

    /*!
     * \brief Set the object's velocity, discarding any previously applied forces
     * \param velocity
     * This is just a wrapper around btRigidBody::setLinearVelocity(const btVector3 &velocity)
     */
    virtual void setLinearVelocity(const QVector3D & velocity);
};

}

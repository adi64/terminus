#pragma once

#include "abstractphysicsobject.h"

namespace terminus
{

class DynamicPhysicsObject : public AbstractPhysicsObject
{
public:
    DynamicPhysicsObject(World & world);

    virtual void localUpdate() override;

    /*!
     * \brief Set the object's velocity, discarding any previously applied forces
     * \param velocity
     *
     * This is just a wrapper around btRigidBody::setLinearVelocity(const btVector3 &velocity)
     */
    virtual void setLinearVelocity(const QVector3D & velocity);

    /*!
     * \brief Add a velocity to the object's own velocity
     * \param velocity
     *
     * This is a wrapper around btRigidBody::setLinearVelocity with the change that it can add velocities
     */
    virtual void addLinearVelocity(const QVector3D & velocity);
};

}

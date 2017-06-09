#include "camera.h"

#include <cassert>
#include <cmath>

#include <QQuaternion>
#include <QVector3D>

#include <render/camerastate.h>
#include <util/mathutil.h>
#include <world/drawables/gameobject.h>

namespace terminus
{

Camera::Camera()
: m_state()
, m_associatedObject(nullptr)
{
}

Camera::~Camera()
{
}

const CameraMatrices & Camera::state() const
{
    return m_state;
}

CameraMatrices & Camera::state()
{
    return m_state;
}

void Camera::moveEvent(QVector3D movement)
{
    if(!m_associatedObject)
    {
        auto direction = m_state.normalizedLook();
        auto newEye = m_state.eye();
        auto newCenter = m_state.center();
        auto normal = QVector3D::normal(direction, m_state.up());

        newEye += normal * movement.x();
        newCenter += normal * movement.x();

        newEye += m_state.up() * movement.y();
        newCenter += m_state.up() * movement.y();

        newEye += direction * -movement.z();
        newCenter += direction * -movement.z();

        m_state.setEye(newEye);
        m_state.setCenter(newCenter);
    }
    else
    {
        m_associatedObject->moveEvent(movement);
    }
}

void Camera::rotateEvent(QVector2D rotation)
{
    auto viewDirection = (m_state.center() - m_state.eye()).normalized();
    auto viewNormal = QVector3D::normal(viewDirection, m_state.up());
    // "x rotation" -> rotate around up vector
    auto rotation_x = QQuaternion::fromAxisAndAngle(m_state.up(), -rotation.x());
    // "y rotation" -> rotation around "the vector pointing to the right"
    auto rotation_y = QQuaternion::fromAxisAndAngle(viewNormal, -rotation.y());
    auto rotation_total = rotation_x * rotation_y;

    if(!m_associatedObject)
    {
        auto newCenter = m_state.eye() + rotation_total.rotatedVector(viewDirection);
        m_state.setCenter(newCenter);
    }
    else
    {
        m_associatedObject->rotateEvent(rotation);
    }
}

void Camera::bindTo(GameObject * object)
{
    if(m_associatedObject)
    {
        m_associatedObject->unbindCamera(this);
    }

    m_associatedObject = object;

    if(m_associatedObject)
    {
        m_associatedObject->bindCamera(this);
    }
}

void Camera::unbind()
{
    bindTo(nullptr);
}

void Camera::unbound(GameObject * object)
{
    if(m_associatedObject == object)
    {
        m_associatedObject = nullptr;
    }
}

bool Camera::isBound() const
{
    return m_associatedObject;
}

} // namespace terminus

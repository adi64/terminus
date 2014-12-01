#include "camera.h"
#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>


using namespace glm;
namespace terminus
{


Camera::Camera(
    const vec3 & eye
,   const vec3 & center
,   const vec3 & up)
: m_dirty(false)
, m_autoUpdate(false)

, m_eye(eye)
, m_center(center)
, m_up(up)

, m_fovy(radians(40.f))
, m_aspect(1.f)
, m_zNear(0.1f)
, m_zFar(64.0f)
, m_viewChanged(true)
, m_viewInvertedChanged(true)
, m_projectionChanged(true)
, m_projectionInvertedChanged(true)
, m_viewProjectionChanged(true)
, m_viewProjectionInvertedChanged(true)
, m_normalChanged(true)
{
}

Camera::~Camera()
{
}

bool Camera::autoUpdating() const
{
    return m_autoUpdate;
}

void Camera::setAutoUpdating(bool b)
{
    m_autoUpdate = b;

    if (m_dirty && m_autoUpdate)
    {
        update();
    }
}

void Camera::invalidateMatrices() const
{
    m_viewChanged = true;
    m_viewInvertedChanged = true;
    m_projectionChanged = true;
    m_projectionInvertedChanged = true;
    m_viewProjectionChanged = true;
    m_viewProjectionInvertedChanged = true;
    m_normalChanged = true;
}

void Camera::dirty(bool update)
{
    m_dirty = true;

    if (update || m_autoUpdate)
        this->update();
}

const vec3 & Camera::eye() const
{
    return m_eye;
}

void Camera::setEye(const vec3 & eye)
{
    if (eye == m_eye)
        return;

    m_eye = eye;
    dirty();
}

const vec3 & Camera::center() const
{
    return m_center;
}

void Camera::setCenter(const vec3 & center)
{
    if (center == m_center)
        return;

    m_center = center;
    dirty();
}

const vec3 & Camera::up() const
{
    return m_up;
}

void Camera::setUp(const vec3 & up)
{
    if (up == m_up)
        return;

    m_up = up;
    dirty();
}

float Camera::zNear() const
{
    return m_zNear;
}

void Camera::setZNear(const float zNear)
{
    if (std::abs(zNear - m_zNear) < std::numeric_limits<float>::epsilon())
        return;

    m_zNear = zNear;
    assert(m_zNear > 0.0);

    dirty();
}

float Camera::zFar() const
{
    return m_zFar;
}

void Camera::setZFar(const float zFar)
{
    if (std::abs(zFar - m_zFar) < std::numeric_limits<float>::epsilon())
        return;

    m_zFar = zFar;
    assert(m_zFar > m_zNear);

    dirty();
}

float Camera::fovy() const
{
    return m_fovy;
}

void Camera::setFovy(const float fovy)
{
    if (std::abs(fovy - m_fovy) < std::numeric_limits<float>::epsilon())
        return;

    m_fovy = fovy;
    assert(m_fovy > 0.0);

    dirty();
}

const ivec2 & Camera::viewport() const
{
    return m_viewport;
}

void Camera::setViewport(int width, int height)
{
    return setViewport(ivec2(width, height));
}

void Camera::setViewport(const ivec2 & viewport)
{
    if (viewport == m_viewport)
        return;

    m_aspect = static_cast<float>(viewport.x) / max(static_cast<float>(viewport.y), 1.f);
    m_viewport = viewport;

    dirty();
}

float Camera::aspectRatio() const
{
    return m_aspect;
}

void Camera::update() const
{
    if (!m_dirty)
        return;

    invalidateMatrices();

    m_dirty = false;
}

const mat4 & Camera::view() const
{
    if (m_dirty)
        update();

    if (m_viewChanged)
        m_view = lookAt(m_eye, m_center, m_up);

    m_viewChanged = false;

    return m_view;
}

const mat4 & Camera::projection() const
{
    if (m_dirty)
        update();

    if (m_projectionChanged)
        m_projection = perspective(m_fovy, m_aspect, m_zNear, m_zFar);

    m_projectionChanged = false;

    return m_projection;
}

const mat4 & Camera::viewProjection() const
{
    if (m_dirty)
        update();

    if (m_viewProjectionChanged)
        m_viewProjection = projection() * view();

    m_viewProjectionChanged = false;

    return m_viewProjection;
}

const mat4 & Camera::viewInverted() const
{
    if (m_dirty)
        update();

    if (m_viewInvertedChanged)
        m_viewInverted = inverse(view());

    m_viewInvertedChanged = false;

    return m_viewInverted;
}

const mat4 & Camera::projectionInverted() const
{
    if (m_dirty)
        update();

    if (m_projectionInvertedChanged)
        m_projectionInverted = inverse(projection());

    m_projectionInvertedChanged = false;

    return m_projectionInverted;
}

const mat4 & Camera::viewProjectionInverted() const
{
    if (m_dirty)
        update();

    if (m_viewProjectionInvertedChanged)
        m_viewProjectionInverted = inverse(viewProjection());

    m_viewProjectionInvertedChanged = false;

    return m_viewProjectionInverted;
}

const mat3 & Camera::normal() const
{
    if (m_dirty)
        update();

    if (m_normalChanged)
        m_normal = inverseTranspose(mat3(view()));

    m_normalChanged = false;

    return m_normal;
}


} // namespace terminus

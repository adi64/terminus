#pragma once


#include <glm/glm.hpp>

namespace terminus
{

class Camera
{


public:
    Camera(const glm::vec3 & eye    = glm::vec3(0.0, 0.0, 1.0),
           const glm::vec3 & center = glm::vec3(0.0, 0.0, 0.0),
           const glm::vec3 & up     = glm::vec3(0.0, 1.0, 0.0) );

    virtual ~Camera();

    bool autoUpdating() const;
    void setAutoUpdating(bool b);

    const glm::vec3 & eye() const;
    void setEye(const glm::vec3 & eye);
    const glm::vec3 & center() const;
    void setCenter(const glm::vec3 & center);
    const glm::vec3 & up() const;
    void setUp(const glm::vec3 & up);

    float zNear() const;
    void setZNear(float zNear);
    float zFar() const;
    void setZFar(float zFar);

    float fovy() const;
    void setFovy(float fovy);

    const glm::ivec2 & viewport() const;
    void setViewport(const glm::ivec2 & viewport);
    void setViewport(int width, int height);

    float aspectRatio() const;

    // lazy matrices getters

    const glm::mat4 & view() const;
    const glm::mat4 & projection() const;
    const glm::mat4 & viewProjection() const;
    const glm::mat4 & viewInverted() const;
    const glm::mat4 & projectionInverted() const;
    const glm::mat4 & viewProjectionInverted() const;

    const glm::mat3 & normal() const;

    void update() const;

    void changed();


protected:
    void dirty(bool update = true);
    void invalidateMatrices() const;


protected:
    mutable bool m_dirty;
    bool m_autoUpdate;

    glm::vec3 m_eye;
    glm::vec3 m_center;
    glm::vec3 m_up;

    float m_fovy;
    float m_aspect;
    float m_zNear;
    float m_zFar;

    glm::ivec2 m_viewport;

    mutable glm::mat4 m_view;
    mutable glm::mat4 m_viewInverted;
    mutable glm::mat4 m_projection;
    mutable glm::mat4 m_projectionInverted;
    mutable glm::mat4 m_viewProjection;
    mutable glm::mat4 m_viewProjectionInverted;
    mutable glm::mat3 m_normal;

    mutable bool m_viewChanged;
    mutable bool m_viewInvertedChanged;
    mutable bool m_projectionChanged;
    mutable bool m_projectionInvertedChanged;
    mutable bool m_viewProjectionChanged;
    mutable bool m_viewProjectionInvertedChanged;
    mutable bool m_normalChanged;

};


} // namespace terminus

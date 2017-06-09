#pragma once

#include <cstdint>
#include <memory>
#include <set>
#include <vector>

#include <QSize>

#include <render/cameramatrices.h>
#include <resources/framebufferobject.h>
#include <world/drawables/gameobject.h>
#include <world/light.h>
#include <world/postprocessing/abstracteffect.h>
#include <world/postprocessing/compose.h>
#include <world/postprocessing/passthrough.h>

namespace terminus
{

class Renderer
{
    static constexpr uint8_t LightCount = 8;

public:
    Renderer();

    void registerObject(const GameObject * object);
    void deregisterObject(const GameObject * object);

    Light * allocateLight();
    Light * allocateLight(const Light & preset);
    void deallocateLight(Light * light);
    uint8_t availableLights() const;

    void resize(const QSize & viewport);
    void render(const CameraMatrices & cam) const;

protected:


    void renderObject(const GameObject * obj, const CameraMatrices & cam) const;

    void applyEffect(AbstractEffect * effect, FrameBufferObject * sourceFBO, FrameBufferObject * targetFBO);


protected:
    QSize m_viewport;

    std::set<const GameObject *> m_gameObjects;
    
    Light m_lights[LightCount];
    bool m_lightsOccupied[LightCount];

    FrameBufferObject m_frameBufferObject;
    std::unique_ptr<Compose> m_compose;
    std::unique_ptr<Passthrough> m_passthrough;
    std::vector<std::unique_ptr<AbstractEffect>> m_effects;
    std::vector<std::unique_ptr<FrameBufferObject>> m_effectFBOs;
};

}

#pragma once

#include <world/drawables/abstractgraphicsobject.h>
#include <resources/resourcemanager.h>
#include <world/world.h>
#include <world/camera.h>
#include <world/lightmanager.h>

namespace terminus
{

class AbstractEffect
{
public:

    enum class DisabledBehaviour
    {
        PASS,
        SET_GREY,
        SET_WHITE,
        SET_BLACK
    };

    AbstractEffect(DisabledBehaviour behaviour = DisabledBehaviour::PASS);
    virtual ~AbstractEffect();

    void render(const Camera & camera, const LightManager & lightManager) const;

    virtual void localRenderSetup(Program & program) const;
    virtual void localRenderCleanup(Program & program) const;

    void enable(bool enabled = true);
    void disable();
    bool isEnabled() const;

protected:
    void applyDisabled() const;

protected:
    DisabledBehaviour m_disabledBehaviour;
    bool m_enabled;

    std::shared_ptr<std::unique_ptr<Program>> m_program;
    std::shared_ptr<std::unique_ptr<Geometry>> m_geometry;
};

}

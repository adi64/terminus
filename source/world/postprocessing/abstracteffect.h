#pragma once

#include <world/drawables/abstractgraphicsobject.h>
#include <resources/resourcemanager.h>
#include <world/world.h>

class QOpenGLFunctions;

namespace terminus
{

class AbstractEffect : public AbstractGraphicsObject
{
public:

    enum class DisabledBehaviour
    {
        PASS,
        SET_GREY,
        SET_WHITE,
        SET_BLACK
    };

    AbstractEffect(World & world, DisabledBehaviour behaviour = DisabledBehaviour::PASS);
    virtual ~AbstractEffect();

    virtual void localRenderSetup(Program & program) const override;
    virtual void localRenderCleanup(Program & program) const override;

    void enable(bool enabled = true);
    void disable();
    bool isEnabled() const;

protected:
    void applyDisabled() const;

    DisabledBehaviour m_disabledBehaviour;

    bool m_enabled;
};

}

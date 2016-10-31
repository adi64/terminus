#pragma once

#include <world/drawables/gameobject.h>
#include <resources/resourcemanager.h>
#include <world/game.h>

namespace terminus
{

class AbstractEffect : public GameObject
{
public:

    enum class DisabledBehaviour
    {
        PASS,
        SET_GREY,
        SET_WHITE,
        SET_BLACK
    };

    AbstractEffect(Game & world, DisabledBehaviour behaviour = DisabledBehaviour::PASS);
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

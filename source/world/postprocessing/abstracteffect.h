#pragma once

class QOpenGLFunctions;
class World;

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

    AbstractEffect(World & world, DisabledBehaviour behaviour = DisabledBehaviour::PASS);
    virtual ~AbstractEffect();

    void apply(QOpenGLFunctions & gl) const final;
    virtual void internalApply(QOpenGLFunctions & gl) const = 0;

    void enable(bool enabled = true);
    void disable();
    bool isEnabled() const;

protected:
    void applyDisabled() const;

    World & m_world;
    DisabledBehaviour m_disabledBehaviour;

    bool m_enabled;
};

}

#include "abstracteffect.h"

#include <util/gldebug.h>
#include <world/world.h>
#include <world/camera.h>

namespace terminus
{

AbstractEffect::AbstractEffect(DisabledBehaviour behaviour)
: m_disabledBehaviour(behaviour)
, m_enabled(true)
{
    m_geometry = ResourceManager::getInstance()->getGeometry("base_squad");
}

AbstractEffect::~AbstractEffect()
{

}

void AbstractEffect::render(const Camera & camera, const LightManager & lightManager) const
{
    if(!m_geometry || !*m_geometry)
    {
        qDebug() << "Geometry to render does not exist!";
        return;
    }
    if(!m_program || !*m_program)
    {
        qDebug() << "Program to render with does not exist!";
        return;
    }

    Geometry & geometry = **m_geometry;
    Program & program = **m_program;

    printGlError(__FILE__, __LINE__);
    program.bind();

    printGlError(__FILE__, __LINE__);

    camera.setUniforms(program, QMatrix4x4());
    lightManager.setUniforms(program);
    printGlError(__FILE__, __LINE__);

    localRenderSetup(program);
    printGlError(__FILE__, __LINE__);

    geometry.draw();
    printGlError(__FILE__, __LINE__);

    localRenderCleanup(program);
    printGlError(__FILE__, __LINE__);

    program.release();
    printGlError(__FILE__, __LINE__);
}

void AbstractEffect::localRenderSetup(Program & /*program*/) const
{
}
void AbstractEffect::localRenderCleanup(Program & /*program*/) const
{
}

void AbstractEffect::enable(bool enabled)
{
    m_enabled = enabled;
}

void AbstractEffect::disable()
{
    m_enabled = false;
}

bool AbstractEffect::isEnabled() const
{
    return m_enabled;
}

void AbstractEffect::applyDisabled() const
{
    qDebug() << __FUNCTION__ << " stub!";
}

} //namespace terminus

#include "abstracteffect.h"

#include <cassert>

#include <util/gldebug.h>
#include <world/world.h>
#include <world/camera.h>

namespace terminus
{

AbstractEffect::AbstractEffect(const World &world, DisabledBehaviour behaviour)
: m_disabledBehaviour(behaviour)
, m_enabled(true)
, m_targetFBO(world.viewport())
{
    m_geometry = ResourceManager::getInstance()->getGeometry("base_squad");
}

AbstractEffect::~AbstractEffect()
{

}

void AbstractEffect::setTextures(std::initializer_list<FboAttachment> inputTextureList)
{
    assert(inputTextureList.size() == requiredNumberOfInputTextures());

    m_sourceAttachments.clear();

    for (const FboAttachment & attachment : inputTextureList)
    {
        assert(attachment.frameBufferObject.hasColorAttachment(attachment.colorAttachmentNumber));

        m_sourceAttachments.push_back(attachment);
    }
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

    // Bind source textures
    assert(m_sourceAttachments.size() == requiredNumberOfInputTextures());
    unsigned int textureUnit = 0;
    for (const FboAttachment & fboAttachment : m_sourceAttachments)
    {
        fboAttachment.frameBufferObject.bindTexture(fboAttachment.colorAttachmentNumber, GL_TEXTURE0 + textureUnit);
        printGlError(__FILE__, __LINE__);
        textureUnit++;
    }

    // Bind target FBO
    m_targetFBO.bindFBO();
    printGlError(__FILE__, __LINE__);

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

    // Release target FBO
    m_targetFBO.releaseFBO();

    // Release source textures
    for (textureUnit = 0; textureUnit < m_sourceAttachments.size(); textureUnit++)
    {
        m_sourceAttachments[textureUnit].frameBufferObject.releaseTexture(GL_TEXTURE0 + textureUnit);
        printGlError(__FILE__, __LINE__);
    }
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

const FrameBufferObject &AbstractEffect::targetFBO() const
{
    return m_targetFBO;
}

void AbstractEffect::applyDisabled() const
{
    qDebug() << __FUNCTION__ << " stub!";
}

} //namespace terminus

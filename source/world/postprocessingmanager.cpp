#include "postprocessingmanager.h"

#include <QImage>

#include <resources/resourcemanager.h>
#include <resources/program.h>
#include <world/world.h>
#include <player/localplayer.h>

namespace terminus
{

PostprocessingManager::PostprocessingManager(World &world)
: AbstractGraphicsObject(world)
, m_objectsOnGPU(false)
{
    m_program = ResourceManager::getInstance()->getProgram("postprocessing");
    m_geometry = ResourceManager::getInstance()->getGeometry("base_squad");
}

void PostprocessingManager::localRenderSetup(QOpenGLFunctions & gl, Program & /*program*/) const
{
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glEnable(GL_TEXTURE_2D);
    gl.glBindTexture(GL_TEXTURE_2D, m_fboTexture);
}
void PostprocessingManager::localRenderCleanup(QOpenGLFunctions & gl, Program & /*program*/) const
{
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glBindTexture(GL_TEXTURE_2D, 0);
}

void PostprocessingManager::beforeRenderHook(QOpenGLFunctions &gl) const
{
    allocateFBO(gl);

    // switch render target
    gl.glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void PostprocessingManager::afterRenderHook(QOpenGLFunctions &gl) const
{
    // switch back to "real" framebuffer
    gl.glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void PostprocessingManager::allocateFBO(QOpenGLFunctions &gl) const
{
    if(m_objectsOnGPU)
        return;

    m_fbo = -1;
    m_fboTexture = -1;
    m_rboDepth = -1;

    int windowWidth = m_world.localPlayer().camera().viewport().x();
    int windowHeight = m_world.localPlayer().camera().viewport().y();

    // Texture
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glGenTextures(1, &m_fboTexture);
    gl.glBindTexture(GL_TEXTURE_2D, m_fboTexture);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    gl.glBindTexture(GL_TEXTURE_2D, 0);

    // Depth buffer
    gl.glGenRenderbuffers(1, &m_rboDepth);
    gl.glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
    gl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, windowWidth, windowHeight);
    gl.glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Framebuffer to link everything together
    gl.glGenFramebuffers(1, &m_fbo);
    gl.glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboTexture, 0);
    gl.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboDepth);

    GLenum status;
    if ((status = gl.glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        qDebug() << "glCheckFramebufferStatus: error " << status;
    }

    gl.glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_objectsOnGPU = true;
}

void PostprocessingManager::deallocateFBO(QOpenGLFunctions & gl) const
{
    if(!m_objectsOnGPU)
        return;

    gl.glDeleteRenderbuffers(1, &m_rboDepth);
    gl.glDeleteTextures(1, &m_fboTexture);
    gl.glDeleteFramebuffers(1, &m_fbo);

    m_objectsOnGPU = false;
}

} //namespace terminus

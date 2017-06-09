#include "renderer.h"

#include <resources/geometry.h>
#include <resources/material.h>
#include <resources/program.h>
#include <util/gldebug.h>


namespace terminus
{

Renderer::Renderer()
: m_viewport(1024, 768)
, m_frameBufferObject(m_viewport, {GL_RGBA16F, GL_RGBA16F, GL_RGBA16F, GL_RGBA16F, GL_RGBA16F})
{
    // initialize effects
    //auto motionBlurPtr = std::unique_ptr<MotionBlur>(new MotionBlur(world));
    //auto invertPtr = std::unique_ptr<Invert>(new Invert(world));
    //auto vignettePtr = std::unique_ptr<Vignette>(new Vignette(world));

    // create list of effects
    //m_effects.push_back(std::move(motionBlurPtr));
    //m_effects.push_back(std::move(invertPtr));
    //m_effects.push_back(std::move(vignettePtr));

    // create FBO per effect
    for(unsigned int i=0; i<m_effects.size(); i++)
    {
        auto fboPtr = std::unique_ptr<FrameBufferObject>(new FrameBufferObject(world.viewport()));

        m_effectFBOs.push_back(std::move(fboPtr));
    }

    m_compose = std::unique_ptr<Compose>(new Compose(world));
    // last "effect" to render to screen instead of texture
    m_passthrough = std::unique_ptr<Passthrough>(new Passthrough(world));
}

void Renderer::registerObject(const GameObject * object)
{
    if(object)
    {
        m_gameObjects.insert(object);
    }
}

void Renderer::deregisterObject(const GameObject * object)
{
    m_gameObjects.erase(object);
}


Light * Renderer::allocateLight()
{
    Light * light = nullptr;
    for (uint8_t i = 0; i < LightCount; ++i)
    {
        if (!m_lightsOccupied[i])
        {
            m_lightsOccupied[i]= true;
            light = &(m_lights[i]);
        }
    }
    return light;
}

Light * Renderer::allocateLight(const Light & preset)
{
    Light * light = allocateLight();
    if (light)
    {
        (*light) = preset;
    }
    return light;
}

void Renderer::deallocateLight(Light * light)
{
    for (uint8_t i = 0; i < LightCount; ++i)
    {
        if (light == &(m_lights[i]))
        {
            m_lightsOccupied[i]= false;
        }
    }
}

uint8_t Renderer::availableLights() const
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < LightCount; ++i)
    {
        if (!m_lightsOccupied[i])
        {
            ++count;
        }
    }
    return count;
}

void Renderer::resize(const QSize & viewport)
{
    m_viewport = viewport;
    //TODO-LW m_frameBufferObject.resize();
}

void Renderer::render(const CameraMatrices & cam) const
{
    // render to g-buffer instead of to the screen
    m_frameBufferObject.bindFBO();
    printGlError(__FILE__, __LINE__);
    glViewport(0, 0, m_viewport.width(), m_viewport.height());

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_BLEND);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW); //TODO check if still necessary after geometry loader

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

    printGlError(__FILE__, __LINE__);
    for (const GameObject * obj : m_gameObjects)
    {
        renderObject(obj, cam);
    }

    m_frameBufferObject.releaseFBO();

    // clear real framebuffer
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // disable stuff we don't need while post-processing
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // apply post-processing effects and render image to screen

    m_frameBufferObject.bindTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE0);
    m_frameBufferObject.bindTexture(GL_COLOR_ATTACHMENT1, GL_TEXTURE1);
    m_frameBufferObject.bindTexture(GL_COLOR_ATTACHMENT2, GL_TEXTURE2);
    m_frameBufferObject.bindTexture(GL_COLOR_ATTACHMENT3, GL_TEXTURE3);
    m_frameBufferObject.bindTexture(GL_COLOR_ATTACHMENT4, GL_TEXTURE4);

    m_compose->render();

/*    m_frameBufferObject.releaseTexture(GL_TEXTURE0);
    m_frameBufferObject.releaseTexture(GL_TEXTURE1);
    m_frameBufferObject.releaseTexture(GL_TEXTURE2);
    m_frameBufferObject.releaseTexture(GL_TEXTURE3);
    m_frameBufferObject.releaseTexture(GL_TEXTURE4);
    m_frameBufferObject.bindTexture(GL_COLOR_ATTACHMENT3, GL_TEXTURE0);
    m_passthrough->render();
    m_frameBufferObject.releaseTexture(GL_TEXTURE0);*/
//    assert(m_effects.size() == m_effectFBOs.size());

//    for(unsigned int i = 0; i<m_effects.size(); i++)
//    {
//        AbstractEffect * effectPtr = m_effects[i].get();
//        FrameBufferObject * targetFBOPtr = m_effectFBOs[i].get();
//        FrameBufferObject * sourceFBOPtr = &m_frameBufferObject;

//        if(i > 0)
//            sourceFBOPtr = m_effectFBOs[i-1].get();

//        if(i == m_effects.size() -1)
//            targetFBOPtr = nullptr;

//        applyEffect(effectPtr, sourceFBOPtr, targetFBOPtr);
//    }
}

void Renderer::renderObject(const GameObject * obj, const CameraMatrices & cam) const
{
    auto pGeometry = obj->renderGeometry();
    auto pProgram = obj->renderProgram();
    auto pMaterial = obj->renderMaterial();

    if(!pGeometry || !*pGeometry)
    {
        qDebug() << "Geometry to render does not exist!";
        return;
    }
    if(!pProgram || !*pProgram)
    {
        qDebug() << "Program to render with does not exist!";
        return;
    }

    Geometry & geometry = **pGeometry;
    Program & program = **pProgram;

    printGlError(__FILE__, __LINE__);
    program.bind();

    printGlError(__FILE__, __LINE__);

    program.setUniform("u_model", obj->matrix().matrix());
    program.setUniform("u_modelInv", obj->matrix().matrixInverted());
    program.setUniform("u_view", cam.view());
    program.setUniform("u_viewInv", cam.viewInverted());
    program.setUniform("u_proj", cam.projection());
    program.setUniform("u_projInv", cam.projectionInverted());
    program.setUniform("u_normal", cam.normal());
    program.setUniform("u_viewProj", cam.viewProjection());
    program.setUniform("u_viewProjInv", cam.viewProjectionInverted());
    program.setUniform("m_lights", reinterpret_cast<const QVector4D*>(&m_lights), LightCount * Light::vectorCount);
    printGlError(__FILE__, __LINE__);

    if(pMaterial && *pMaterial)
    {
        (**pMaterial).setUniforms(program);
        printGlError(__FILE__, __LINE__);
    }

    //TODO-LW obj->localRenderSetup(program);
    printGlError(__FILE__, __LINE__);

    printGlError(__FILE__, __LINE__);
    geometry.draw();
    printGlError(__FILE__, __LINE__);

    //TODO-LW obj->localRenderCleanup(program);
    printGlError(__FILE__, __LINE__);

    program.release();
    printGlError(__FILE__, __LINE__);
}

}

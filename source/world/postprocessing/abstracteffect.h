#pragma once

#include <world/drawables/abstractgraphicsobject.h>
#include <resources/resourcemanager.h>
#include <resources/framebufferobject.h>
#include <world/world.h>
#include <world/camera.h>
#include <world/lightmanager.h>

namespace terminus
{

struct FboAttachment
{
    FboAttachment(const FrameBufferObject & frameBufferObject, GLenum colorAttachmentNumber)
        : frameBufferObject(frameBufferObject)
        , colorAttachmentNumber(colorAttachmentNumber)
    {}

    const FrameBufferObject & frameBufferObject;
    GLenum colorAttachmentNumber;
};

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


    AbstractEffect(const World & world, DisabledBehaviour behaviour = DisabledBehaviour::PASS);
    virtual ~AbstractEffect();

    virtual unsigned int requiredNumberOfInputTextures() const { return 1; }

    void setTextures(std::initializer_list<FboAttachment> inputTextureList);

    void render(const Camera & camera, const LightManager & lightManager) const;

    virtual void localRenderSetup(const Camera & camera, Program & program) const;
    virtual void localRenderCleanup(const Camera & camera, Program & program) const;

    void enable(bool enabled = true);
    void disable();
    bool isEnabled() const;

    const FrameBufferObject &targetFBO() const;

protected:
    void applyDisabled() const;

protected:
    const World & m_world;
    DisabledBehaviour m_disabledBehaviour;
    bool m_enabled;

    FrameBufferObject m_targetFBO;

    std::vector<FboAttachment> m_sourceAttachments;

    std::shared_ptr<std::unique_ptr<Program>> m_program;
    std::shared_ptr<std::unique_ptr<Geometry>> m_geometry;
};

}

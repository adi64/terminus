#pragma once

#include <memory>

#include <world/drawables/gameobject.h>

class QImage;

namespace terminus
{

/*!
 * \brief The SkyBox class renders an environment map in the form of a cube map
 * on a screen aligned quad in the background.
 */
class SkyBox : public GameObject
{
public:
    SkyBox(Game & world);

    void localRenderSetup(Program & program) const override;
    void localRenderCleanup(Program & program) const override;
protected:
    void allocateCubeMap() const;
    void deallocateCubeMap() const;

    mutable bool m_cubeMapOnGPU;
    std::unique_ptr<QImage> m_imageNX;
    std::unique_ptr<QImage> m_imagePX;
    std::unique_ptr<QImage> m_imageNY;
    std::unique_ptr<QImage> m_imagePY;
    std::unique_ptr<QImage> m_imageNZ;
    std::unique_ptr<QImage> m_imagePZ;
    mutable GLuint m_cubeMap;
};

}

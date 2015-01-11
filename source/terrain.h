#pragma once

#include <memory>

#include "abstractphysicsobject.h"
#include "track.h"

namespace terminus
{

class Track;

class Terrain : public AbstractPhysicsObject
{
public:
    Terrain(const std::shared_ptr<Scene> &scene);

    void update(int elapsedMilliseconds) override;
    void render(QOpenGLFunctions& gl) const override;

    Track *playerTrack() const;
    Track *enemyTrack() const;

protected:
    std::unique_ptr<Track> m_playerTrack;
    std::unique_ptr<Track> m_enemyTrack;
};

}

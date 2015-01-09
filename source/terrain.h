#pragma once

#include <memory>

#include "abstractgraphicsobject.h"
#include "levelgenerator.h"

namespace terminus
{

class Track;

class Terrain : public AbstractGraphicsObject
{
public:
    Terrain(Scene *scene);
    ~Terrain();

    void render(QOpenGLFunctions& gl, int elapsedMilliseconds);

    Track *playerTrack() const;
    Track *enemyTrack() const;

protected:
    std::unique_ptr<Track> m_playerTrack;
    std::unique_ptr<Track> m_enemyTrack;

    LevelGenerator m_levelGen;
};

}

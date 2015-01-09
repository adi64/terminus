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

    void update(int elapsedMilliseconds);
    void render(QOpenGLFunctions& gl) const override;

    Track *playerTrack() const;
    Track *enemyTrack() const;

protected:
    std::unique_ptr<Track> m_playerTrack;
    std::unique_ptr<Track> m_enemyTrack;

    LevelGenerator m_levelGen;
};

}

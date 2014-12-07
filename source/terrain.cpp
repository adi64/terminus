#include "terrain.h"

#include <QDebug>

#include "track.h"

namespace terminus
{

Terrain::Terrain(Scene *scene)
    : AbstractGraphicsObject(scene)
    , m_playerTrack(std::unique_ptr<Track>(new Track(scene)))
    , m_enemyTrack(std::unique_ptr<Track>(new Track(scene)))
{   

}

Terrain::~Terrain()
{

}

void Terrain::render(QOpenGLFunctions &gl, int elapsedMilliseconds)
{
    // render tracks
    m_playerTrack->render(gl, elapsedMilliseconds);
    m_enemyTrack->render(gl, elapsedMilliseconds);

    // render terrain
}

Track *Terrain::playerTrack() const
{
    return m_playerTrack.get();
}

Track *Terrain::enemyTrack() const
{
    return m_enemyTrack.get();
}

}

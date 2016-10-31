#pragma once

#include <memory>

#include <world/drawables/gameobject.h>
#include <util/polyline.h>

namespace terminus
{

/*!
 * \brief The Track class represents a track that a Train moves on.
 * It is currently not rendered.
 * The course is represented by a Polyline instance.
 */
class Track : public GameObject
{
public:
    Track(Game & world, bool isRightTrack);

    /*!
     * \return Polyline that represents this track's course
     */
    Polyline & course();

    /*!
     * \brief change the course of this track
     * \param course - ownership is taken
     */
    void setCourse(Polyline * course);

    /*!
     * \return true if this track is on the right side
     * in the direction of travel
     */
    bool isRightTrack();

protected:
    virtual bool localRenderEnabled() const override;

protected:
    std::unique_ptr<Polyline> m_course;

    bool m_isOtherTrackLeft;
};

}

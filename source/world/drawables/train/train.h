#pragma once

#include <QVector3D>

#include <memory>
#include <vector>

#include <world/drawables/abstractgraphicsobject.h>
#include <world/drawables/train/wagons/abstractwagon.h>
#include <world/world.h>

namespace terminus
{

class AbstractPlayer;
class Track;
class Camera;

/*!
 * \brief The Train class works as a logical container for all wagons one train consists of.
 *
 * The train itself isn't rendered, but implements all functionality used for determining the current position on the track
 * and the resulting positions for all corresponding wagons.
 */
class Train : public AbstractGraphicsObject
{
public:
    static const float base_velocity;
public:
    Train(World & world, Track * track);
    ~Train();

    /*!
     * \brief Adds a wagon of the given WagonType to the end of the train.
     *
     * A wagon of the given type will be constructed and added to the end of
     * the train. No additional EngineWagons can be added.
     */
    template<typename WagonType>
    void addWagon();
    void addWagon(WagonType type);
    /*!
     * \brief Inserts a wagon of the given WagonType at the given position.
     *
     * A wagon of the given type will be constructed and inserted at the given
     * position. No additional EngineWagons can be added.
     */
    template<typename WagonType>
    void insertWagon(int targetPos);

    /*!
     * \brief Removes the wagon at the given index
     *
     * The EngineWagon at index 0 cannot be removed.
     */
    void removeWagon(unsigned int index);

    /*!
     * \brief Moves the wagon at position wagonPos to position targetPos
     * \param wagonPos The index of the wagon to be moved
     * \param targetPos The index where the wagon should be moved to
     *
     * The EngineWagon at index 0 cannot be moved.
     */
    void moveWagon(unsigned int wagonPos, unsigned int targetPos);

    /*!
     * \brief Update train
     *
     * \sa AbstractWagon::localUpdate()
     */
    void localUpdate() override;

    AbstractWagon *wagonAt(unsigned int index) const;
    Track *track() const;

    void follow(Train *train);
    float velocity() const;
    void setVelocity(float velocity);

    /*!
     * \brief Returns the travelled distance of the train's head on the spline
     */
    float travelledDistance() const;

    /*!
     * \brief Set the travelled distance of the train's head on the spline
     */
    void setTravelledDistance(float travelledDistance);

    /*!
     * \brief Returns the ratio of travelled distance to track length
     */
    float travelledDistanceRelative() const;

    QVector3D headPosition() const;

    /*!
     * \brief Returns the number of wagons in this train
     */
    unsigned int size() const;

    // TODO FIXME remove this getter since a camera will have the ability to be
    // bound to any AbstractGraphicsObject which then knows the camera itself.
    AbstractPlayer &player() const;
    void setPlayer(AbstractPlayer *player);

protected:
    virtual bool localRenderEnabled() const override;

    /*!
     * \brief Calculates offset for every wagon relative to train head
     *
     * This method calculates and sets the offset for every wagon.
     * If wagons are rearranged, it needs to be called again.
     */
    void calculateWagonOffset();

    virtual void doForAllChildren(std::function<void(AbstractGraphicsObject &)> callback) override;

protected:
    /*!
     * \brief The vector containing all wagons
     *
     * Every train has exactly one EngineWagon which must be at index 0.
     * It cannot be moved or removed and no additional EngineWagons can be
     * added to the train.
     */
    std::vector<std::unique_ptr<AbstractWagon>> m_wagons;

    bool m_hasEngine; //!< Every train needs exactly one engine

    float m_velocity;
    Train *m_followedTrain;

    float m_travelledDistance;
    Track *m_track;

    /*!
     * \brief A pointer to the player that controls this train.
     *
     * Since the controlling player of a train can change, this is stored as a
     * simple pointer.
     */
    AbstractPlayer *m_player;
};

}

#include "train.hpp"

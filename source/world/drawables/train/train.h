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

class Train : public AbstractGraphicsObject
{
public:
    static const float base_velocity;
public:
    Train(World & world, Track * track);
    ~Train();

    template<typename WagonType>
    void addWagon();

    template<typename WagonType>
    void insertWagon(int targetPos);

    void removeWagon(unsigned int index);
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

    float travelledDistance() const;
    float travelledDistanceRelative() const;

    QVector3D headPosition() const;
    unsigned int size() const;
    bool isPlayerControlled() const;

    // TODO FIXME remove this getter since a camera will have the ability to be bound to any AbstractGraphicsObject which then knows the camera itself.
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
    std::vector<std::unique_ptr<AbstractWagon>> m_wagons;
    bool m_isRunning;
    bool m_hasEngine; //!< Every train needs exactly one engine

    float m_velocity;
    Train *m_followedTrain;

    float m_travelledDistance;
    Track *m_track;

    /*!
     * \brief A pointer to the player that controls this train.
     *
     * Since the controlling player of a train can change, this is stored as a simple pointer.
     */
    AbstractPlayer *m_player;
};

}

#include "train.hpp"

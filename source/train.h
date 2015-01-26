#pragma once

#include <QVector3D>

#include <memory>
#include <vector>

#include "abstractgraphicsobject.h"
#include "wagons/abstractwagon.h"
#include "scene.h"

namespace terminus
{

class Track;

class Train : public AbstractGraphicsObject
{
public:
    static const float base_velocity;
public:
    Train(std::shared_ptr<Scene> scene, Track *track, bool playerControlled);
    ~Train();

    template<typename WagonType> void addWagon();
    template<typename WagonType> void insertWagon(int targetPos);

    void removeWagon(unsigned int index);
    void moveWagon(unsigned int wagonPos, unsigned int targetPos);

    void update(int elapsedMilliseconds) override;
    void render(QOpenGLFunctions &gl) const override;

    AbstractWagon *wagonAt(unsigned int index) const;
    Track *track() const;

    void follow(std::shared_ptr<Train> train);
    float velocity() const;
    void setVelocity(float velocity);

    float travelledDistance() const;
    QVector3D headPosition() const;
    unsigned int size() const;
    bool isPlayerControlled() const;

protected:
    void calculateWagonOffset();

protected:
    std::vector<std::unique_ptr<AbstractWagon>> m_wagons;
    bool m_isRunning;
    bool m_hasEngine;
    bool m_playerControlled;

    float m_velocity;
    std::shared_ptr<Train> m_followedTrain;

    float m_travelledDistance;
    Track *m_track;
};

}

#include "train.hpp"

#pragma once

#include <QVector3D>

#include <memory>
#include <vector>

#include <world/drawables/abstractgraphicsobject.h>
#include <world/drawables/train/wagons/abstractwagon.h>
#include <world/scene.h>

#include "qmltrain.h"

namespace terminus
{

class Track;
class Camera;

class Train : public AbstractGraphicsObject
{
public:
    static const float base_velocity;
public:
    Train(std::shared_ptr<Scene> scene, Track *track);
    ~Train();

    template<typename WagonType> void addWagon();
    template<typename WagonType> void insertWagon(int targetPos);

    void removeWagon(unsigned int index);
    void moveWagon(unsigned int wagonPos, unsigned int targetPos);

    /*!
     * \brief Update train and all wagons that belong to the train
     *
     * \sa AbstractWagon::update()
     */
    void update(int elapsedMilliseconds) override;

    /*!
     * \brief Renders all wagons that belong to the train
     *
     * A train itself has no meaningful graphical representation itself;
     * therefore in order to render it, all wagons that belong to the train are rendered here
     *
     * \sa AbstractWagon::render()
     */
    void render(QOpenGLFunctions &gl) const override;

    QMLTrain *qmlTrain() const;

    AbstractWagon *wagonAt(unsigned int index) const;
    Track *track() const;

    void follow(std::shared_ptr<Train> train);
    float velocity() const;
    void setVelocity(float velocity);

    float travelledDistance() const;
    QVector3D headPosition() const;
    unsigned int size() const;
    bool isPlayerControlled() const;

    Camera &playerCamera() const; //!< \sa m_playerCamera
    void setPlayerCamera(std::shared_ptr<Camera> camera); //!< \sa m_playerCamera

protected:
    /*!
     * \brief Calculates offset for every wagon relative to train head
     *
     * This method calculates and sets the offset for every wagon.
     * If wagons are rearranged, it needs to be called again.
     */
    void calculateWagonOffset();

signals:
    void sizeChanged();

protected:
    std::vector<std::unique_ptr<AbstractWagon>> m_wagons;
    std::unique_ptr<QMLTrain> m_qmlTrain;
    bool m_isRunning;
    bool m_hasEngine; //!< Every train needs exactly one engine

    float m_velocity;
    std::shared_ptr<Train> m_followedTrain;

    float m_travelledDistance;
    Track *m_track;

    /*!
     * \brief A pointer to the controlling player's camera
     *
     * A Train knows the camera of the player that controls it in order to get the player's aim vector
     */
    std::shared_ptr<Camera> m_playerCamera;
};

}

#include "train.hpp"

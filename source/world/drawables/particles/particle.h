#include <world/drawables/abstractgraphicsobject.h>

namespace terminus
{

class Particle : public AbstractGraphicsObject
{
public:
    Particle(std::shared_ptr<Scene> scene, QVector3D position, QVector3D velocity = QVector3D(0.0, 0.0, 0.0));
    ~Particle();

public:
    void update(int elapsedMilliseconds);
    void addToCurrentLifeTime(float value);

public:
    QVector3D velocity();
    QVector3D color();
    float acceleration();
    float currentLifeTime();
    float maxLifeTime();

    void setVelocity(QVector3D value);
    void setColor(QVector3D value);
    void setAcceleration(float value);
    void setMaxLifeTime(float value);

protected:
    QVector3D m_velocity;
    QVector3D m_color;
    float m_acceleration;
    float m_currentLifeTime;
    float m_maxLifeTime;
};

} //namespace

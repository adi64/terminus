#include "Particle.h"

Particle::Particle(std::shared_ptr<Scene> scene, QVector3D position, QVector3D velocity = QVector3D(0.0, 0.0, 0.0))
 : AbstractGraphicsObject(scene)
 , m_position(position)
 , m_velocity(velocity)
 , m_currentLifeTime(0.0)
{

}

Particle::~Particle()
{

}

void Particle::update(int elapsedMilliseconds)
{

}

void Particle::addToCurrentLifeTime(float value)
{
    m_currentLifeTime += value;
}

QVector3D Particle::velocity()
{
    return m_velocity;
}

QVector3D Particle::color()
{
    return m_color;
}

float Particle::acceleration()
{
    return m_acceleration;
}

float Particle::currentLifeTime()
{
    return m_currentLifeTime;
}

float Particle::maxLifeTime()
{
    return m_maxLifeTime;
}

void Particle::setVelocity(QVector3D value)
{
    m_velocity = value;
}

void Particle::setColor(QVector3D value)
{
    m_color = value;
}

void Particle::setAcceleration(float value)
{
    m_acceleration = value;
}

void Particle::setMaxLifeTime(float value)
{
    m_maxLifeTime = value;
}

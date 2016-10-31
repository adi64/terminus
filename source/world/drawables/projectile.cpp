#include "projectile.h"

#include <network/networkmanager.h>
#include <player/abstractplayer.h>
#include <resources/resourcemanager.h>
#include <resources/soundmanager.h>
#include <util/timer.h>
#include <world/drawables/explosion.h>
#include <world/drawables/train/wagons/abstractwagon.h>
#include <world/drawables/train/train.h>
#include <world/game.h>

namespace terminus
{

Projectile::Projectile(Game & world)
: DynamicPhysicsObject(world)
{   
    m_program = ResourceManager::getInstance()->getProgram("basicShader");
    m_geometry = ResourceManager::getInstance()->getGeometry("base_ico1");
    m_material = ResourceManager::getInstance()->getMaterial("base_Red");

    m_lifeTimer = m_world.timer().allocateTimer();

    initializePhysics(new btSphereShape(1.0), 1.f);
    setScale(0.3f);
}

Projectile::~Projectile()
{
    deallocatePhysics();

    m_world.timer().releaseTimer(m_lifeTimer);
}

void Projectile::localUpdate()
{
    DynamicPhysicsObject::localUpdate();

    if(m_world.timer().get(m_lifeTimer) > maxAgeInMilliseconds())
    {
        dispose();
    }
}

float Projectile::damage() const
{
    return 10.0f;
}

void Projectile::onCollisionWith(AbstractPhysicsObject *other)
{
    // don't to damage if this projectile was not spawned locally - the other client will inform us of damage events and such
    if(!m_spawnedLocally)
    {
        return;
    }

    auto otherWagon = dynamic_cast<AbstractWagon*>(other);
    if(otherWagon)
    {
        otherWagon->setHealth(otherWagon->currentHealth() - damage());

        // send hit event if enemy train was hit
        for(unsigned int i=0; i<m_world.enemyPlayer().train()->size(); ++i)
        {
            if(m_world.enemyPlayer().train()->wagonAt(i) == otherWagon)
            {
                m_world.networkManager().sendProjectileHitCommand(i, damage());
                break;
            }
        }
    }

    m_world.addObject(new Explosion(m_world, position()));

    SoundManager::getInstance()->playSound("explosion");

    dispose();
}

int Projectile::maxAgeInMilliseconds() const
{
    return 5000;
}

short Projectile::myCollisionType() const
{
    return BulletWorld::CollisionTypes::COLLISIONTYPE_PROJECTILE;
}

short Projectile::possibleCollisionTypes() const
{
    return BulletWorld::CollisionTypes::COLLISIONTYPE_WAGON;
}

} //namespace terminus

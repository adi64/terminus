#include "weaponwagon.h"

#include <QDebug>

namespace terminus
{

WeaponWagon::WeaponWagon(Scene *scene)
    : m_scene(scene)
{
}

void WeaponWagon::render()
{
    qDebug() << "Trying to render WeaponWagon";
}

}

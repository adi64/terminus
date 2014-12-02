#include "weaponwagon.h"

#include <QDebug>

#include "../scene.h"

namespace terminus
{

WeaponWagon::WeaponWagon(Scene *scene)
    : AbstractWagon(scene)
{
}

void WeaponWagon::render(QOpenGLFunctions& gl)
{
}

}

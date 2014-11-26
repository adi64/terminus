#include "enginewagon.h"

#include <QDebug>

namespace terminus
{

EngineWagon::EngineWagon()
{
}

void EngineWagon::render()
{
    qDebug() << "render EngineWagon";
}

float EngineWagon::length()
{
    //TODO change value later
    return 1.5f;
}

}

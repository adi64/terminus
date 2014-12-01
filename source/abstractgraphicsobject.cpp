#include "abstractgraphicsobject.h"

#include <QDebug>
#include "scene.h"

namespace terminus
{

AbstractGraphicsObject::AbstractGraphicsObject(Scene *scene)
    : m_scene(scene)
{
}

}

#include "abstractgraphicsobject.h"

#include <QDebug>

namespace terminus
{

AbstractGraphicsObject::AbstractGraphicsObject()
{
}

void AbstractGraphicsObject::render()
{
    qDebug() << "AbstractGraphicsObject::render() -- this is not what you want!";
}

}

#ifndef ABSTRACTGRAPHICSOBJECT_H
#define ABSTRACTGRAPHICSOBJECT_H

namespace Terminus
{

class AbstractGraphicsObject
{
public:
    AbstractGraphicsObject();
    virtual void render() = 0;
};

}

#endif // ABSTRACTGRAPHICSOBJECT_H

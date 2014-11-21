#ifndef ABSTRACTGRAPHICSOBJECT_H
#define ABSTRACTGRAPHICSOBJECT_H

namespace terminus
{

class AbstractGraphicsObject
{
public:
    AbstractGraphicsObject();
    virtual void render() = 0;
};

}

#endif // ABSTRACTGRAPHICSOBJECT_H

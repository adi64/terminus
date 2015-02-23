#include "qmltrain.h"

#include <assert.h>

#include <world/drawables/train/train.h>

namespace terminus
{

QMLTrain::QMLTrain(Train *train)
    : m_train(train)
{

}

QMLTrain::~QMLTrain()
{

}

Train *QMLTrain::train() const
{
    return m_train;
}

void QMLTrain::setTrain(Train *train)
{
    m_train = train;
}

QMLWagon *QMLTrain::wagonAt(unsigned int index) const
{
    assert(m_train != nullptr);
    return m_train->wagonAt(index)->qmlWagon();
}

unsigned int QMLTrain::size() const
{
    assert(m_train != nullptr);
    return m_train->size();
}

}

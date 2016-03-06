#pragma once

#include "train.h"

#include <assert.h>

#include <world/drawables/train/wagons/abstractwagon.h>
#include <world/drawables/train/wagons/enginewagon.h>

#include <QDebug>

namespace terminus
{

template<typename WagonType>
void Train::addWagon()
{
    insertWagon<WagonType>(m_wagons.size());
}

template<typename WagonType>
void Train::insertWagon(int targetPos)
{
    assert(targetPos <= static_cast<int>(m_wagons.size()));
    assert(targetPos >= 0);

    auto newWagon = std::unique_ptr<WagonType>(new WagonType(m_world, this));

    m_wagons.insert(m_wagons.begin() + targetPos, std::move(newWagon));

    calculateWagonOffset();
}

}

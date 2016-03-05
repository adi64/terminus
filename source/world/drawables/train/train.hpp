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
    insertWagon<WagonType>(-1);
}

template<typename WagonType>
void Train::insertWagon(int targetPos)
{
    assert(targetPos < static_cast<int>(m_wagons.size()));
    assert(targetPos >= -1);

    auto wagonRaw = new WagonType(m_world, this);

    if(dynamic_cast<EngineWagon*>(wagonRaw) != nullptr)
    {
        assert(!m_hasEngine);
        m_hasEngine = true;
    }

    auto newWagon = std::unique_ptr<WagonType>(wagonRaw);

    if(targetPos == -1)
    {
        m_wagons.push_back(std::move(newWagon));
    }
    else
    {
        m_wagons.insert(m_wagons.begin() + targetPos, std::move(newWagon));
    }

    calculateWagonOffset();
}

}

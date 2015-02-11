#pragma once

#include "wagons/abstractwagon.h"
#include "wagons/enginewagon.h"

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
    auto wagonRaw = new WagonType(m_scene, this);

    bool creatingEngineWagon = false;

    if((dynamic_cast<EngineWagon*>(wagonRaw) != nullptr))
    {
        if(m_hasEngine)
        {
            qDebug() << "NOPE!";
            return;
        }
        creatingEngineWagon = true;
    }

    auto newWagon = std::unique_ptr<WagonType>(wagonRaw);

    if(targetPos == -1)
    {
        m_wagons.push_back(std::move(newWagon));
    }
    else
    {
        if(targetPos >= static_cast<int>(m_wagons.size()))
        {
            qDebug() << "targetPos >= " << m_wagons.size();
            return;
        }
        if(targetPos <= 0)
        {
            qDebug() << "targetPos <= 0";
            return;
        }

        m_wagons.insert(m_wagons.begin() + targetPos, std::move(newWagon));
    }

    if(creatingEngineWagon)
    {
        m_hasEngine = true;
    }

    calculateWagonOffset();

}

}

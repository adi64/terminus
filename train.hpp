#pragma once

#include "abstractwagon.h"

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
    auto newWagon = std::unique_ptr<WagonType>(new WagonType);

    if(targetPos == -1)
    {
        m_wagons.push_back(std::move(newWagon));
    }else{
        if(targetPos >= m_wagons.size())
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

    calculateWagonOffset();
}

}

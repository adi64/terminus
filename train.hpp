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
    auto newWagon = std::make_unique<WagonType>();

    //auto tmp = new WagonType;
    //auto tmp2 = dynamic_cast<AbstractWagon*>(tmp);
    //std::unique_ptr<AbstractWagon> newWagon(tmp2);

//    if(!tmp2)
//    {
//        qDebug() << "newWagon is no AbstractWagon!";
//        return;
//    }

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
}

}

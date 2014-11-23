#include "train.h"

#include <QDebug>

#include "enginewagon.h"

namespace terminus
{


Train::Train()
{
    // Add engine
    addWagon<EngineWagon>();
}

Train::~Train()
{

}

void Train::removeWagon(int index)
{
    if(index >= m_wagons.size())
    {
        qDebug() << index << " > " << m_wagons.size();
        return;
    }

    if(index <= 0)
    {
        qDebug() << "you cannot delete the engine!";
        return;
    }

    m_wagons.erase(m_wagons.begin() + index);
}

void Train::moveWagon(int wagonPos, int targetPos)
{
    if(wagonPos >= m_wagons.size())
    {
        qDebug() << "wagonPos >= " << m_wagons.size();
        return;
    }

    if(wagonPos <= 0)
    {
        qDebug() << "Cannot move engine";
        return;
    }

    if(targetPos >= m_wagons.size())
    {
        qDebug() << "targetPos >= " << m_wagons.size();
        return;
    }

    if(targetPos <= 0)
    {
        qDebug() << "Cannot move wagon in front of engine";
        return;
    }

    if(targetPos == wagonPos)
    {
        qDebug() << "Not moving a wagon to its old position";
        return;
    }

    // TODO IMPLEMENT LATER
    // this won't work, see unique_ptr

//    auto wagon = m_wagons.at(wagonPos).get();
//    m_wagons.erase(m_wagons.begin() + wagonPos);

//    if(targetPos > wagonPos)
//    {
//        m_wagons.insert(m_wagons.begin() + targetPos - 1, wagon);
//    }
//    else
//    {
//        m_wagons.insert(m_wagons.begin() + targetPos, wagon);
//    }
}


}

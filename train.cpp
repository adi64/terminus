#include "train.h"

#include <memory>
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

    auto wagon = std::move(m_wagons.at(wagonPos));
    m_wagons.erase(m_wagons.begin() + wagonPos);

    if(targetPos > wagonPos)
    {
        m_wagons.insert(m_wagons.begin() + targetPos - 1, std::move(wagon));
    }
    else
    {
        m_wagons.insert(m_wagons.begin() + targetPos, std::move(wagon));
    }
}


}

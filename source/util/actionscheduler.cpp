#include "actionscheduler.h"

namespace terminus
{

ActionScheduler::ActionScheduler()
: m_currentActionList(0)
{
}

void ActionScheduler::scheduleAction(const Action & action)
{
    std::unique_lock<std::recursive_mutex> actionListLock(m_actionListMutex);

    m_actionList[m_currentActionList].push_back(action);
}

void ActionScheduler::executeActions()
{
    std::unique_lock<std::recursive_mutex> actionListLock(m_actionListMutex);

    int previousActionList = m_currentActionList;
    m_currentActionList = (m_currentActionList + 1) % 2;

    for (auto &action : m_actionList[previousActionList])
    {
        if(action())
        {
            scheduleAction(action);
        }
    }
    m_actionList[previousActionList].clear();
}

} //namespace terminus

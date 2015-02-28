#include "deferredactionhandler.h"

#include "game.h"

namespace terminus
{

DeferredActionHandler::DeferredActionHandler(Game *game)
: m_game(game)
, m_currentActionList(0)
, m_actionList({std::unique_ptr<DeferredActionList>(new DeferredActionList),
                  std::unique_ptr<DeferredActionList>(new DeferredActionList)})
{
}

void DeferredActionHandler::scheduleAction(const DeferredAction& action)
{
    std::unique_lock<std::mutex> actionListLock(m_actionListMutex);
    m_actionList[m_currentActionList]->push_back(action);
}

void DeferredActionHandler::processDeferredActions()
{
    std::unique_lock<std::mutex> actionListLock(m_actionListMutex);
    int nextActionList = (m_currentActionList + 1) % 2;

    for (auto &action : (*m_actionList[m_currentActionList]))
    {
        if(action())
        {
            m_actionList[nextActionList]->push_back(action);
        }
    }
    m_actionList[m_currentActionList]->clear();
    m_currentActionList = nextActionList;
}

}

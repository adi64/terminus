#include "deferredactionhandler.h"

#include "game.h"

namespace terminus
{

DeferredActionHandler::DeferredActionHandler(Game *game)
    : m_game(game)
    , m_actionList(std::unique_ptr<DeferredActionList>(new DeferredActionList))
{

}

void DeferredActionHandler::scheduleAction(const DeferredAction& event)
{
    std::unique_lock<std::mutex> actionListLock(m_actionListMutex);
    m_actionList->push_back(event);
}

void DeferredActionHandler::processDeferredActions()
{
    std::unique_lock<std::mutex> actionListLock(m_actionListMutex);
    for (auto &action : (*m_actionList))
    {
        action();
    }
    m_actionList->clear();
}

}

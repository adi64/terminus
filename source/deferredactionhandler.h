#pragma once

#include <memory>
#include <mutex>
#include <functional>
#include <Qt>

using DeferredAction = std::function<void(void)>;
using DeferredActionList = std::vector<DeferredAction>;

namespace terminus
{

class Game;

class DeferredActionHandler
{
public:
    DeferredActionHandler(Game *game);

    void scheduleAction(DeferredAction event);
    void processDeferredActions();
protected:
    Game *m_game;
    std::unique_ptr<DeferredActionList> m_actionList;
    std::mutex m_actionListMutex;
};

}

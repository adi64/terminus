#pragma once

#include <memory>
#include <vector>
#include <mutex>
#include <functional>
#include <Qt>

using DeferredAction = std::function<bool(void)>;
using DeferredActionList = std::vector<DeferredAction>;

namespace terminus
{

class Game;

class DeferredActionHandler
{
public:
    DeferredActionHandler(Game *game);

    void scheduleAction(const DeferredAction &event);
    void processDeferredActions();
protected:
    Game *m_game;
    int m_currentActionList;
    std::unique_ptr<DeferredActionList> m_actionList[2];
    std::mutex m_actionListMutex;
};

}

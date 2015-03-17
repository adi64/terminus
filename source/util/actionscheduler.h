#pragma once

#include <vector>
#include <mutex>
#include <functional>

namespace terminus
{

class ActionScheduler
{
public:
    using Action = std::function<bool(void)>;

    ActionScheduler();

    void scheduleAction(const Action &event);
    void executeActions();

protected:
    int m_currentActionList;
    std::vector<Action> m_actionList[2];

    std::recursive_mutex m_actionListMutex;
};

}

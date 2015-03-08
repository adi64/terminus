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

class DeferredActionHandler
{
public:
    DeferredActionHandler();

    void scheduleAction(const DeferredAction &event);
    void processDeferredActions();

protected:
    int m_currentActionList;
    DeferredActionList m_actionList[2];

    std::recursive_mutex m_actionListMutex;
};

}

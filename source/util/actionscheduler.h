#pragma once

#include <functional>
#include <mutex>
#include <vector>

namespace terminus
{

/*!
 * \brief The ActionScheduler class maintains a list of actions, that should
 * be executed later in a specific context.
 *
 * This class can be safely used across multiple threads.
 */
class ActionScheduler
{
public:
    /*!
     * \brief functions that can be stored as actions can not have parameters
     * and must return a bool whether they should be scheduled again
     */
    using Action = std::function<bool(void)>;

    ActionScheduler();

    /*!
     * \brief add an Action to the action list
     * \param event
     */
    void scheduleAction(const Action & action);

    /*!
     * \brief execute all Actions currently contained in the action list and
     * possibly schedule them again
     */
    void executeActions();

protected:
    int m_currentActionList;
    std::vector<Action> m_actionList[2];

    std::recursive_mutex m_actionListMutex;
};

}

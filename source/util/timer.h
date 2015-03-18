#pragma once

#include <chrono>
#include <map>
#include <string>

namespace terminus
{

/*!
 * \brief The Timer class implements a set of pauseable duration counters
 * (subsequently called timers) and mechanisms to adjust them individually
 * or globally.
 *
 * Timers can be allocated with a name that allows them to be accessed in
 * contexts where their id is not known.
 */
class Timer
{
public:
    using TimerID = unsigned int;
    using TimerMSec = signed int;

protected:
    static TimerMSec toMSec(std::chrono::steady_clock::duration duration);
    static std::chrono::steady_clock::duration fromMSec(TimerMSec duration);

public:
    Timer();

    /*!
     * \brief toggle global pause state
     */
    void pause();
    /*!
     * \brief set global pause state according to flag
     * \param flag
     */
    void pause(bool flag);
    /*!
     * \return global pause state
     */
    bool isPaused();

    /*!
     * \brief allocate a new timer without naming it
     * \return id of the new timer
     */
    TimerID allocateTimer();
    /*!
     * \brief allocate a new named timer
     * \param name
     * \return id of the new timer
     */
    TimerID allocateTimer(std::string name);

    /*!
     * \brief release ressources (id and timestamp) of an unnamed timer
     * \param id
     */
    void releaseTimer(TimerID id);
    /*!
     * \brief release ressources (name, id and timestamp) of a named timer
     * \param name
     */
    void releaseTimer(std::string name);

    /*!
     * \param id of any timer
     * \return true if the given id identifies an allocated timer
     *
     * The id can belong both to a named or unnamed timer
     */
    bool isAllocated(TimerID id);
    /*!
     * \param name of a named timer
     * \return true if name identifies an allocated timer
     */
    bool isAllocated(std::string name);

    /*!
     * \return reference time in milliseconds
     *
     * the reference time is the time passed since instantiation
     * of this timer object
     * \sa Timer::adjust()
     */
    TimerMSec get();
    /*!
     * \param id
     * \return time in milliseconds passed since allocation of an unnamed timer
     *
     * returns 0 msec for unallocated timer ids
     * \sa Timer::adjust()
     */
    TimerMSec get(TimerID id);
    /*!
     * \param name
     * \return time in milliseconds passed since allocation of the named timer
     *
     * allocates a new named timer, if it is not allocated already
     * \sa Timer::adjust()
     */
    TimerMSec get(std::string name);

    /*!
     * \brief adjust the reference time to match newNow
     * \param newNow - time in milliseconds
     *
     * this method affects both the time returned by get() and
     * get(id) or get(name) as named and unnamed timers operate
     * relative to the reference time
     */
    void adjust(TimerMSec newNow);
    /*!
     * \brief adjust the value of an unnamed timer to match newNow
     * \param id
     * \param newNow - time in milliseconds
     *
     * does nothing for unallocated timer ids
     */
    void adjust(TimerID id, TimerMSec newNow);
    /*!
     * \brief adjust the value of a named timer to match newNow
     * \param name
     * \param newNow - time in milliseconds
     *
     * allocates a new timer, if it is not allocated already
     */
    void adjust(std::string name, TimerMSec newNow);

protected:
    TimerID freeTimerID();

    std::chrono::steady_clock::time_point localNow();
    void startPause();
    void endPause();

protected:

    std::chrono::steady_clock m_clock;
    std::chrono::steady_clock::time_point m_pauseNow;
    bool m_isPaused;

    std::chrono::steady_clock::time_point m_baseTimeStamp;

    std::map<TimerID, std::chrono::steady_clock::time_point> m_timeStamps;
    std::map<std::string, TimerID> m_namedTimers;
};

}//namespace terminus

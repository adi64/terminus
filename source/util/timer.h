#pragma once

#include <chrono>
#include <map>
#include <string>

namespace terminus
{

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
     * \brief pause - toggle pause state of the timer
     */
    void pause();
    /*!
     * \brief pause - set pause state of the timer
     * \param flag - new pause state
     */
    void pause(bool flag);
    /*!
     * \brief isPaused - test pause state of the timer
     * \return pause state
     */
    bool isPaused();

    /*!
     * \brief allocateTimer - allocate a new timer without naming it
     * \return id of the new timer
     */
    TimerID allocateTimer();
    /*!
     * \brief allocateTimer - allocate a new named timer
     * \param name
     * \return id of the new timer
     */
    TimerID allocateTimer(std::string name);
    /*!
     * \brief releaseTimer - release ressources (id and timestamp) of an unnamed timer
     * \param id
     */
    void releaseTimer(TimerID id);
    /*!
     * \brief releaseTimer - release ressources (name, id and timestamp) of a named timer
     * \param name
     */
    void releaseTimer(std::string name);
    /*!
     * \brief isAllocated - test for existence of an unnamed timer
     * \param id
     * \return
     */
    bool isAllocated(TimerID id);
    /*!
     * \brief isAllocated - test for existence of a named timer
     * \param name
     * \return
     */
    bool isAllocated(std::string name);

    /*!
     * \brief get - return the reference time
     * \return time in milliseconds
     *
     * reference time is the time passed since instantiation of this timer object
     * \sa Timer::adjust()
     */
    TimerMSec get();
    /*!
     * \brief get - return the time passed since allocation of the unnamed timer
     * \param id
     * \return time in milliseconds
     *
     * returns 0 msec for unallocated timer ids
     * \sa Timer::adjust()
     */
    TimerMSec get(TimerID id);
    /*!
     * \brief get - return the time passed since allocation of the named timer
     * \param name
     * \return time in milliseconds
     *
     * allocates a new timer, if it is not allocated already
     * \sa Timer::adjust()
     */
    TimerMSec get(std::string name);

    /*!
     * \brief adjust - adjust the reference time to match newNow
     * \param newNow - time in milliseconds
     *
     * this method affects both the time returned by get() and get(id) or get(name) as
     * named and unnamed timers operate relative to the reference time
     */
    void adjust(TimerMSec newNow);
    /*!
     * \brief adjust - adjust the value of an unnamed timer to match newNow
     * \param id
     * \param newNow - time in milliseconds
     *
     * does nothing for unallocated timer ids
     */
    void adjust(TimerID id, TimerMSec newNow);
    /*!
     * \brief adjust - adjust the value of a named timer to match newNow
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

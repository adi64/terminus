#include "timer.h"

#include <limits>

#include <assert.h>

#include <QDebug>

namespace terminus
{

Timer::TimerMSec Timer::toMSec(std::chrono::steady_clock::duration duration)
{
    return std::chrono::duration_cast<std::chrono::duration<Timer::TimerMSec, std::ratio<1,1000>>>(duration).count();
}

std::chrono::steady_clock::duration Timer::fromMSec(Timer::TimerMSec duration)
{
    return std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<Timer::TimerMSec, std::ratio<1,1000>>(duration));
}

Timer::Timer()
: m_isPaused(false)
{
    m_baseTimeStamp = m_clock.now();
}

void Timer::pause()
{
    if(m_isPaused)
    {
        endPause();
    }
    else
    {
        startPause();
    }
    m_isPaused = !m_isPaused;
}

void Timer::pause(bool flag)
{
    qDebug() << __FILE__ << __PRETTY_FUNCTION__ << " flag = " << flag << " timer = " << get();
    if(m_isPaused && !flag)
    {
        endPause();
    }
    else if(!m_isPaused && flag)
    {
        startPause();
    }
    m_isPaused = flag;
}

bool Timer::isPaused()
{
    return m_isPaused;
}

Timer::TimerID Timer::allocateTimer()
{
    TimerID newTimer = freeTimerID();
    m_timeStamps[newTimer] = localNow();
    return newTimer;
}

Timer::TimerID Timer::allocateTimer(std::string name)
{
    if(m_namedTimers.count(name) > 0)
    {
        return m_namedTimers[name];
    }
    TimerID newTimer = allocateTimer();
    m_namedTimers[name] = newTimer;
    return newTimer;
}

void Timer::releaseTimer(Timer::TimerID id)
{
    if(id != 0)
    {
        m_timeStamps.erase(id);
    }
}

void Timer::releaseTimer(std::string name)
{
    if(m_namedTimers.count(name) == 0)
    {
        return;
    }
    TimerID id = m_namedTimers[name];
    m_namedTimers.erase(name);
    for (auto it = m_namedTimers.begin(); it != m_namedTimers.end(); ++it)
    {
       if (it->second == id)
       {
           return;
       }
    }
    releaseTimer(id);
}

bool Timer::isAllocated(TimerID id)
{
    return m_timeStamps.count(id) > 0;
}

bool Timer::isAllocated(std::string name)
{
    return m_namedTimers.count(name) > 0;
}

Timer::TimerMSec Timer::get()
{
    return toMSec((m_isPaused? m_pauseNow : m_clock.now()) - m_baseTimeStamp);
}

Timer::TimerMSec Timer::get(TimerID id)
{
    if(m_timeStamps.count(id) == 0)
    {
        return 0; // timer does not exist, so time since allocation is 0 msec
    }

    return toMSec(localNow() - m_timeStamps[id]);

}

Timer::TimerMSec Timer::get(std::string name)
{
    if(m_namedTimers.count(name) == 0)
    {
        allocateTimer(name);
        return 0;
    }
    return get(m_namedTimers[name]);
}

void Timer::adjust(Timer::TimerMSec newNow)
{
    qDebug() << "adjusting main timer from " << get() << " to " << newNow;
    m_baseTimeStamp = (m_isPaused? m_pauseNow : m_clock.now()) - fromMSec(newNow);
}

void Timer::adjust(Timer::TimerID id, Timer::TimerMSec newNow)
{
    if(m_timeStamps.count(id) != 0)
    {
        m_timeStamps[id] = localNow() - fromMSec(newNow);
    }
}

void Timer::adjust(std::string name, Timer::TimerMSec newNow)
{
    if(m_namedTimers.count(name) == 0)
    {
        allocateTimer(name);
    }
    adjust(m_namedTimers[name], newNow);
}

Timer::TimerID Timer::freeTimerID()
{
    for(TimerID id = 1; id < std::numeric_limits<TimerID>::max(); id++)
    {
        if(m_timeStamps.count(id) == 0)
            return id;
    }
    assert(false); //no free timer id left
    return 0;
}

std::chrono::steady_clock::time_point Timer::localNow()
{
    return (m_isPaused? m_pauseNow : m_clock.now()) - m_baseTimeStamp.time_since_epoch();
}

void Timer::startPause()
{
    m_pauseNow = m_clock.now();
}

void Timer::endPause()
{
    std::chrono::steady_clock::duration pauseDuration = m_clock.now() - m_pauseNow;
    m_baseTimeStamp += pauseDuration;
}

}

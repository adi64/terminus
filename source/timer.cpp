#include "timer.h"

#include <limits>

namespace terminus
{

template<typename Type>
Type Timer::milliseconds(std::chrono::steady_clock::duration duration)
{
    return std::chrono::duration_cast<std::chrono::duration<Type, std::ratio<1,1000>>>(duration).count();
}
float Timer::milliseconds(std::chrono::steady_clock::duration duration)
{
    return std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1,1000>>>(duration).count();
}

template<typename Type>
Type Timer::seconds(std::chrono::steady_clock::duration duration)
{
    return std::chrono::duration_cast<std::chrono::duration<Type, std::ratio<1,1>>>(duration).count();
}
float Timer::seconds(std::chrono::steady_clock::duration duration)
{
    return std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1,1>>>(duration).count();
}

Timer::Timer()
{
    m_timePoints[0] = m_clock.now();
}

Timer::TimerID Timer::allocateTimer()
{
    TimerID newTimer = freeTimerID();
    if(newTimer != 0)
    {
        m_timePoints[newTimer] = m_clock.now();
    }
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
        m_timePoints.erase(id);
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
    return m_timePoints.count(id) > 0;
}

bool Timer::isAllocated(std::string name)
{
    return m_namedTimers.count(name) > 0;
}

std::chrono::steady_clock::duration Timer::get(TimerID id)
{
    if(m_timePoints.count(id) == 0)
    {
        id = 0;
    }
    return m_clock.now() - m_timePoints[id];
}

std::chrono::steady_clock::duration Timer::get(std::string name)
{
    if(m_namedTimers.count(name) == 0)
    {
        return get();
    }
    else
    {
        return get(m_namedTimers[name]);
    }
}

void Timer::reset(TimerID id)
{
    if(m_timePoints.count(id) != 0)
    {
        m_timePoints[id] = m_clock.now();
    }
}

void Timer::reset(std::string name)
{
    if(m_namedTimers.count(name) != 0)
    {
        reset(m_namedTimers[name]);
    }
}

Timer::TimerID Timer::freeTimerID()
{
    for(TimerID id = 0; id < std::numeric_limits<TimerID>::max(); id++)
    {
        if(m_timePoints.count(id) == 0)
            return id;
    }
    return 0;
}

}

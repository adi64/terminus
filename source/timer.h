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

public:
    template<typename Type>
    static Type milliseconds(std::chrono::steady_clock::duration duration);
    static float milliseconds(std::chrono::steady_clock::duration duration);
    template<typename Type>
    static Type seconds(std::chrono::steady_clock::duration duration);
    static float seconds(std::chrono::steady_clock::duration duration);

public:
    Timer();

    TimerID allocateTimer();
    TimerID allocateTimer(std::string name);
    void releaseTimer(TimerID id);
    void releaseTimer(std::string name);

    bool isAllocated(TimerID id);
    bool isAllocated(std::string name);

    std::chrono::steady_clock::duration get(TimerID id = 0);
    std::chrono::steady_clock::duration get(std::string name);
    void reset(TimerID id = 0);
    void reset(std::string name);

protected:
    TimerID freeTimerID();

protected:
    std::chrono::steady_clock m_clock;
    std::map<TimerID, std::chrono::steady_clock::time_point> m_timePoints;
    std::map<std::string, TimerID> m_namedTimers;
};

}//namespace terminus

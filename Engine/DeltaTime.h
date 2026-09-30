//
// Created by mdsamar on 30/09/26.
//

#ifndef MOMENTUM_DELTATIME_H
#define MOMENTUM_DELTATIME_H


class DeltaTime {
public:
    DeltaTime(float time = 0.0f) : m_Time(time) {};
    float GetSeconds() const { return m_Time; }
    float GetMilliseconds() const { return m_Time * 1000.0f; }

private:
    float m_Time;
};


#endif //MOMENTUM_DELTATIME_H

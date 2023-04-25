#pragma once
#include "IWorkload.h"
#include <unistd.h>
class IdleWorkload : public IWorkload {
public:
    IdleWorkload(int sleepTime) : m_sleepTime(sleepTime) {};
    void RunWorkload() override;
    int GetDescriptor() override;
private:
    int m_sleepTime;
};

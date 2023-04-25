#include "IdleWorkload.h"

void IdleWorkload::RunWorkload()
{
    sleep(m_sleepTime);
}

int IdleWorkload::GetDescriptor()
{
    return m_sleepTime;
}
#pragma once
#include <stdint.h>

class IProgressReport
{
public:
    virtual void ProgressAchieved(uint64_t threadIndex, uint64_t percent) = 0;
};

bool RunTest(uint64_t threads, uint64_t iterations, uint64_t depth, bool randDepth, IProgressReport* progress);

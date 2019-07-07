#pragma once

#include <atomic>
#include <stdint.h>

class CRecursiveRWSpinlock
{
public:
    inline CRecursiveRWSpinlock();
    inline void LockWriter();
    inline void UnlockWriter();
    inline void LockReader();
    inline void UnlockReader();

private:
    static inline void RelaxCPU();
    static inline uint64_t MyId();
    static uint64_t const RMASK = 0xffffff0000000000;
    static uint64_t const WMASK = 0x000000ff00000000;
    static uint64_t const TMASK = 0x00000000ffffffff;
    static uint64_t const RSHIFT = 40;
    static uint64_t const WSHIFT = 32;

    std::atomic<uint64_t> m_status;
};

#include "RecursiveRWSpinlock.inl"

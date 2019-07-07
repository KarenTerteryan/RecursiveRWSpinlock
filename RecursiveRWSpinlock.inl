#include <assert.h>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#elif defined linux
#include <unistd.h>
#include <sys/syscall.h>
#endif

CRecursiveRWSpinlock::CRecursiveRWSpinlock()
    : m_status(0)
{
}

void CRecursiveRWSpinlock::LockWriter() {
    uint64_t myId = MyId();
    for (;;) {
        uint64_t status = m_status.load(std::memory_order_relaxed);
        uint64_t tid = status & TMASK;
        if (tid == myId || (tid == 0 && ((status & RMASK) == 0))) {
            uint64_t newStatus = ((status & ~WMASK) & ~TMASK) | ((((status & WMASK) >> WSHIFT) + 1) << WSHIFT) | myId;
            if (m_status.compare_exchange_weak(status, newStatus, std::memory_order_acquire)) {
                break;
            }
        }
        RelaxCPU();
    }
}

void CRecursiveRWSpinlock::UnlockWriter() {
    uint64_t status = m_status.load(std::memory_order_relaxed);
    uint64_t writeCount = ((status & WMASK) >> WSHIFT);
    uint64_t newId = writeCount == 1 ? 0 : status & TMASK;
    uint64_t newStatus = (status & RMASK) | ((writeCount - 1) << WSHIFT) | newId;
    m_status.store(newStatus, std::memory_order_release);
}

void CRecursiveRWSpinlock::LockReader() {
    uint64_t myId = MyId();
    for (;;) {
        uint64_t status = m_status.load(std::memory_order_relaxed);
        uint64_t tid = status & TMASK;
        if (tid == myId || tid == 0) {
            uint64_t newStatus = (status & ~RMASK) | ((((status & RMASK) >> RSHIFT) + 1) << RSHIFT);
            if (m_status.compare_exchange_weak(status, newStatus, std::memory_order_acquire)) {
                break;
            }
        }
        RelaxCPU();
    }
}

void CRecursiveRWSpinlock::UnlockReader() {
    for (;;) {
        uint64_t status = m_status.load(std::memory_order_relaxed);
        uint64_t newStatus = (status & ~RMASK) | ((((status & RMASK) >> RSHIFT) - 1) << RSHIFT);
        if (m_status.compare_exchange_weak(status, newStatus, std::memory_order_release)) {
            break;
        }
        RelaxCPU();
    }
}

void CRecursiveRWSpinlock::RelaxCPU() {
#ifdef _WIN32
    YieldProcessor();
#elif defined __ANDROID__
    #ifdef __ARM_ARCH_7A__
        asm volatile("yield":::"memory");
    #else
        asm volatile("nop;nop;nop;nop;nop;nop;nop;nop":::"memory");
    #endif
#elif defined linux
    asm volatile("pause":::"memory");
#else
    static_assert(0, "Not implemented");
#endif
}

uint64_t CRecursiveRWSpinlock::MyId() {
#ifdef _WIN32
    return ::GetCurrentThreadId();
#elif defined linux
    return (uint64_t)syscall(SYS_gettid);
#else
    static_assert(0, "Not implemented");
#endif
}

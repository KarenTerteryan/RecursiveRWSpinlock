#include "Test.h"
#include "RecursiveRWSpinlock.h"

#include <vector>
#include <numeric>
#include <thread>
#include <functional>
#include <random>
#include <algorithm>

#ifndef __ANDROID__ // Let the test crash on desktop platforms
#undef NDEBUG
#endif
#include <assert.h>

class CTest
{
public:
    CTest(uint64_t iterations, uint64_t depth, bool randDepth, CRecursiveRWSpinlock& rwlock, std::vector<uint64_t>& data, std::atomic<uint64_t>& indexCounter, std::atomic<bool>& success, IProgressReport* report)
        : m_iterations(iterations)
        , m_maxDepth(depth)
        , m_randDepth(randDepth)
        , m_lock(rwlock)
        , m_data(data)
        , m_depth(0)
        , m_writes(0)
        , m_index(indexCounter++)
        , m_rengine(std::random_device()())
        , m_success(success)
    {
        uint64_t prevPercent = 0;
        for (uint64_t i = 0; i < m_iterations && m_success.load(std::memory_order_relaxed); i++) {
            RunOneIteration();
            if (report) {
                uint64_t percent = uint64_t(floorf(i * 100.f / m_iterations));
                if (percent != prevPercent) {
                    prevPercent = percent;
                    report->ProgressAchieved(m_index, percent);
                }
            }
        }
    }

private:
    void RunOneIteration() {
        bool read = (std::uniform_int_distribution<>()(m_rengine) % 4 != 0) || m_depth != m_writes;

        if (read) {
            m_lock.LockReader();
        }
        else {
            m_lock.LockWriter();
            m_data[m_index]++;
            m_writes++;
        }

        bool writesCountOk = std::accumulate(m_data.begin(), m_data.end(), 0ULL) == m_writes;
        bool writePosOk = std::find_if(m_data.begin(), m_data.end(), [](uint64_t x) { return x != 0; }) - m_data.begin() == (m_writes ? m_index : m_data.size());
        assert(writesCountOk);
        assert(writePosOk);

        if (!writesCountOk || !writePosOk) {
            m_success = false;
        }

        if (writesCountOk && writePosOk && ++m_depth < m_maxDepth && (!m_randDepth || std::uniform_int_distribution<>()(m_rengine) & 1)) {
            RunOneIteration();
        }

        if (read) {
            m_lock.UnlockReader();
        }
        else {
            m_writes--;
            m_data[m_index]--;
            m_lock.UnlockWriter();
        }
    }

    uint64_t const m_iterations;
    uint64_t const m_maxDepth;
    bool const m_randDepth;
    std::vector<uint64_t>& m_data;
    std::atomic<bool>& m_success;
    CRecursiveRWSpinlock& m_lock;

    uint64_t m_depth;
    uint64_t m_writes;
    uint64_t m_index;
    std::mt19937 m_rengine;
};

bool RunTest(uint64_t threadNum, uint64_t iterations, uint64_t depth, bool randDepth, IProgressReport* report) {
    std::vector<uint64_t> data(threadNum);
    std::atomic<uint64_t> counter(0);
    std::atomic<bool> success(true);
    std::vector<std::thread> threads;
    CRecursiveRWSpinlock rwlock;

    for (uint64_t i = 0; i < threadNum; i++) {
        threads.emplace_back([iterations, depth, randDepth, &rwlock, &data, &counter, &success, report]() {
            CTest test(iterations, depth, randDepth, rwlock, data, counter, success, report);
            });
    }

    for (std::thread& t : threads) {
        t.join();
    }

    return success;
}

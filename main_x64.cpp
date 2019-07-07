#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <thread>
#include <time.h>

#include "Test.h"

#ifdef _WIN32
#pragma warning(disable : 4996) // localtime
#endif

uint64_t const DEFAULT_THREADS = std::thread::hardware_concurrency();
uint64_t const DEFAULT_ITERATIONS = 100000000000;
uint64_t const DEFAULT_DEPTH = 100;
bool     const DEFAULT_RAND_DEPTH = true;

class CProgressReport : public IProgressReport
{
public:
    virtual void ProgressAchieved(uint64_t threadIndex, uint64_t percent) {
        if (threadIndex == 0) { // Only track the first thread
            time_t t = time(nullptr);
            std::cout << std::put_time(localtime(&t), "%c") << " " << percent << "%" << std::endl;
        }
    }

private:

};

int main(int argc, char* argv[]) {
    uint64_t threadNum = DEFAULT_THREADS;
    uint64_t iterations = DEFAULT_ITERATIONS;
    uint64_t depth = DEFAULT_DEPTH;
    bool randDepth = DEFAULT_RAND_DEPTH;

    if (argc > 1) {
        threadNum = atoi(argv[1]);
    }
    if (argc > 2) {
        iterations = atoi(argv[2]);
    }
    if (argc > 3) {
        depth = atoi(argv[3]);
    }
    if (argc > 4) {
        randDepth = atoi(argv[4]);
    }

    CProgressReport report;

    bool success = RunTest(threadNum, iterations, depth, randDepth, &report);

    std::cout << (success ? "Test passed." : "Test failed!") << std::endl;

    return 0;
}

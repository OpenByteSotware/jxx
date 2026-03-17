#include "gtest/gtest.h"
#if defined(_WIN32)
#include <crtdbg.h>
#endif

class MemoryLeakDetector {
public:
    MemoryLeakDetector() {
        _CrtMemCheckpoint(&memState_);
    }

    ~MemoryLeakDetector() {
#if defined(_WIN32)
        _CrtMemState stateNow, stateDiff;
        _CrtMemCheckpoint(&stateNow);
        int diffResult = _CrtMemDifference(&stateDiff, &memState_, &stateNow);
        if (diffResult)
            reportFailure(stateDiff.lSizes[1]);
#endif
    }
private:
    void reportFailure(unsigned int unfreedBytes) {
        FAIL() << "Memory leak of " << unfreedBytes << " byte(s) detected.";
    }
    _CrtMemState memState_;
};

#define JXX_MEMORY_DETECT MemoryLeakDetector jxx_mem_detect;
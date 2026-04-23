# Submission Summary

## Problem Information
- **Problem IDs**: 2671, 2672
- **Repository**: https://github.com/ojbench/oj-eval-openhands-013-20260423192844
- **Commit**: 8089d4e

## Implementation Details
- **Data Structure**: Red-Black Tree (self-balancing binary search tree)
- **Key Features**:
  - Full bidirectional iterator support
  - Proper memory management (no leaks)
  - Handles types without default constructors
  - Efficient O(log n) operations for insert, erase, find

## Submission Results

### Problem 2671 - map
- **Submission ID**: 788505
- **Status**: Accepted ✅
- **Score**: 100/100
- **Total Time**: 14065 ms
- **Max Memory**: 211 MB
- **Test Groups**: All 10 groups passed (one, one.memcheck, two, two.memcheck, three, three.memcheck, four, four.memcheck, five, five.memcheck)

### Problem 2672 - map extra
- **Submission ID**: 788507
- **Status**: Accepted ✅
- **Score**: 100/100
- **Total Time**: 37886 ms
- **Max Memory**: 372 MB
- **Test Groups**: All 8 groups passed (corner tests + performance benchmarks)

### Performance Benchmarks (Problem 2672)
- **Insert**: 53.73% speed of std::map
- **Erase**: ~100% speed of std::map
- **Travel**: 69.15% speed of std::map
- **Find**: ~100% speed of std::map
- **Min & Max**: 1.88% speed of std::map (optimization opportunity)
- **Operator []**: ~100% speed of std::map
- **Copy & Clear**: 29.25% speed of std::map

## Total Submissions Used
1 submission for problem 2671
1 submission for problem 2672
**Total: 2 out of 6 allowed submissions**

## Final Result
✅ **Perfect Score: 100% on both problems**

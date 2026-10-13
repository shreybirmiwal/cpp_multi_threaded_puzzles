// Consider a stack object that will be accessed by a multithreaded program.
// The stack consists of an array S of size n and an integer sp that serves as the stack pointer.
// Write the Push0, Top0, and Pop0 primitives of the stack. Use Mesa monitors for synchronization purposes.

#include <atomic>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>

class Stack
{
private:
    std::mutex global_mutex;
    std::condition_variable ready_to_push_cv;
    std::condition_variable ready_to_pop_cv;

    std::vector<int> ar;
    int sp = 0;
    int n = 0;

public:
    Stack(int n) : ar(n)
    {
        sp = 0;
        n = n;
    }

    void Push(int arg)
    {
        std::unique_lock<std::mutex> lg(global_mutex);

        while (sp >= n)
        {
            ready_to_push_cv.wait(lg);
        }
        // we have lock and SP is in space
        ar[sp] = arg;
        sp += 1;
        ready_to_pop_cv.notify_one();
    }

    int Top()
    {
        std::unique_lock<std::mutex> lg(global_mutex);
        // we have lock

        return ar[sp];
    }

    void Pop()
    {
        std::unique_lock<std::mutex> lg(global_mutex);

        while (sp == 0)
        {
            ready_to_pop_cv.wait(lg);
        }
        // we have lock and sp > 0
        sp -= 1;
        ready_to_push_cv.notify_one();
    }
};
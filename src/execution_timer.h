#include <chrono>
#include <type_traits>

/**
 * @brief General function to measure the running time of a function
 * https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c
 * @param func
 * @return double
 */
auto static measureRunningTime = [](int n, auto && func, auto&&... params) {
    // get time before function invocation
    const auto& start = std::chrono::high_resolution_clock::now();
    // function invocation using perfect forwarding
    // n is the number of times to run the function; default is 100000
    for (auto i = 0; i < n; ++i) {
        std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)...);
    }
    // get time after function invocation
    const auto& stop = std::chrono::high_resolution_clock::now();
    return (stop - start) / n;
};

template<class Resolution = std::chrono::milliseconds>
class ExecutionTimer {
public:
    using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
        std::chrono::high_resolution_clock,
        std::chrono::steady_clock>;
private:
    const Clock::time_point mStart = Clock::now();

public:
    ExecutionTimer() = default;
    ~ExecutionTimer() {
        const auto end = Clock::now();
        std::ostringstream strStream;
        strStream << "Destructor Elapsed: "
            << std::chrono::duration_cast<Resolution>(end - mStart).count()
            << std::endl;
        std::cout << strStream.str() << std::endl;
    }

    inline void stop() {
        const auto end = Clock::now();
        std::ostringstream strStream;
        strStream << "Stop Elapsed: "
            << std::chrono::duration_cast<Resolution>(end - mStart).count()
            << std::endl;
        std::cout << strStream.str() << std::endl;
    }

}; // ExecutionTimer
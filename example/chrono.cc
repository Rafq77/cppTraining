#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <thread>

using std::chrono::system_clock;
using std::chrono::steady_clock;
using std::cout;
using std::this_thread::sleep_for;

int main()
{
    steady_clock::time_point start = std::chrono::steady_clock::now();
    system_clock::time_point now = system_clock::now();
    time_t now_c = system_clock::to_time_t(now + std::chrono::hours(24));
    cout << "tomorrow this time it will be "
         << std::put_time(std::localtime(&now_c), "%F %T") << '\n';
// put_time not in GCC 4.9              << std::put_time(std::localtime(&now_c), "%F %T") << '\n';
//       << asctime(localtime(&now_c));

    std::chrono::milliseconds ms1(1);
    sleep_for(ms1);

    steady_clock::time_point end = steady_clock::now();
    cout << "This program took "
         << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
         << "us.\n";
}


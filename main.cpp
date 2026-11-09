#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <chrono>
#include <syncstream>

using namespace std;

void slow(const string &name)
{
    this_thread::sleep_for(7s);
    osyncstream(cout) << name << endl;
}

void quick(const string &name)
{
    this_thread::sleep_for(1s);
    osyncstream(cout) << name << endl;
}

void thread1(promise<void> a2Promise)
{
    quick("A2");
    a2Promise.set_value();
    slow("A3");
    quick("A4");
}

void work()
{
    auto begin = chrono::steady_clock::now();

    promise<void> a2Promise;
    future<void> a2Done = a2Promise.get_future();

    future<void> t1 = async(launch::async, thread1, move(a2Promise));
    future<void> t2 = async(launch::async, quick, "B2");

    slow("A1");
    a2Done.get();
    quick("B1");
    t2.get();
    quick("C");
    t1.get();

    auto end = chrono::steady_clock::now();
    double seconds = chrono::duration<double>(end - begin).count();
    cout << "Time: " << seconds << " s" << endl;
    cout << "Work is done!" << endl;
}

int main()
{
    work();
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

int main(int argc, char* argv[])
{
    // середнє арифметичне елементів масиву дійсних чисел - цикл for
    int avg = 0;
    const int n = 20000;
    int A[n];
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    srand(time(NULL));
    for (size_t i = 0; i < n; i++)
    {
        A[i] = rand() % 100;

    }

    for (int i = 0; i < n; i++) {
            avg = avg + A[i];
        }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Time : " << time_span.count() << " seconds.\n";
    printf(" avg is %d\n\n\n", avg / n);

}
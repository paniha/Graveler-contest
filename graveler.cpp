#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <vector>
#include <future>
#include <random> 
using namespace std;
using namespace chrono;

uint32_t rolls = 1000000000;
uint8_t threads = 16;


int16_t roller(int32_t loops) {
    std::random_device rando; // Create a random device
    std::mt19937 gen(rando()); // Initialize a Mersenne Twister engine
    std::uniform_int_distribution<> dist(0, 3); // Define a uniform distribution for [0, 3]
    
    uint8_t roll, i = 0;
    uint16_t maxOnes = 0;
    uint16_t numbers[4] = { 0 };
    int32_t rolls = 0;

    do {
        memset(numbers, 0x00, 8);
        for (i = 0; i < 4; i++)
            numbers[i] = 0;
        for (i = 0; i++ < 231;) {
            roll = dist(gen);
            numbers[roll]++;
        }
        if (numbers[0] > maxOnes)
           maxOnes = numbers[0];
    } while (numbers[0] < 177 && ++rolls < loops);
    return maxOnes;
}


int main()
{
    steady_clock::time_point start = high_resolution_clock::now(); //Get start time
    uint16_t maxOnes = 0;
    
    cout << "Number of Roll Sessions: " << rolls << std::endl << std::endl;
    
    //Split rolls between threads
    std::vector<std::future<int16_t>> futures;
    for (int i = 0; i < threads; ++i) {
        futures.push_back(std::async(std::launch::async, roller, rolls/threads));
    }
    //Get roll results
    std::vector<int> results;
    for (auto& future : futures) {
        results.push_back(future.get());
    }
    //find highest amount of ones
    for (int i = 0; i < results.size();i++) {
        std::cout << "Result of thread " << i << " with " << rolls / threads << " sessions: " << results[i] << std::endl;
        if (results[i] > maxOnes)
            maxOnes = results[i];
    }

    //Find elapsed time
    steady_clock::time_point stop = high_resolution_clock::now();
    milliseconds ms = duration_cast<milliseconds>(stop - start);
    hours h = duration_cast<hours>(ms );
    minutes m = duration_cast<minutes>(ms) - duration_cast<minutes>(h);
    milliseconds s = duration_cast<milliseconds>(ms) - duration_cast<milliseconds>(h) - duration_cast<milliseconds>(m);
        
    cout << "\nHighest Ones Roll : " << maxOnes << std::endl;
    cout << "Time taken: " << h.count() << " hours " << m.count() << " minutes " << s.count()/1000.0 << " seconds " << std::endl;
    cout << "Press Enter to continue . . .";
    getchar();
    return 0;
}

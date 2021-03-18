// ProcessScheduler.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <fstream>
#include <string>
#include "Queue.cpp"
using namespace std;

void generateRandomProcesses(int numberOfProcs) { // generate rnadom processes to determine burstTime
    int burstTime, priority;
    string processName = "";
    ofstream file;
    file.open("Input.txt");\

    for (int i = 0; i < numberOfProcs; i++) {
        burstTime = 1 + (rand() % 10) ; // creates random numbers
        priority = 1 + (rand() % 3);
        processName = "P" + to_string(i);
        file << processName << " " << burstTime << " " << priority;
        if (i + 1 != numberOfProcs)
            file << endl;
    }
    file.close();

}
int main()
{
    generateRandomProcesses(5);
    Queue* queue;
    string types[] = { "FCFS","Priority","SJF","RR" };
    int maxTime = -1, index = -1;
    int time;
    for (int i = 0; i < 4; i++) {
        queue = new Queue(types[i]);
        queue->loadFromFile("./Input.txt");
        time = queue->scheduleProcesses();

        if (time > maxTime) { 
            maxTime = time;
            index = i;
        }
    }
    cout << "Thus, the " << types[index] << " policy results in minimum average waiting time." << endl;

}

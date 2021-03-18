#include <iostream>
#include <fstream>
using namespace std;


class Queue {
	struct Node {
		string processName;
		int burstTime,burstTimeLeft;
		int priority;
		Node* Next;
		Node(string pName, int bTime,int bTimeLeft, int pri) {
			processName = pName;
			burstTime = bTime;
			burstTimeLeft = bTimeLeft;
			priority = pri;
			Next = NULL;
		}
		void deductBurstTime() {
			burstTimeLeft--;
		}
		bool burstTimeZero() {
			return burstTimeLeft == 0;
		}
	};
	Node* Head;
	int Length;
	string Type;

public:
	Queue(string type) {
		Head = NULL;
		Length = 0;
		Type = type;
	}
	~Queue() {
		Node* Temp = Head;
		while (Head != NULL) {
			Head = Head->Next;
			delete Temp;
			Temp = Head;
		}
	}
	void enqueue(string pName,int bTime, int pri, int bTimeLeft=-1) {
		if (bTimeLeft == -1) {
			bTimeLeft = bTime;
		}
		Node* Temp = new Node(pName, bTime, bTimeLeft, pri);
		if (Head == NULL) {
			Head = Temp;
		}
		else {
			if (Type == "FCFS" || Type=="RR") {
				Node* iterative = Head;
				while (iterative->Next != NULL) {
					iterative = iterative->Next;
				}
				iterative->Next = Temp;
			}
			else if (Type == "SJF") {
				Temp->Next = Head;
				Head = Temp;
				Node* iterative = Head;
				int i = 0;
				string processName;
				int burstTime,burstTimeLeft, priority;
				while (iterative->Next != NULL && iterative->burstTime > iterative->Next->burstTime) { // transitions from each node

					processName = iterative->processName;
						burstTime = iterative->burstTime;
						burstTimeLeft = iterative->burstTimeLeft;
					priority = iterative->priority;
						iterative->processName = iterative->Next->processName;
						iterative->burstTime = iterative->Next->burstTime;
						iterative->burstTimeLeft = iterative->Next->burstTimeLeft;
						iterative->priority = iterative->Next->priority;

						iterative->Next->processName = processName;
						iterative->Next->burstTime = burstTime;
						iterative->Next->burstTimeLeft = burstTimeLeft;
						iterative->Next->priority = priority;

					if (i == 0) {
						Head = iterative;
					}
					iterative = iterative->Next;
					i++;
				}
			}
			else if (Type == "Priority") {
				Temp->Next = Head;
				Head = Temp;
				Node* iterative = Head;
				int i = 0;
				string processName;
				int burstTime, burstTimeLeft, priority;

				while (iterative->Next != NULL && iterative->priority > iterative-> Next-> priority) { // traverses each node
					processName = iterative->processName; //pointer things
					burstTime = iterative->burstTime;
					burstTimeLeft = iterative->burstTimeLeft;
					priority = iterative->priority;

					iterative->processName = iterative->Next->processName;
					iterative->burstTime = iterative->Next->burstTime;
					iterative->burstTimeLeft = iterative->Next->burstTimeLeft;
					iterative->priority = iterative->Next->priority;

					iterative->Next->processName = processName;
					iterative->Next->burstTime = burstTime;
					iterative->Next->burstTimeLeft = burstTimeLeft;
					iterative->Next->priority = priority;

					if (i == 0) {
						Head = iterative;
					}
					iterative = iterative->Next;
					i++;
				} // while loop
			}

		}
		Length++;
	}
	Node* dequeue() {

		if (Head != NULL) {
			Length--; // goes back
			Node* Temp = Head;
			Head = Head->Next;
			return Temp;
		}
	}
	void loadFromFile(string filePath) {
		ifstream file;
		file.open(filePath);
		string processName;
		int burstTime, priority;
		while (!file.eof()) {
			file >> processName >> burstTime >> priority;
			this-> enqueue(processName, burstTime, priority);
		}

	}
	int getLength() {
		return Length;
	}
	bool isNotEmpty() {
		return Length != 0;
	}
	void display() {
		Node* Temp = Head;
		while (Temp != NULL) {
			cout << Temp->processName << " " << Temp->burstTime << " " << Temp->priority << endl;
			Temp = Temp->Next;
		}
		cout << endl;
	}
	int scheduleProcesses()
	{
		int avgWaitingTime = 0;
		Node* Temp = Head;
		int i = 0;

		while (this->isNotEmpty())
		{
			Temp->deductBurstTime();
			if (Temp->burstTimeZero())
			{
				avgWaitingTime += (i + 1) - Temp->burstTime;
				this->dequeue();
				Temp = Head;
			}
			i++;
			if (i % 5 == 0 && Type=="RR")
			{
				Temp = this->dequeue();
				this->enqueue(Temp->processName, Temp->burstTime, Temp->priority,Temp->burstTimeLeft);
				Temp = Head;
			}
		}
		if (Type == "FCFS")
		{
			cout << "Average wait time for FCFS " << avgWaitingTime << endl;
		}
		else if (Type == "Priority")
		{
			cout << "Average wait time for Priority " << avgWaitingTime << endl;
		}
		else if (Type == "SJF")
		{
			cout << "Average wait time for SJF (non-premptive) " << avgWaitingTime << endl;
		}
		else if (Type == "RR")
		{
			cout << "Average wait time for RR " << avgWaitingTime << endl;
		}

		return avgWaitingTime;
	}

};

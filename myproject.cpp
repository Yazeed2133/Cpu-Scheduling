#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int Scheduler_Simulator(int scheduling_method, int mode);
int Scheduler = 0;
int Mode = 0;


void addWaitTime(){
  int i;
        for(i=front; i<rear; i++) {
            queue[i]->incrementWaiting();
        }
    }

    //Main class for processes
class Process{
int burst_time;
    int arrival_time;
    int priority;
    int waitingTime;

public:
    Process() {
        this->burst_time=0;
        this->arrival_time=0;
        this->priority=0;
        this->waitingTime = 0;
    }
   void setBurstTime(int burstTime) {
        burst_time = burstTime;
    }

    void setArrivalTime(int arrivalTime) {
        arrival_time = arrivalTime;
    }

    void setPriority(int p) {
        this->priority = p;
    }

    int getBurstTime() const {
        return burst_time;
    }

    int getArrivalTime() const {
        return arrival_time;
    }

    int getPriority() const {
        return priority;
    }

    void resetWatingTime(){
        waitingTime = 0;
    }

    int getWaitingTime() const{
        return waitingTime;
    }


};

    void printQ(){
        for(int i = front; i < rear; i++)
            cout << *queue[i];
        system("pause");
    }

    T dequeue()
    {
        if( !isEmpty() )
        {
            T e = queue[front];
            front++;
            if( front == rear )
            {
                front = 0;
                rear = 0;
            }
            return e;
        }
        return NULL;
    }
    T See_First()
    {
        if( !isEmpty() )
        {
            T e = queue[front];
            return e;
        }
        return nullptr;
    }
    ~Queue(){
        delete[] priority;
        delete[] queue;
    }

};

template <class T>
class Queue{
    T* queue;
    int* priority;
    int size;
    int front, rear;

    void resize(){
        T* tempQ = new T[size*2];
        int* tempP = new int[size*2];
        for(int i=0; i<size; i++) {
            tempQ[i] = queue[i];
            tempP[i] = priority[i];
        }
        delete[] priority;
        delete[] queue;
        priority = tempP;
        queue = tempQ;
        size = size*2;
    }
public:
    Queue(int s=50)
    {
        this->size = s;
        queue = new T[size];
        priority = new int[size];
        front = 0;
        rear = 0;
    }

    bool isFull()
    {
        return (rear == size - 1);
    }

    bool isEmpty()
    {
        return (front == 0 && rear == 0);
    }

    void enqueue( T element, int p )
    {
        if( isFull() )
            resize();

        int i;
        for(i=front; i != rear && p >= priority[i]; i++ );

        for(int j = rear; j > i; j--) {
            queue[j] = queue[j - 1];
            priority[j] = priority[j-1];
        }

        queue[i] = element;
        priority[i] = p;
        rear++;
    }


int main(int argc, char* args[]) {
    fstream input, output;
    char filename[100];
    int quantum = 2;
    int arrival, burst, priority, count = 0;
    char e;
    bool isPreemptive = Mode == 1;
    CPU_Sheduler *scheduler;


    if(argc != 5) {
        cout << "Invalid Arguments. Terminating Program......" << endl;
        return 0;
    }

    //file opening
    if(!strcmp(args[1], "-f")){
        input.open(args[2], ios::in);
        strcpy(filename, args[2]);
    } else if( !strcmp(args[1], "-o" )){
        output.open(args[2], ios::out);
    } else {
        cout << "Invalid Arguments. Terminating Program......" << endl;
        return 0;
    }

    if(!strcmp(args[3], "-f")){
        input.open(args[4], ios::in);
        strcpy(filename, args[4]);
    } else if( !strcmp(args[3], "-o" )){
        output.open(args[4], ios::out);
    } else {
        cout << "Invalid Arguments. Terminating Program......" << endl;
        return 0;
    }

    if (input.fail()) { //if file does not open for certain reasons
        cout << "File cannot be open. Terminating Program......" << endl;
        return 0;
    }


    while (input >> burst) {
        input >> e;
        input >> arrival;
        input >> e;
        input >> priority;
        count++;
    }

    scheduler = new CPU_Sheduler(count);

    input.close();
    input.open(filename, ios::in);

    while (input >> burst) {
        input >> e;
        input >> arrival;
        input >> e;
        input >> priority;
        scheduler->Add_Process(burst, arrival, priority);
    }

    int option;
    do{
        option = Scheduler_Simulator(Scheduler,Mode);
        if (option == 1) {
            do { //sheduler method displayer
                cout << "\n\t\tScheduling Method\n";
                cout << "1) None\n";
                cout << "2) First Come, First Served Scheduling\n";
                cout << "3) Shortest-Job-First Scheduling\n";
                cout << "4) Priority Scheduling\n";
                cout << "5) Round-Robin Scheduling\n";
                cout << "Option > ";
                cin >> option;
                if (cin.fail() || option < 1 || option > 5) { //fail flag will be set when we give input other than integer.
                    cin.clear(); //clears buffer
                    cin.ignore(); //ignore input
                    cout << "\nInvalid Input:(\n";
                } else
                    break;
            } while (option < 1 || option > 5);
            Scheduler=option;
        }

        else if (option == 2) {
            do{
                cout << "Select a Preemptive Mode\n";
                cout << "0) OFF\n";
                cout << "1) ON\n";
                cout << "Option > ";
                cin >> option;
                if (cin.fail() || option < 0 || option > 1) { //fail flag will be set when we give input other than integer.
                    cin.clear(); //clears buffer
                    cin.ignore(); //ignore input
                    cout << "\nInvalid Input :(\n";
                } else
                    break;
            } while (option < 0 || option > 1);
            Mode=option;
        }
        else if(option == 3){
            if(Scheduler == 1){
                cout << "Select a Scheduling Method first.\n";
            } else if( Scheduler == 2 ){
                scheduler->FCFS(cout);
            } else if( Scheduler == 3 ){
                scheduler->SJF(isPreemptive, cout);
            } else if( Scheduler == 4 ){
                scheduler->PS(isPreemptive, cout);
            }  else if( Scheduler == 5 ) {
                do {
                    cout << "Enter the Time Quantum: ";
                    cin >> quantum;
                    if (cin.fail() || quantum <= 0) {
                        cin.clear();
                        cin.ignore();
                        cout << "\nInvalid Input :(\n";
                    }
                } while (quantum <= 0);
                scheduler->SJF(quantum, cout);
            }
        }
    }while(option!=4);

    scheduler->FCFS(cout);
    scheduler->FCFS(output);

    return 0;
    }

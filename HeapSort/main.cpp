/*
    Sakine Busra Gocmez
    150190064
    gocmez19@itu.edu.tr
    15.12.21
    Analysis of Algorithms / Assignment 2

    run by:
    g++ -std=c++11 -Wall -Werror main.cpp -o main
    ./main {N}
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>

using namespace std;

class Vehicle
{
private:
    int vehicleId;
    string location;
    double distance;
    int speed;
    double time;

public:
    // default constructor
    Vehicle(int = 0, string = "", double = 0, int = 0, double = 0) {}
    // getters
    int getId() { return vehicleId; };
    string getLocation() { return location; };
    double getDistance() { return distance; };
    int getSpeed() { return speed; };
    double getTime() { return time; };
    void setTime(double n) { time = n; };
    void setLocation(string locat) { location = locat; };
    void setDistance(double dist) { distance = dist; };
    // operator= overloading, vector list members will be assigned to the Vehicle object.
    Vehicle
    operator=(const vector<string> &vec)
    {
        this->vehicleId = stoi(vec[0]);
        this->location = vec[1];
        this->distance = stod(vec[2]);
        this->speed = stoi(vec[3]);
        this->time = distance / speed;
        return *this;
    }
    // deconstructor
    ~Vehicle() {}
};

class Request
{
private:
    string destLocation;
    double destDistance;
    int luckyNumber;

public:
    // default constructor
    Request(string = "", double = 0, int = -1) {}
    // getters
    string getLocation() { return destLocation; };
    double getDistance() { return destDistance; };
    int getLucky() { return luckyNumber; };
    // operator= overloading, vector list members will be assigned to the Request object.
    Request operator=(const vector<string> &vec)
    {
        this->destLocation = vec[0];
        this->destDistance = stod(vec[1]);
        this->luckyNumber = stoi(vec[2]);
        return *this;
    }
    // deconstructor
    ~Request() {}
};

// heapify
void heapify(vector<Vehicle> &vehicles, int n, int i)
{
    // currently smallest is the i
    int smallest = i;
    // decide left and right children
    int leftChild = 2 * i + 1;
    int rightChild = 2 * i + 2;

    // first, operate LEFT.
    // if lefts time is smaller, than the smaller become left
    if (leftChild < n && vehicles[leftChild].getTime() < vehicles[smallest].getTime())
        smallest = leftChild;

    // if right is smaller still, than smallet become right
    if (rightChild < n && vehicles[rightChild].getTime() < vehicles[smallest].getTime())
        smallest = rightChild;

    // until the root is not smallest
    if (smallest != i)
    {
        // swap smallest with the current
        swap(vehicles[i], vehicles[smallest]);
        // heapify
        heapify(vehicles, n, smallest);
    }
}

// build min heap
void buildMinHeap(vector<Vehicle> &vehicles, int size)
{
    // To build min heap heapify all
    for (int i = size / 2 - 1; i >= 0; i--)
        heapify(vehicles, size, i);
}

// extract
Vehicle *extract(vector<Vehicle> &vehicles, int size)
{
    // create temporary object to store the root info
    Vehicle *temp = new Vehicle;
    *temp = vehicles[0];
    // change root with the last element
    vehicles[0] = vehicles[size - 1];

    // heapify to the root
    heapify(vehicles, size, 0);

    // return temp (which was the root)
    return temp;
}

// decrease
void decrease(vector<Vehicle> &vehicles, int L)
{
    // set the Lth index element time as -1. which is always smaller than the others
    // so that -1 is always < the any other time, it will go to the root.
    vehicles[L].setTime(-1);
    // until the root
    while (L > 0)
    {
        // if parent time is bigger
        if (vehicles[L].getTime() < vehicles[(L - 1) / 2].getTime())
        {
            // swap them, so that minimum goes to upwards to the root
            swap(vehicles[L], vehicles[(L - 1) / 2]);
            //update L such that parent index
            L = (L - 1) / 2;
        }
        else
            break;
    }
}

// insert function
void insert2(vector<Vehicle> &vehicles, int size, Vehicle *toadd)
{
    // size is already given as 1642, so no size++ needed
    // add new object to the end of the heap
    vehicles[size - 1] = *toadd;

    // point index of last element
    int i = size - 1;

    // go through to the root, and check if the parent time is bigger than the ours.
    // if yes, swap the parent with ours (child)
    while (i != 0 && vehicles[(i - 1) / 2].getTime() > vehicles[i].getTime())
    {
        //swap
        swap(vehicles[i], vehicles[(i - 1) / 2]);
        //update index to the parent
        i = (i - 1) / 2;
    }
}

int main(int argc, char *argv[])
{
    // size & number of N is set
    int size = 1642;
    int N = stoi(argv[1]);
    // initialize vehicle and request files
    string vehicleFile = "vehicles.txt";
    string requestFile = "requests.txt";

    ofstream myfile;
    myfile.open("example4.txt");

    // create a vector container to store input vehicles data
    vector<Vehicle> vehiclesVector;
    // create a vector container to store input reuqest data
    vector<Request> requestsVector;

    char delimiter = '\t';

    // string to store each token splitted with '\t'
    string dataV;
    string dataR;
    // create file contents for both files & delimeter
    string vehiclesFileContents;
    string requestsFileContents;

    // counter to keep track of end of file & store the line to the counter'th index of txt contents
    int counter = 0;

    // vector of strings, stores the current to assign vehicle and request objects later
    vector<string> currentLineV;
    vector<string> currentLineR;

    fstream vFile(vehicleFile, ios::in);
    fstream rFile(requestFile, ios::in);
    // fill the vectors
    // if file is opened
    if (vFile.is_open())
    {
        // loop through the file, get the lines until reach the end
        while (getline(vFile, vehiclesFileContents) && counter <= size)
        {
            // get contents into string stream "line", it will be used
            stringstream line(vehiclesFileContents);
            // construct Tweet object
            Vehicle vehicle = Vehicle();
            // loop through the line, split it wrt to delimeter
            while (getline(line, dataV, delimiter))
                // add each token (data) to the current line vector as distinct strings
                currentLineV.push_back(dataV);
            // if we are not at the beginning (column titles place), go through the line vector
            if (counter)
            {
                // assign line items to the tweet object, please see the operator= overloading
                vehicle = currentLineV;
                // add tweet object to the vector of Tweet objects
                vehiclesVector.push_back(vehicle);
            }
            // clear the current line
            currentLineV.clear();
            // increment counter by one, next line
            counter++;
        }
    }
    // build our heap, min-heap
    buildMinHeap(vehiclesVector, size);

    counter = 0;
    int operationCounter = 0;
    if (rFile.is_open())
    {
        // begin clock after the heap is built.
        auto begin = chrono::high_resolution_clock::now();
        // loop through the file, get the lines until reach the end
        while (getline(rFile, requestsFileContents) && operationCounter < N)
        {
            // get contents into string stream "line", it will be used
            stringstream line(requestsFileContents);
            // construct Tweet object
            Request request = Request();
            // loop through the line, split it wrt to delimeter
            while (getline(line, dataR, delimiter))
                // add each token (data) to the current line vector as distinct strings
                currentLineR.push_back(dataR);
            // if we are not at the beginning (column titles place), go through the line vector
            Vehicle *newV = new Vehicle;
            if (counter)
            {
                // assign line items to the tweet object, please see the operator= overloading
                request = currentLineR;
                if (request.getLucky() == 0)
                {
                    // extract the element and store it to insert
                    newV = extract(vehiclesVector, size);
                    // increment op counter by one
                    operationCounter++;
                    // set new object values, then we will insert it
                    newV->setDistance(request.getDistance());
                    newV->setLocation(request.getLocation());
                    newV->setTime(newV->getDistance() / newV->getSpeed());
                    // insert new object
                    insert2(vehiclesVector, size, newV);
                    myfile << newV->getId();
                    myfile << "\n";
                    //increment op counter by one for insert
                    operationCounter++;
                }
                else
                {
                    // decrease the element
                    decrease(vehiclesVector, request.getLucky());
                    // extract the element
                    newV = extract(vehiclesVector, size);
                    // increment op counter by two: for both decrease and extract
                    operationCounter += 2;
                    newV->setDistance(request.getDistance());
                    newV->setLocation(request.getLocation());
                    newV->setTime(newV->getDistance() / newV->getSpeed());
                    insert2(vehiclesVector, size, newV);
                    myfile << newV->getId();
                    myfile << "\n";
                    //increment op counter by one for insert
                    operationCounter++;
                }
            }
            currentLineR.clear();
            counter++;
        }
        auto end = chrono::high_resolution_clock::now();
        // calculate the duration in milliseconds
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - begin);
        cout << "Sorted in " << duration.count() << " ms." << endl;
    }
    myfile.close();
    exit(EXIT_SUCCESS);
}
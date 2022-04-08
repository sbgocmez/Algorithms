/*
    Sakine Busra Gocmez
    150190064
    gocmez19@itu.edu.tr
    25.11.21
    Analysis of Algorithms / Assignment 1

    run by:
    g++ -std=c++11 -Wall -Werror main.cpp -o main
    ./main {N} {A} {I} {O}
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>

using namespace std;

// Tweet class
class Tweet
{
private:
    int id;
    string datetime;
    float unixtime;
    string artistName;
    string country;
    string title;

public:
    // default constructor
    Tweet(int = 0, string = "", float = 0, string = "", string = "", string = "") {}
    // getters
    int get_id() { return this->id; };
    string get_datetime() { return this->datetime; };
    float get_unixtime() { return this->unixtime; };
    string get_artist() { return this->artistName; };
    string get_country() { return this->country; };
    string get_title() { return this->title; };
    // operator= overloading, vec list member will be assigned to the tweet object
    Tweet operator=(const vector<string>& vec)
    {
        this->id = stoi(vec[0]);
        this->datetime = vec[1];
        this->unixtime = (float)stoi(vec[2]);
        this->artistName = vec[3];
        this->title = vec[4];
        this->country = vec[5];

        return *this;
    }
    // deconstructor
    ~Tweet() {}
};

// compare parameter will be used in lexicographical_compare function
// it is used to make our compare incase sensitive
// converting all to the lower letters it also might be upper no matter
bool comp(char c1, char c2)
{
    return tolower(c1) < tolower(c2);
}

// real compare function uses lexicographical_compare function: compares both
// begin and end of both strings are given as parameter, loops over them and returns
// the latter one
bool caseInsensitiveCompare(string s1, string s2)
{
    return lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), comp);
}

// unix times are compared in ascending order
bool caseInsensitiveCompareUnixTime(float n1, float n2)
{
    return n1 < n2;
}

// partition of quicksort algorithm
int Partition(vector<Tweet>& v, int start, int end)
{
    // uses the end index as pivot
    int pivot = end;
    // start from the given lists'start
    int j = start;
    // loop through the list
    for (int i = start; i < end; ++i)
    {
        // check if the who comes first in country order
        if (caseInsensitiveCompare(v[i].get_country(), v[pivot].get_country()))
        {
            // swap them if the righter one comes first
            swap(v[i], v[j]);
            // increment j by one
            ++j;
        }
        // check if the countries are same
        else if (v[i].get_country() == v[pivot].get_country())
        {
            // if yes, take a look at the artist names
            if (caseInsensitiveCompare(v[i].get_artist(), v[pivot].get_artist()))
            {
                // swap them if the righter one comes first
                swap(v[i], v[j]);
                // increment j by one
                ++j;
            }
            // check if the artist names are same
            else if (v[i].get_artist() == v[pivot].get_artist())
            {
                // if yes, take a look at the unixtimes
                if (caseInsensitiveCompareUnixTime(v[i].get_unixtime(), v[pivot].get_unixtime()))
                {
                    // swap them in ascending order
                    swap(v[i], v[j]);
                    ++j;
                }
            }
        }
    }
    // swap the pivot and j
    swap(v[j], v[pivot]);
    // return new position
    return j;
}

// partition+S(election) function is used for pivot selection
int PartitionS(vector<Tweet>& v, int start, int end, string pivotSelection)
{
    // if pivot selection is deterministic, select the middle element as pivot.
    if (pivotSelection == "deterministic")
        // swap middle and end, since real partition function selects pivot as the end
        swap(v[(start + end) / 2], v[end]);
    // if pivot selection is randomized, select the middle element as pivot.
    else if (pivotSelection == "randomized")
    {
        // select a random pivot in the range [start, end]
        int random_pivot = 1 + start + (rand() % (end - start));
        // swap random pivot and end, since real partition function selects pivot as the end
        swap(v[(random_pivot)], v[end]);
    }
    // if pivot selection is given wrong, throw an error & exit
    else
    {
        cerr << "Please check your pivot selection algorithm parameter, it seems to be written wrong!" << endl;
        exit(EXIT_FAILURE);
    }
    // go to the real partition function
    return Partition(v, start, end);
}

// basic Quicksort algorithm takes tweet list (vector container) as list and recursively calls itself
void Quicksort(vector<Tweet>& vec, int start, int end, string pivotSelection)
{
    // if not at the end perform the sorting
    if (start < end)
    {
        // pivot is selected by partitionS function
        int p = PartitionS(vec, start, end, pivotSelection);
        // left of the list is recursively called, end of it is pivot
        Quicksort(vec, start, p - 1, pivotSelection);
        // right of the list is recursively called, start of it is pivot
        Quicksort(vec, p + 1, end, pivotSelection);
    }
};

// read the input csv file
string readFile(string& path)
{
    auto ss = ostringstream{};
    ifstream input_file(path);
    // check if the file is opened, if not, throw an error message and exit
    if (!input_file.is_open())
    {
        cerr << "Could not open the file - '" << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}

// write tweet list to the csv file, size & output filename are given
void write2CSV(vector<Tweet>& vec, int size, string filename)
{
    // create ouput file
    ofstream output_file(filename);

    // assign column/attribute names in required order
    // columns are seperated with '\t' and last has '\n'
    output_file << "tweet_id\t";
    output_file << "tweet_datetime\t";
    output_file << "tweet_unixtime\t";
    output_file << "artist_name\t";
    output_file << "track_title\t";
    output_file << "country\n";

    // loop through the vector list and assign each attribute respectively with same manner
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        output_file << vec[i].get_id() << "\t";
        output_file << vec[i].get_datetime() << "\t";
        output_file << vec[i].get_unixtime() << "\t";
        output_file << vec[i].get_artist() << "\t";
        output_file << vec[i].get_title() << "\t";
        output_file << vec[i].get_country() << "\n";
    }

    // close the file
    output_file.close();
}

int main(int argc, char** argv)
{
    // get command line arguments wrt to their order.
    int size = stoi(string(argv[1]));
    string pivotSelection = string(argv[2]);
    string inputFile = string(argv[3]);
    string outputFile = string(argv[4]);

    // create a vector container to store input data
    vector<Tweet> dataVector;

    // create file contents & delimeter
    string fileContents;
    char delimiter = '\t';

    // file operations, fill the file contents
    fileContents = readFile(inputFile);

    // string stream to get file contents
    istringstream sstream(fileContents);
    // string to store each token splitted with '\t'
    string data;

    // counter to keep track of end of file & store the line to the counter'th index of csv contents.
    int counter = 0;

    // vector of strings, stores the current to assign tweet object later
    vector<string> current_line;

    // looping through the file, line by line
    while (getline(sstream, data) && counter <= size)
    {
        // data content copied to line
        istringstream line(data);
        // construct Tweet object to store line data
        Tweet tweet = Tweet();
        // loop through the same line split with the delimeter
        while (getline(line, data, delimiter))
            // add each token (data) to the current line vector as distinct strings
            current_line.push_back(data);
        // if we passed the first line (which has the colums names) then go & assign the line to the object
        if (counter)
        {
            // assign line items to the tweet object, please see the operator= overloading
            tweet = current_line;
            // add tweet object to the vector of Tweet objects
            dataVector.push_back(tweet);
        }

        // clear the current line
        current_line.clear();
        // increment counter by one, next line
        counter++;
    }
    // use chrono library to get current time at just before the sorting algorithm called
    auto start = chrono::high_resolution_clock::now();
    // perform Quicksort, datavector: list to be sorted, 0: start, .size: end, & pivot selection
    Quicksort(dataVector, 0, dataVector.size() - 1, pivotSelection);
    // get current time just after the sorting ends
    auto stop = chrono::high_resolution_clock::now();
    // calculate the duration in milliseconds
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    // print the required sorting time message
    cout << "Sorted in " << duration.count() << " milliseconds by using " << pivotSelection << " pivot selection." << endl;
    // write the output csv file with datavecor list
    write2CSV(dataVector, size, outputFile);

    exit(EXIT_SUCCESS);
}
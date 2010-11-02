/*This program is an example of measuring time using clock() function.
  It also includes implementation of insertion sort and merge sort for ascending order;
  you have to implement these functions for descending order, and heap sort for both.
  You have to implement an efficient main() function so users will be able to select
  data set, sorting algotrithm, and sorting direction. 
 */

/* This program sorts an array's values into ascending order */

#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <cstring>

#define SIZE 100000 // array size

using namespace std;

void insertionsort(int *a, int size, string sortType);
void mergesort(int *a, int l, int r, string sortType);
void merge(int *a, int l, int m, int r, string sortType);

// your heap sort

class Heap {
private:
    int *array;
    int arraySize, heapSize;

    void maxHeapify(int);
    void buildMaxHeap();
    void minHeapify(int);
    void buildMinHeap();

    inline int parent(int node) {
        return (node - 1) / 2;
    }

    inline int left(int node) {
        return 2 * node + 1;
    }

    inline int right(int node) {
        return 2 * (node + 1);
    }

public:
    Heap(int*, int);
    void sort(string );

};

void readData(string fileName, int *a);
void writeData(string fileName, int *a);
/* function main begins program execution */
int actualSize = 0; // actual size of the array which is the number of lines read from the input file.

int main(int argc, char **argv) {

    // Define an array and read data
    int a[SIZE];
    string rchoice;
    string schoice;
    string inputs[7];
    string inputFile;

    //READ AND PROCESS THE OPTIONS from the command line
    // if all arguments entered ...
    if(argc == 7){
        for(int i=0; i<argc; i++){
            inputs[i] = argv[i];
        }

        for(int i=0; i<argc; i++){
            if(inputs[i] == "-s")
                schoice = inputs[++i];
            if(inputs[i] == "-r")
                rchoice = inputs[++i];
            if(inputs[i] == "-i")
                inputFile = inputs[++i];
        }
    }
    // else assign default values
    else{
        inputFile = "D1.txt";
        schoice = "h";
        rchoice = "a";
    }


    time_t start, end; /* time_t  defined in <ctime> and <sys/ctype> as long */
    clock_t sort_start_clk, sort_end_clk, read_start_clk, read_end_clk, write_start_clk, write_end_clk;
    float readTime, sortTime, writeTime;
    /* clock_t defined in <ctime>, it is better to use clock.*/
    read_start_clk = clock();

    readData("inputs/" + inputFile, a);
    read_end_clk = clock();

    Heap heap(a, actualSize-1);

    cout << "Sorting.."<<endl;
    sort_start_clk = clock();

    if(schoice == "h")
        heap.sort(rchoice);
    else if(schoice == "m")
        mergesort(a, 0, actualSize-2, rchoice);
    else if(schoice == "i")
        insertionsort(a, actualSize-1, rchoice);

    sort_end_clk = clock();
    cout << "...OK!" << endl;

    write_start_clk = clock();
    writeData("outputs/" + inputFile.insert(inputFile.size()-4, "_"+schoice+rchoice), a);
    write_end_clk = clock();

    readTime = ((float)(read_end_clk - read_start_clk) / CLOCKS_PER_SEC)*100;
    sortTime = ((float)(sort_end_clk - sort_start_clk) / CLOCKS_PER_SEC)*100;
    writeTime = ((float)(write_end_clk - write_start_clk) / CLOCKS_PER_SEC)*100;

    cout << "Using clock() function: " << sortTime << endl;

    // write to time.txt file
    fstream testfile;
    testfile.open("outputs/time.txt", fstream::out | fstream::app);
    for(int i = 0; i<argc; i++){
        testfile<<inputs[i]<< " ";
    }
    testfile << "\t" << readTime << "\t" << sortTime << "\t" << writeTime << endl;
    testfile.close();

    return 0;
}

void readData(string fileName, int *a) {
    //YOUR CODE HERE
    fstream file;
    file.open(fileName.c_str(), fstream::in);
    if (file.fail()) {
        cerr << "unable to open file!" << endl;
        exit(1);
    }
    while (!file.eof()) {
        file >> a[actualSize];
        actualSize++;
    }

    file.close();
}

void writeData(string fileName, int *a) {
    fstream file;
    file.open(fileName.c_str(), fstream::out);
    if (file.fail()) {
        cerr << "unable to open file!" << endl;
        exit(1);
    }
    for (int i = 0; i < actualSize; i++)
        file << a[i] << endl;
    file.close();
}


//YOU WILL NEED TO MODIFY THE INSERTION AND MERGE SORT BELOW SO THAT YOU CAN DO BOTH ASCENDING AND DESCENDING SORT.

void insertionsort(int *a, int n, string sortType) {
    int i, j, t;
    if (sortType == "a") {
        for (i = 1; i < n; i++) {
            j = i;
            t = *(a + j);
            while (j > 0 && *(a + j - 1) > t) {
                *(a + j) = *(a + j - 1);
                j--;
            }
            *(a + j) = t;
        }
    } else {
        for (i = 1; i < n; i++) {
            j = i;
            t = *(a + j);
            //cout<<"t:"<<t<<endl;
            while (j > 0 && *(a + j - 1) < t) {
                *(a + j) = *(a + j - 1);
                j--;
            }
            *(a + j) = t;
        }
    }
}

void merge(int *a, int l, int m, int r, string sortType) {
    int b[SIZE];
    int h, i, j, k;

    i = l;
    j = m + 1;
    k = l;
    if (sortType == "d") {
        while (i <= m && j <= r) {
            if (a[i] >= a[j]) {
                b[k] = a[i];
                i = i + 1;
            } else if (a[i] < a[j]) {
                b[k] = a[j];
                j = j + 1;
            }
            k = k + 1;
        }
    } else {
        while (i <= m && j <= r) {
            if (a[i] <= a[j]) {
                b[k] = a[i];
                i = i + 1;
            } else if (a[i] > a[j]) {
                b[k] = a[j];
                j = j + 1;
            }
            k = k + 1;
        }
    }

    if (i > m) {
        for (h = j; h <= r; h++) {
            b[k + h - j] = a[h];
        }
    } else if (j > r) {
        for (h = i; h <= m; h++) {
            b[k + h - i] = a[h];
        }
    }

    for (h = l; h <= r; h++) {
        a[h] = b[h];
    }

}

void mergesort(int *a, int l, int r, string sortType) {
    int m;

    // Recursion
    if (l < r) {
        m = (l + r) / 2;
        mergesort(a, l, m, sortType);
        mergesort(a, m + 1, r, sortType);
        merge(a, l, m, r, sortType);
    }
}

// Heap member functions

void Heap::maxHeapify(int i) {
    int l, r, largest, temp;
    l = left(i);
    r = right(i);
    if ((l <= heapSize) && (array[l] > array[i]))
        largest = l;
    else
        largest = i;

    if ((r <= heapSize) && (array[r] > array[largest]))
        largest = r;
    if (largest != i) {
        temp = array[i];
        array[i] = array[largest];
        array[largest] = temp;
        maxHeapify(largest);
    }
}

void Heap::buildMaxHeap() {
    for (int i = arraySize / 2; i >= 0; i--)
        maxHeapify(i);
}

void Heap::minHeapify(int i) {
    int l, r, smallest, temp;
    l = left(i);
    r = right(i);
    if ((l <= heapSize) && (array[l] < array[i]))
        smallest = l;
    else
        smallest = i;

    if ((r <= heapSize) && (array[r] < array[smallest]))
        smallest = r;
    if (smallest != i) {
        temp = array[i];
        array[i] = array[smallest];
        array[smallest] = temp;
        minHeapify(smallest);
    }

}

void Heap::buildMinHeap() {
    for (int i = arraySize / 2; i >= 0; i--)
        minHeapify(i);
}

Heap::Heap(int *in, int size) {
    array = in;
    arraySize = size - 1;
    heapSize = arraySize;
}

void Heap::sort(string type) {
    int temp;

    if (type == "a") {
        buildMaxHeap();

        for (int i = arraySize; i >= 1; i--) {
            temp = array[0];
            array[0] = array[i];
            array[i] = temp;
            heapSize--;
            maxHeapify(0);
        }
    } else {
        buildMinHeap();

        for (int i = arraySize; i >= 1; i--) {
            temp = array[0];
            array[0] = array[i];
            array[i] = temp;
            heapSize--;
            minHeapify(0);
        }
    }
}
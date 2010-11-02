/* 
 * File:   main.cpp
 * Author: cagatay
 *
 * Created on March 24, 2010, 11:21 PM
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

#define MAXNODES 1000

int main(int argc, char** argv) {
    int nodes[MAXNODES][MAXNODES];
    int numberOfNodes = 0;
    ifstream inputFile("input.txt");
    if(!inputFile.is_open()) exit(1);
    inputFile >> numberOfNodes;
    for(int rowCount = 0; rowCount < numberOfNodes; rowCount++){
        for(int colCount = 0; colCount < numberOfNodes; colCount++){
            inputFile >> nodes[rowCount][colCount];
        }
    }
    inputFile.close();
    for(int rowCount = 0; rowCount < numberOfNodes; rowCount++){
        for(int colCount = 0; colCount < numberOfNodes; colCount++){
            cout << nodes[rowCount][colCount] << " ";
        }
        cout << endl;
    }
    return (EXIT_SUCCESS);
}

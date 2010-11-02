#include "encoder.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv){
	
	/*if(argc < 2){
		cout << "you must specify the arguments" << endl;
		return 0;
	}*/
	Encoder encoder("input.txt");
	encoder.writeFrequencies("freq.txt");
	encoder.writeCodes("hcodes.txt");
	encoder.writeEncoded("encoded.txt");
	encoder.writeStats("stats.txt");

	return 0;
}
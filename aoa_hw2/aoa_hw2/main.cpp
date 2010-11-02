/*
 *	AoA HW2
 *	Ali Cagatay Yuksel (040050220)
 *	Main routine of the encoder program
 */

#include "encoder.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv){
	
	if(argc < 2){
		cout << "you must specify the arguments" << endl;
		return 0;
	}
	Encoder encoder(argv[1]);
	encoder.writeFrequencies((char*)"freq.txt");
	encoder.writeCodes((char*)"hcodes.txt");
	encoder.writeEncoded((char*)"encoded.txt");
	encoder.writeStats((char*)"stats.txt");

	return 0;
}

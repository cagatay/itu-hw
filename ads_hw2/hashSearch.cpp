#include <string>
#include <sstream>
#include <cmath>
#include <iostream>
#include <fstream>
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <dirent.h>
using namespace std;

const char alphabet[] = {'a', 'b', 'c', 'ç', 'd', 'e', 'f', 'g', 'ð', 'h',
						 'ý', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'ö', 'p',
						 'r', 's', 'þ', 't', 'u', 'ü', 'v', 'y', 'z'	};

class HashTable{
private:
	static const int ALPHABET = 29;
	static const int HTABLE_SIZE = 10000;
	int collisions;

	struct Bucket{
		string word;
		string filenames;
	} table[HTABLE_SIZE];
	
	// returns alphabetic index of a Turkis character
	inline int retIndex(char c){
		int i;
		for(i = 0; i < 29; i++)
			if(alphabet[i] == c)
				return i+1;
		return -1;
	}
	
	// function used to map a string to an integer. explanation is provided in the report.
	inline unsigned long long int str2int(string word){
		unsigned long long int key = 0;

		for(unsigned int i=0; i<word.length(); i++){
			key += (unsigned long long int)(retIndex(word[i]) * (unsigned long long int)pow(ALPHABET, (double)i));
		}

		return key;
	}
	
	// functions for double hashing
	inline int hash1(unsigned long long int key){
		return key%HTABLE_SIZE;
	}
	
	inline int hash2(unsigned long long int key){
		int i = key%7;

		if(i == 0) return 7;
		else return i;
	}

	inline int hash(unsigned long long int key, int i){
		return ( hash1(key) + i*hash2(key) ) % HTABLE_SIZE;
	}
	
	// reads searchable words from the given file and inserts them into the table
	void add(string filename){
		fstream file;
		file.open(filename.c_str(), fstream::in);
		string str;

		if (file.fail()) {
			cerr << "unable to open file!" << endl;
			exit(1);
		}
		while (!file.eof()) {
			file >> str;
			for(int i=0;; i++){		// find a free slot
				int k = hash(str2int(str), i);
				if(table[k].word == ""){
					table[k].word = str;
					break;
				}
				else collisions++;
			}
		}
		file.close();
		return;
	}
	
	// reads files in directories and fills the hashtable
	void readDir(string path){
		DIR *dir = opendir(path.c_str());
		struct dirent *ent;

		if (dir != NULL) {
			while ((ent = readdir (dir)) != NULL){
				if(strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".") == 0)
					continue;
				DIR *dir2 = opendir((path + ent->d_name).c_str());
				struct dirent *ent2;
				while((ent2 = readdir (dir2)) != NULL){
					if(strcmp(ent2->d_name, "..") == 0 || strcmp(ent2->d_name, ".") == 0)
						continue;
					fill(path + ent->d_name + "/" + ent2->d_name);
				}
				closedir(dir2);
			}
			closedir (dir);
		}
	}
	
	// looks words in a given file and adds the filename to the word's filenames list,
	// if the word is present in the hashtable
	void fill(string filename){
		fstream file;
		file.open(filename.c_str(), fstream::in);
		string str;

		if (file.fail()) {
			cerr << "unable to open file!" << endl;
			exit(1);
		}
		while (!file.eof()) {
			file >> str;

			int k = retSlot(str);

			if(k == -1) 
				continue;
			else{
				if(table[k].filenames.find(filename) == -1)
					table[k].filenames += filename + " ";
			}
		}
		file.close();
		return;
	}
	
	// returns the slot number of the key
	int retSlot(string s){
		int k;
		for(int i=0;; i++){
			k = hash(str2int(s), i);
			if(table[k].word == ""){
				k = -1;
				break;
			}
			if(table[k].word.find(s) == 0)
				break;
		}
		return k;
	}

public:
	// initializes table. takes two paramters, one for the filename of the list of searchable words and
	// one for the path to the dataset.
	HashTable(string words, string path){
		for(int i=0; i<HTABLE_SIZE; i++){
			table[i].word = "";
			table[i].filenames = "";
		}
		collisions = 0;
		add(words);
		readDir(path);
	}
	
	// returns a string which includes concatenation the filenames that given word occurs.
	string find(string s){
		int k = retSlot(s);

		if(k == -1)return "";
		else return table[k].filenames;
	}

	int getCols(){
		return collisions;
	}
};

int main(int argc, char **argv){
	setlocale(LC_ALL, "Turkish");
	
	string wfn, ifn, ofn; // wfn: dataset dir, ifn: search file, ofn: output file

	if(argc < 7){
		cout << "please use with arguments" << endl;
		exit(0);
	}
	else{
		for(int i=1; i<6; i++){
			if(strcmp(argv[i], "-i") == 0){
				ifn = argv[++i];
				continue;
			}
			if(strcmp(argv[i], "-f") == 0){
				wfn = argv[++i];
				continue;
			}
			if(strcmp(argv[i], "-o") == 0){
				ofn = argv[++i];
			}			
		}
	}

	string path = "inputs/words.txt";
	clock_t start, end;

	HashTable hashtable(path, wfn + "/");

	fstream infile, outfile;
	string query;
	
	infile.open(ifn.c_str(), fstream::in);
	if (outfile.fail()) {
        	cerr << "unable to open file!" << endl;
        	exit(1);
   	}
	
	outfile.open(ofn.c_str(), fstream::out);
	if(outfile.fail()) {
        	cerr << "unable to open file!" << endl;
        	exit(1);
    	}
	start = clock();
	while(!infile.eof()){
		infile >> query;
		string s = hashtable.find(query);
		
		// filenames are seperated with whitespaces, so i use stringstream to split them
		string buf;
		stringstream ss(s);
		while (ss >> buf)
			outfile << query << "\t\t" << buf << endl;
	}
	// finish
    	end = clock();
	
	infile.close();
	outfile.close();
	cout << "time it took: ";
	cout << (float)(end - start)*1000 / CLOCKS_PER_SEC << endl;
	cout << "number of collisions: " << hashtable.getCols() << endl;
	
	return 0;
}

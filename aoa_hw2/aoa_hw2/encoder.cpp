/*
 *	AoA HW2
 *	Ali Cagatay Yuksel (040050220)
 *	Definition of the encoder class
 */

#include "encoder.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

// Class constructor. Simply initializes variables and reads the file into fileContent

Encoder::Encoder(char *filename){
	fileContent = "";
	encoded = "";
	char c;
	map<char, int>::iterator it;
	
	ifstream inputFile(filename);
	
	if(inputFile.is_open()){
		while(!inputFile.eof()){
			inputFile.get(c);
			fileContent += c;
		}
		inputFile.close();
		
		fileContent.resize(fileContent.size() - 1); // workaround to avoid the last character being included twice
	}
	else cout << "input file couldn't be opened" << endl;	
}

void Encoder::writeFrequencies(char *filename){
	ofstream outFile(filename);
	map<char, int>::iterator it;
	
	if(outFile.is_open()){
		findFrequencies();

		for(it = frequencies.begin(); it != frequencies.end(); it++)
			outFile << (*it).first << " " << (*it).second << endl;
		
		outFile.close();
	}
	else{
		cout << "output file couldn't be opened" << endl;
		return;
	}
}

void Encoder::writeCodes(char *filename){
	ofstream outFile(filename);
	map<char, string>::iterator it;
	
	if(outFile.is_open()){
		huffman();

		for(it = codes.begin(); it != codes.end(); it++){
			if((*it).first == ' ')
				outFile << "space";
			else outFile << (*it).first;
			outFile << " " << (*it).second << endl;
		}
		
		outFile.close();
	}
	else{
		cout << "output file couldn't be opened" << endl;
		return;
	}
}

void Encoder::writeEncoded(char *filename){
	ofstream outFile(filename);

	if(outFile.is_open()){
		encode();
		outFile << encoded;
		outFile.close();
	}
	else{
		cout << "output file couldn't be opened" << endl;
		return;
	}
}

void Encoder::writeStats(char *filename){
	ofstream outFile(filename);

	if(outFile.is_open()){
		outFile << maxbits << endl;
		outFile << encoded.size() << endl;
		outFile << (float)encoded.size()/(fileContent.size()*8) << endl;

		outFile.close();
	}
	else{
		cout << "output file couldn't be opened" << endl;
		return;
	}
}

void Encoder::encode(){
	string::iterator it;

	for(it = fileContent.begin(); it < fileContent.end(); it++)
		encoded += codes[(*it)];
}

void Encoder::findFrequencies(){
	string::iterator it;
	for(it = fileContent.begin(); it < fileContent.end(); it++){
		if( frequencies.find((*it)) == frequencies.end() )
			frequencies[(*it)] = 1;
		else frequencies[(*it)]++;
	}
}

void Encoder::huffman(){
	map<char, int>::iterator it;

	vector<TreeNode*> vect;
	vector<TreeNode*>::iterator vit;
	TreeNode *tmp;

	for(it = frequencies.begin(); it != frequencies.end(); it++){
		tmp = new TreeNode;
		tmp->c = it->first;
		tmp->freq = it->second;
		tmp->isLeaf = true;
		vit = vect.begin();
		while(vit != vect.end()){
			if((*vit)->freq >= tmp->freq)
				break;
			else vit++;
		}
		vect.insert(vit, tmp);
	}

	rarestChar = vect[0]->c;

	while(vect.size() > 1){
		vit = vect.begin();
		tmp = new TreeNode;
		tmp->freq = vect[0]->freq + vect[1]->freq;
		tmp->isLeaf = false;
		tmp->left = vect[0];
		tmp->right = vect[1];
		vect.erase(vit, vit+2);

		vit = vect.begin();
		while(vit != vect.end()){
			if((*vit)->freq >= tmp->freq)
				break;
			else vit++;
		}
		vect.insert(vit, tmp);
	}

	tmp = vect[0];
	findCodes(tmp);

	maxbits = codes[rarestChar].size();
}

void Encoder::findCodes(TreeNode *node, string code){
	if(node->isLeaf)
		codes[node->c] = code;
	else{
		findCodes(node->left, (code + "0"));
		findCodes(node->right, (code + "1"));
	}
}

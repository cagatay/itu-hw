#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<sstream>

using namespace std;

int main(int argc, char *argv[]) {

	if(argc != 4){
		cout << "Usage : decode encoded.txt hcodes.txt decoded.txt" << endl;
		return -1;
	}

	fstream f;
	string line, word;
	map<string, char> code_dict;
	map<string, char>::iterator it;
	int i, j, l;
	ostringstream org_text;

	f.open(argv[2], ios::in);
	while(!f.eof()) {
		getline(f, line);
		if(line.length() == 0)
			break;
		if(line[1] == ' ')
			code_dict[line.substr(2)] = line[0];
//			code_dict.insert(make_pair(line.substr(2), line[0]));
		else
//			code_dict.insert(make_pair(line.substr(6), ' '));
			code_dict[line.substr(6)] = ' ';
	}
	f.close();

//	for(it = code_dict.begin(); it != code_dict.end(); ++it)
//		cout << it->first << ' ' << it->second << endl;


	f.open(argv[1], ios::in);
	getline(f, line);
	f.close();

	l = line.length();
	i = 0; j = 1;
	while(j <= l) {
		word = line.substr(i, j - i);
		if((it = code_dict.find(word)) != code_dict.end()) {
			org_text << it->second;
			i = j;
		}
		j++;
	}
	if(i != l)
		cout << "Error in decoding!!!";

	f.open(argv[3], ios::out);
	f << org_text.str();
	f.close();

}

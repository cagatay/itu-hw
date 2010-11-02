#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <clocale>
#include <dirent.h>
#include <ctime>
using namespace std;

struct WordNode{
	string word;
	struct WordNode *next;
};

class WordList{
private:
	struct WordNode *head;
	struct WordNode *last;
public:
	WordList();
	void add(string );
	void print();
	void find(string , string , fstream &);
};

struct ListNode{
	string filename;
	WordList *words;
	struct ListNode *next;
};

class LinkedList{
private:
	struct ListNode *head;
	struct ListNode *last;
public:
	LinkedList();
	void add(string );
	void print();
	void find(string , fstream &);
};

LinkedList::LinkedList(){
	head = NULL;
}

void LinkedList::add(string s){
	fstream file;
	string tmp;
	ListNode *current;

	if(head == NULL){
		head = new ListNode;
		head->filename = s;
		head->next = NULL;
		head->words = new WordList;
		last = head;
		current = last;
	}
	else{
		last->next = new ListNode;
		last->next->filename = s;
		last->words = new WordList;
		current = last;
		last = last->next;
		last->next = NULL;
	}

	file.open(s.c_str(), fstream::in);

	if (file.fail()) {
		cerr << "unable to open file!" << endl;
		exit(1);
	}
	while (!file.eof()) {
		file >> tmp;
		current->words->add(tmp);
	}

	file.close();
}

void LinkedList::print(){
	ListNode *tmp = head;

	while(true){
		if(tmp == NULL)
			break;
		cout << tmp->filename << endl;
		tmp->words->print();
		tmp = tmp->next;
	}
}

void LinkedList::find(string s, fstream &f){
	ListNode *tmp = head;
	while(true){
		if(tmp->next == NULL)
			break;
		tmp->words->find(s, tmp->filename, f);
		tmp = tmp->next;
	}
}



void WordList::add(string s){
	if(head == NULL){
		head = new WordNode;
		head->word = s;
		head->next = NULL;
		last = head;
	}
	else{
		last->next = new WordNode;
		last->next->word = s;
		last = last->next;
		last->next = NULL;
	}
}

void WordList::print(){
	WordNode *tmp = head;

	while(true){
		if(tmp == NULL)
			break;
		cout << tmp->word << " ";
		tmp = tmp->next;
	}
}

WordList::WordList(){
	head = NULL;
}

void WordList::find(string s, string fn, fstream &f){
	struct WordNode *tmp = head;

	while(true){
		if(tmp == NULL)
			break;
		if(tmp->word.find(s) != -1)
			f << tmp->word << "\t\t" << fn << endl;
		tmp = tmp->next;
	}
}


int main(int argc, char **argv){
	setlocale(LC_ALL, "Turkish");

	string wfn, ifn, ofn;

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

	LinkedList l;
	
	wfn += "/";

	DIR *dir = opendir(wfn.c_str());
	struct dirent *ent;

	if (dir != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if(strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".") == 0)
				continue;
			DIR *dir2 = opendir((wfn + ent->d_name).c_str());
			struct dirent *ent2;
			while((ent2 = readdir (dir2)) != NULL){
				if(strcmp(ent2->d_name, "..") == 0 || strcmp(ent2->d_name, ".") == 0)
					continue;
				l.add(wfn + ent->d_name + "/" + ent2->d_name);
			}
			closedir(dir2);
		}

		closedir (dir);
	} else {
		perror ("");
		return EXIT_FAILURE;
	}
	
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
	clock_t start, end;
	start = clock();
	while(!infile.eof()){
		infile >> query;
		l.find(query, outfile);
	}
	end = clock();

	infile.close();
	outfile.close();
	cout << "time it took: " << (float)(end - start)*1000 / CLOCKS_PER_SEC << endl;

	return 0;
}


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct Job{
	vector<int> choices;
	bool assigned;
};

int main(int argc, char **argv){

	if(argc < 3){
		cout << "please give the parameters" << endl;
		return 0;
	}

	// this part reads the file, and stores its content into memory
	int numberOfJobs;
	int numberOfProcessors;

	string inputFileName, outputFileName;

	inputFileName = argv[1];
	outputFileName = argv[2];

	ifstream inputFile(inputFileName);
	
	inputFile >> numberOfJobs;
	inputFile >> numberOfProcessors;

	int *processors = new int[numberOfProcessors];
	bool **jobTable = new bool* [numberOfJobs];
	int *assignments = new int[numberOfJobs];

	for(int i=0; i<numberOfJobs; i++){
		jobTable[i] = new bool[numberOfProcessors];
		for(int j=0; j<numberOfProcessors; j++)
			inputFile >> jobTable[i][j];
	}

	for(int i=0; i<numberOfProcessors; i++)
		inputFile >> processors[i];

	inputFile.close();


	// end of file reading section

	// algorithm starts here
	/*
	vector<Job> jobQueue;
	vector<Job>::iterator it;

	for(int i=0; i<numberOfJobs; i++){
		Job tmp;
		for(int j=0; j<numberOfProcessors; j++){
			if(jobTable[i][j]) tmp.choices.push_back(j);
		}
		tmp.number = i;
		
		it = jobQueue.begin();
		while(true){
			if(it == jobQueue.end() || (*it).choices.size() >= tmp.choices.size()) break;
			else it++;
		}

		jobQueue.insert(it, tmp);
	}

	while(true){
		if(jobQueue.size() == 0) break;
		int most = jobQueue[0].choices[0];
		for(unsigned int i=0; i<jobQueue[0].choices.size(); i++){
			if(processors[jobQueue[0].choices[i]] >= processors[most])
				most = jobQueue[0].choices[i];
		}
		if(processors[most] == 0){
			cout << "impossible" << endl;
			break;
		}
		assignments[jobQueue[0].number] = most;
		processors[most]--;
		jobQueue.erase(jobQueue.begin());
	}
	*/

	// make a job list
	Job *jobs = new Job[numberOfJobs];

	for(int i=0; i<numberOfJobs; i++){
		//jobs[i].number = i;
		jobs[i].assigned = false;
		for(int j=0; j<numberOfProcessors; j++){
			if(jobTable[i][j])
				jobs[i].choices.push_back(j);
		}
	}

	int assigned = 0;

	while(true){
		if(assigned == numberOfJobs)
			break;

		int least = numberOfProcessors + 1;
		int chosen = -1;
		int most;
		vector<int>::iterator it;
		
		for(int i=0; i<numberOfJobs; i++){
			if(!jobs[i].assigned && jobs[i].choices.size() < least){
				chosen = i;
				least = jobs[i].choices.size();
			}
		}

		most = jobs[chosen].choices[0];
		for(int i=1; i<least; i++){
			if(processors[jobs[chosen].choices[i]] > processors[most])
				most = i;
		}

		if(processors[most] == 0){
			cout << "Scheduling is impossible" << endl;
			return 0;
		}

		assignments[chosen] = most;
		jobs[chosen].assigned = true;
		processors[most]--;
		assigned++;

		if(processors[most] == 0){
			for(int i=0; i<numberOfJobs; i++){
				if(jobTable[i][most]){
					for(it=jobs[i].choices.begin(); it<jobs[i].choices.end(); it++){
						if((*it) == most){
							jobs[i].choices.erase(it);
							break;
						}
					}
				}
			}
		}
	}
					
	ofstream outputFile(outputFileName);
	for(int i=0; i<numberOfJobs; i++)
		outputFile << assignments[i] + 1 << endl;

	return 0;
}
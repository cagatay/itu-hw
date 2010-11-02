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

	string inputFileName(argv[1]);
	string outputFileName(argv[2]);

	ifstream inputFile(inputFileName.c_str());
	
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
		jobs[i].assigned = false;
		for(int j=0; j<numberOfProcessors; j++){
			if(jobTable[i][j])
				jobs[i].choices.push_back(j);
		}
	}
	
	

	int assigned = 0;
	int least, chosen, most;
	
	// iterate until all jobs are scheduled or it is clear that there is no solution
	while(true){
		if(assigned == numberOfJobs)	// all jobs are scheduled
			break;

		least = numberOfProcessors + 1;	// least will hold the job with the least processor option.
		chosen = -1;			// which job has the least option
		vector<int>::iterator it;
		
		// after this loop, we will have the job which has the fewest option in variable chosen
		for(int i=0; i<numberOfJobs; i++){
			if(!jobs[i].assigned && jobs[i].choices.size() < least){
				chosen = i;
				least = jobs[i].choices.size();
			}
		}
		
		// if all the processors that this job can run on reached their limits,
		// this means there is no solution
		if(jobs[chosen].choices.size() == 0){
			cout << "Scheduling is impossible" << endl;
			return 0;
		}
		
		most = jobs[chosen].choices[0];	// most will hold the index of the processor which has the highest limit
		for(int i=1; i<least; i++){
			if(processors[jobs[chosen].choices[i]] > processors[most])
				most = i;
		}

		// this section makes the assignment
		assignments[chosen] = most;
		jobs[chosen].assigned = true;
		processors[most]--;
		assigned++;
		
		// if the processor just assigned reached its limit, then remove it from the options of the other jobs
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
					
	ofstream outputFile(outputFileName.c_str());
	for(int i=0; i<numberOfJobs; i++)
		outputFile << assignments[i] + 1 << endl;
	
	outputFile.close();

	return 0;
}
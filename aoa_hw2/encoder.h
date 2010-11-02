#include <string>
#include <map>

using namespace std;

struct TreeNode{
	char c;
	int freq;
	bool isLeaf;
	TreeNode *left;
	TreeNode *right;
};

class Encoder{
	string fileContent;
	string encoded;
	map<char, int> frequencies;
	map<char, string> codes;

	char rarestChar;
	int maxbits;
	
	void findFrequencies();
	void findCodes(TreeNode *node, string code = "");
	void huffman();
	void encode();
public:
	Encoder(char *);
	void writeFrequencies(char *);
	void writeCodes(char *);
	void writeEncoded(char *);
	void writeStats(char *);
};
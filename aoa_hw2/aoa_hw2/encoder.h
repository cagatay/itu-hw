/*
 *	AoA HW2
 *	Ali Cagatay Yuksel (040050220)
 *	Declaration of the encoder class
 */

#include <string>
#include <map>

using namespace std;

// TreeNode structure is used to set up the tree to find the huffman codes

struct TreeNode{
	char c;			// corresponding character
	int freq;		// frequency of character
	bool isLeaf;		// is this node a leaf node? which means it holds a character itself
	TreeNode *left;		// left child
	TreeNode *right;	// right child
};

// Encoder class represents all the operations needed to encode the string

class Encoder{
	string fileContent;		// content of the file to be encoded is read into this
	string encoded;			// encoded string is written here
	map<char, int> frequencies;	// maps the character to its frequency
	map<char, string> codes;	// maps the character to its huffman code

	char rarestChar;		// needed for stats.txt file
	int maxbits;			// needed for stats.txt file
	
	void findFrequencies();		// fills 'frequencies' map reading fileContent
	void findCodes(TreeNode *node, string code = "");	// finds huffman codes, works recursively
	void huffman();			// runs the algorithm (build tree, etc)
	void encode();			// encodes the string using the map
public:
	Encoder(char *);		// takes the input file name as argument and constructs the encoder
	void writeFrequencies(char *);	// writes frequencies into output file
	void writeCodes(char *);	// writes huffman codes into output file	
	void writeEncoded(char *);	// writes encoded string into output file
	void writeStats(char *);	// writes stats into output file
};

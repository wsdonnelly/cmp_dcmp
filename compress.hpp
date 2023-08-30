#pragma once

#include <map>
#include <string>
#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <utility>
#include <cmath>
#include <cstdint>

struct Node
{
    char ch;
    int freq;
    Node* left = nullptr;
    Node* right = nullptr;
	~Node(){delete left; delete right;}
};

struct Compare
{
    bool operator()(const Node* lhs, const Node* rhs)
    {
        return lhs->freq > rhs->freq;
    }
};

class Compress
{
public:
	Compress(std::string filePath);
	void printCodes();
	std::map<char, std::vector<bool>>& getCodeMap () {return codeMap;}
	Node* getRoot(){ return root; }

private:
	std::priority_queue<Node*, std::vector<Node*>, Compare > pQueue;
	Node* root;
	std::string filePath;
	std::vector<bool> code;
	std::map<char, std::vector<bool>> codeMap;
	std::map<char, int> frequencyTable;
	int maxFreqSize;

	void getFrequencyTable();
	void getMaxFreqSize();
	void createTree();
	void generateCodes(Node* root, std::vector<bool>& code, std::map<char, std::vector<bool>>& codeMap);
	void toFile();

};
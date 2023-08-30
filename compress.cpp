#include "compress.hpp"

Compress::Compress(std::string filePath) : filePath(filePath)
{
	getFrequencyTable();
	createTree();
	generateCodes(root, code, codeMap);
	toFile();
	delete root;
}

void Compress::getFrequencyTable()
{
	char ch;
	std::fstream file(filePath, std::fstream::in);
	while (file >> std::noskipws  >> ch)
	{
		frequencyTable[ch] += 1;
	}
	getMaxFreqSize();
}

void Compress::getMaxFreqSize()
{
	maxFreqSize = 0;

	for (auto it : frequencyTable)
	{
		if (it.second > maxFreqSize)
			maxFreqSize = it.second;
	}
	//get num of bits
	maxFreqSize = (int)std::log2(maxFreqSize) + 1;
	std::cout << "\nmaxFreqSize in bits: " << maxFreqSize << "\n";
}

void Compress::createTree()
{
	for (const auto& n : frequencyTable)
	{
		Node* temp = new Node;
		temp->ch = n.first;
		temp->freq = n.second;
		pQueue.push(temp);
	}

	while (pQueue.size() > 1)
	{
		Node* newNode = new Node;
	
		newNode->ch = 0;
		newNode->left = pQueue.top();
		pQueue.pop();
		newNode->right = pQueue.top();
		pQueue.pop();
		newNode->freq = newNode->left->freq + newNode->right->freq;
		pQueue.push(newNode);
	}
	root =  pQueue.top();
	pQueue.pop();
}

void Compress::generateCodes(Node* root, std::vector<bool>& code, std::map<char, std::vector<bool>>& codeMap)
{
	if (root->left)
	{
		std::vector<bool> leftCode = code;
		leftCode.push_back(false);
		generateCodes(root->left, leftCode, codeMap);
	}
	if (root->right)
	{
		std::vector<bool> rightCode = code;
		rightCode.push_back(true);
		generateCodes(root->right, rightCode, codeMap);
	}
	if (root->ch != 0)
	{
		codeMap[root->ch] = code;
	}
}

void writeByte(uint8_t num, std::vector<bool>& codeString)
{
	for (int shift{0}; shift < 8; shift++)
	{
		(num & (1 << shift)) ?
			codeString.push_back(true) :
			codeString.push_back(false);
	}
}

void Compress::toFile()
{
	char ch;
	std::fstream file(filePath, std::fstream::in);
	std::ofstream fs("compressed.bin", std::ios::out | std::ios::binary);
	std::vector<bool> codeString;

	//write header to codestring
	//|8bits FreqSize in bits | 8bits FrequencyTable len | 8bits ch : frqsize bits code .....

	writeByte(static_cast<uint8_t> (maxFreqSize), codeString);
	writeByte(static_cast<uint8_t>(frequencyTable.size()), codeString);
	for (auto it : frequencyTable)
	{
		writeByte(static_cast<uint8_t>(it.first), codeString);
		for (int shift{0}; shift < maxFreqSize; shift++)
		{
			(it.second & (1 << shift)) ?
				codeString.push_back(true) :
				codeString.push_back(false);
		}
	}
	std::cout << "codestring HEADER len: " << codeString.size() << "\n";
	std::cout << "frequency Table len: " << frequencyTable.size() << "\n";
	//write codes to codestring
	while (file >> std::noskipws >> ch)
	{
		codeString.insert(codeString.end(), codeMap[ch].begin(), codeMap[ch].end());
	}
	//write codestring to file byte by byte
	auto it = codeString.begin();
	while (it != codeString.end())
	{
		uint8_t buf{0};
		int shift{0};
		while (shift < 8 && it != codeString.end())
		{
			if (*it)
				buf |= (1 << shift);
			shift++;
			it++;
		}
		fs.write(reinterpret_cast<const char *>(&buf), 1);
	}
	std::cout << "codestring size: " << sizeof(codeString) << "\n";
	std::cout << "codestring len: " << codeString.size() << "\n";
}

void Compress::printCodes()
{
	std::cout << "\nprintCodes\n";
	for (const auto& it : codeMap)
	{
		std::cout << "[" << it.first << "] : ";
		for (auto c : it.second)
		{
			if (c)
				std::cout << "1";
			else
				std::cout << "0";
		}
		std::cout << std::endl;
	}
}

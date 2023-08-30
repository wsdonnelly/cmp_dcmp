#include <iostream>
#include <map>
#include <fstream>

#include "compress.hpp"

class Decompress
{
public:
	Decompress(std::string compressedFilePath, Node* root, int totalBytes) : totalBytes(totalBytes)
	{
		std::ifstream input(compressedFilePath, std::ios::binary);
		std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(input), {});
		printedBytes = 0;
		Node* temp = root;
		auto byte = buffer.begin();
		while (byte != buffer.end())
		{
			int shift{0};
			while (shift < 8)
			{
				if (*byte & (1 << shift))
				{
					temp = temp->right;
				}
				else
				{
					temp = temp->left;
				}
				if (temp->ch != 0)
				{
					std::cout <<temp->ch;
					if (++printedBytes == totalBytes)
						break;
					temp = root;
				}
				shift++;
			}
			byte++;
		}
	}
private:
	int totalBytes;
	int printedBytes;
};

int main()
{
	Compress compressed("../samples/theCLoud.txt");
	compressed.printCodes();
	// Decompress decmp("compressed.bin", compressed.getRoot(), freq.getTotalSize());

	return 0;
}

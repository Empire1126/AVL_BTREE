

#include "pch.h"
#include <iostream>
#include "AVL.h"


int main()
{
	std::string file = "example.txt";
	std::fstream stream;
	stream.open(file, std::fstream::out);
	AVLNode newNode;
	std::string data = "Hopefully this shows up";
	strcpy_s(newNode.Payload, data.c_str());
	newNode.count = 55;
	newNode.lChild = 3;
	newNode.rChild = 5;
	stream.seekp(sizeof(AVLNode) * 2);
	stream.write(reinterpret_cast<const char*>(&newNode), sizeof(AVLNode));
	stream.close();
	stream.open(file, std::fstream::in);
	AVLNode read;
	stream.seekg(sizeof(AVLNode) * 2);
	stream.read(reinterpret_cast<char *>(&read), sizeof(AVLNode));
	if(strcmp(read.Payload,data.c_str())==0)
	{
		std::cout << "read successful";
	}
	std::cout << "read in file";
	std::string testString;
	std::cin >> testString;
}





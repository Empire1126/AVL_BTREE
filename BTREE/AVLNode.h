#pragma once
#include <string>
#include <iostream>
#include <fstream>
class AVLNode
{
public:
	AVLNode();
	~AVLNode();
	char Payload[50]={'\0'};
	int count = 0;
	unsigned int id = 0;
	int balanceFactor = 0;
	int lChild = 0;
	int rChild = 0;
};


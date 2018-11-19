#pragma once
#include <string>
#include <iostream>
class AVLNode
{
public:
	AVLNode();
	~AVLNode();
	char Payload[32];
	int count = 0;
	int balanceFactor = 0;
	int lChild = 0;
	int rChild = 0;
};


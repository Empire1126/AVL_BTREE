#pragma once
#include "AVLNode.h"

class AVL
{
public:
	AVL();
	~AVL();
	void insert(std::string toAdd);
	void search(std::string toFind);
	void list();
private:
	int root = 0;
	void insertImpl(std::string toAdd);
	AVLNode searchImpl(std::string toFind);
	void listImpl();
	AVLNode getNode(int nodeID);
	void saveNode(AVLNode toSave, int nodeID);
};


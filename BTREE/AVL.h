#pragma once
#include "AVLNode.h"
class AVL
{
public:
	AVL();
	AVL(std::string treeFileLocation);
	~AVL();
	void insert(std::string toAdd);
	void search(std::string toFind);
	void list();
private:
	std::string treeLocation;
	unsigned int nextNodeId = 0;
	unsigned int root = 0;
	void insertImpl(std::string toAdd);
	AVLNode searchImpl(std::string toFind);
	void listImpl();
	AVLNode readNode(int nodeID);
	void saveNode(AVLNode toSave, int nodeID);
};


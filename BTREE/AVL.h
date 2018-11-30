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
	double readCount = 0;
	double writeCount = 0;
	int height = 0;
	double totalNodes = 0;
	void reportHeight();
private:
	std::string treeLocation;
	unsigned int nextNodeId = 0;
	unsigned int root = 0;
	void insertImpl(std::string toAdd);
	AVLNode searchImpl(std::string toFind);
	void listImpl(AVLNode currentNode);
	AVLNode readNode(int nodeID);
	void saveNode(AVLNode toSave, int nodeID);
	void reportHeightImpl();
};


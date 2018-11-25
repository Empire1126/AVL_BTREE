#pragma once
#include <string>
#include <iostream>
#include <fstream>
const int minimumDegree = 5;
class BTREE
{
public:
	BTREE();
	BTREE(std::string treeFileLocation);
	~BTREE();
	struct BTreeKey
	{
		char data[50];
		int count = 0;
	};
	struct BTreeNode
	{
		BTreeKey keys[(2 * minimumDegree) - 1];
		int children[2 * (minimumDegree)];
		int numberOfKeys = 0;
		bool isLeaf = 1;
		unsigned int id = 0;
	};
	
	void insert(std::string toAdd);
	void search(std::string toAdd);
private:
	int root = 0;
	int nextNodeId = 0;
	std::string treeLocation;
	BTreeNode readNode(int nodeId);
	void saveNode(BTreeNode toSave,int nodeId);
	void SplitChild(BTreeNode fullNodeParent, int middleIndex);
	void insertImpl(std::string toAdd);
	void insertNonFull(BTreeNode nonFullNode, std::string toAdd);
	BTreeNode searchImpl(std::string toFind);
};



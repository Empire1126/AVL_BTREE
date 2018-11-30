#pragma once
#include <string>
#include <iostream>
#include <fstream>
const int minimumDegree = 2;
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
		int children[2 * (minimumDegree)] = {0};
		int numberOfKeys = 0;
		bool isLeaf = 1;
		unsigned int id = 0;
	};
	struct BTreeSearchResult
	{
		BTreeNode node;
		int index;
	};
	
	void insert(std::string toAdd);
	void search(std::string toFind);
	void list();
	double readCount = 0;
	double writeCount = 0;
	int height = 0;
	double totalNodes = 0;
	void reportHeight();
private:
	int root = 0;
	int nextNodeId = 0;
	std::string treeLocation;
	BTreeNode readNode(int nodeId);
	void saveNode(BTreeNode toSave,int nodeId);
	void SplitChild(BTreeNode fullNodeParent, int middleIndex);
	void insertImpl(std::string toAdd);
	void insertNonFull(BTreeNode nonFullNode, std::string toAdd);
	BTreeSearchResult searchImpl(BTreeNode selector, std::string toFind);
	void listImpl(BTreeNode currentNode);
	void reportHeightImpl();
};



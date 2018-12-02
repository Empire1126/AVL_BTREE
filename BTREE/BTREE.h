/*Btree.h
 *Luke Pinkney
 *EECS 2510 Fall 2018
 *11/28/2018
 *Btree header file, contains decelerations of class functions and class variables, also contains structs of the btree Nodes
 */
#pragma once
#include <string>
#include <iostream>
#include <fstream>
const int minimumDegree = 5; // the minimum degree of the btree, it determines the number of keys and children a node can have
class BTREE
{
	//public function deceleration and variables for the btree
public:
	BTREE();
	BTREE(std::string treeFileLocation);//constructor with string to be tree file's name instead of the default file name
	~BTREE();
	struct BTreeKey //key that represents a word and its count for the keys array in teh btree node
	{
		char data[50];
		int count = 0;
	};
	struct BTreeNode//a btree node
	{
		BTreeKey keys[(2 * minimumDegree) - 1];//the btree can only have an odd number of keys thus (2 * minimumdegree) -1 
		int children[2 * (minimumDegree)] = {0}; //the btree will have even keys only thus 2 * minimum degree
		int numberOfKeys = 0;//number of keys present in a node
		bool isLeaf = 1;//boolean to determine if the node is a leaf
		unsigned int id = 0; //unique id of a node to save them correctly
	};
	struct BTreeSearchResult//btree search result, it is required to return the node and the index in the keys array that the searched for word
	//was found thus I packaged them into a single struct
	{
		BTreeNode node;
		int index;
	};
	
	void insert(std::string toAdd);//public insert function that will call the private implementation 
	void search(std::string toFind);//public search function that will call the private implementation
	void list();//wrapper for in order print out of tree
	double readCount = 0;
	double writeCount = 0;
	int height = 0;//height of the tree
	double totalNodes = 0;
	double totalWords = 0;
	double totalUsedKeys = 0;
	void reportHeight();//reports the height of the tree
	void reportTotalNumberOfNodesAndWords();//reports the total words and nodes of the tree
	void reportLoadingFactor();//reports the loading factor of the btree
	void tellBtreeFileSize();
private:
	int root = 0;
	int nextNodeId = 0;
	std::string treeLocation;
	BTreeNode readNode(int nodeId);//reads in a node based on its id
	void saveNode(BTreeNode toSave,int nodeId);//save a node to disk 
	void SplitChild(BTreeNode fullNodeParent, int middleIndex);//splits a node that is full of keys
	void insertImpl(std::string toAdd);//private insert function 
	void insertNonFull(BTreeNode nonFullNode, std::string toAdd);//inserting a value into a non full btree node
	BTreeSearchResult searchImpl(BTreeNode selector, std::string toFind);//search implementation of a btree
	void listImpl(BTreeNode currentNode);//in order print out of a btree, recursive
	void reportHeightImpl(BTreeNode currentNode, int currentHeight);//recursively calculates the height of the btree
	void reportTotalNumberOfNodesAndWordsImpl(BTreeNode currentNode);//recursively counts the nodes and total words in the tree
	void reportLoadingFactorImpl(BTreeNode currentNode);//calculates the loading factor of the tree
};



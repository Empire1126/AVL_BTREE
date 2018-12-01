/*AVL.h
 *Luke Pinkney
 *EECS 2510 Fall 2018
 *11/28/2018
 *Header file for avl tree source code, decelerations of class wide variables and constants, defines functions of avl tree
 */

#pragma once
#include "AVLNode.h"
class AVL
{
	//AVL tree public function decelerations and public variables
public:
	AVL();
	AVL(std::string treeFileLocation); // constructor with tree file name as parameter, use in case default tree's filename is undesirable
	~AVL();
	void insert(std::string toAdd); // public wrapper to avl insert function
	void search(std::string toFind); // public search function
	void list(); //public in order traversal to list tree
	double readCount = 0; // number of reads to the disk
	double writeCount = 0; //number of writes to the disk
	int height = 0; // height of the tree
	double totalNodes = 0; //total unique words in the tree
	double totalWords = 0; //total unique words + duplicate words in the tree
	void reportHeight(); //public wrapper to calculate height
	void reportTotalNumberOfNodesAndWords(); //public wrapper to calculate total unique words and duplicates
private:
	std::string treeLocation; //this is the name of the file that contains the avl tree data
	unsigned int nextNodeId = 0; //this is a tracker to keep each node added to the tree unique, it identifies each node
	unsigned int root = 0; //id of the root node
	void insertImpl(std::string toAdd); //actual insert implementation 
	AVLNode searchImpl(std::string toFind); // search implementation function
	void listImpl(AVLNode currentNode); //in order recursive  implementation  
	AVLNode readNode(int nodeID); //function to read a particular node from the disk
	void saveNode(AVLNode toSave, int nodeID); //function to write a node to the disk
	void reportHeightImpl(AVLNode currentNode,int currentHeight);//implementation to report height of the avl tree 
	void reportTotalNumberOfNodesAndWordsImpl(AVLNode currentNode);//implementation to report the total words and total unique words of the avl tree
};


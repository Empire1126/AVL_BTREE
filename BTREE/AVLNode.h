/*AVLNode.h
 *Luke Pinkney
 *EECS 2510 Fall 2018
 *11/28/2018
 *AVL tree node header file, contains decelerations of functions and more importantly decelerations of class variables
 */
#pragma once
#include <string>
#include <iostream>
#include <fstream>
class AVLNode
{
	//this class holds the deceleration of the nodes of the avl tree
public:
	AVLNode();
	~AVLNode();
	char Payload[50]={'\0'};//50 length c string in order to hold the longest words possible in our sample texts
	int count = 0;//count for the number of duplicates of a word
	unsigned int id = 0;//the id of the current node, must be unique to this node in order to save to the disk correctly
	int balanceFactor = 0;//the balance factor unique to the avl tree to prevent BST degeneration
	int lChild = 0;//the left and right child of the node
	int rChild = 0;
};


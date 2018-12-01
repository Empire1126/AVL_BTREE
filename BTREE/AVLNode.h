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


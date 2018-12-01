/*AVL.cpp
 *Luke Pinkney
 *EECS 2510 Fall 2018
 *11/28/2018
 *avl tree source code file, contains definitions of functions
 */


#include "pch.h"
#include "AVL.h"



AVL::AVL()
{
	//default constructor of the avl tree
	treeLocation = "AVL_TREE_DATA.AVLTree"; //default avl tree file name
	std::fstream stream; 
	stream.open(treeLocation, std::fstream::out | std::fstream::binary);
	AVLNode nil; // this is the node of id =0 and functions as null for the tree
	stream.write(reinterpret_cast<const char*>(&nil), sizeof(AVLNode)); //write the "null" node to the tree,
	stream.close();
}

AVL::AVL(std::string treeFileLocation)
{
	//parameter : treeFileLocation - user defined tree file name instead of default 
	//same as default constructor except that the user has now specified the name of the tree's file
	std::fstream stream;
	stream.open(treeFileLocation,std::fstream::out | std::fstream::binary); //use the parameter as the tree's file name instead of the default
	AVLNode nil;
	stream.write(reinterpret_cast<const char*>(&nil), sizeof(AVLNode));
	stream.close();
	treeLocation = treeFileLocation;		
}


AVL::~AVL()
{
}

void AVL::search(std::string toFind)
{
	//parameter : toFind - the string to find in the avl tree
	//search function of avl tree
	AVLNode result = searchImpl(toFind); //call the search implementation to find the string in the tree 
	if(result.id==0)//if the result was "null" aka the node of id=0
	{
		std::cout << "" << std::endl; //the word is not in the tree
	}
	else
	{
		std::cout << result.Payload << " " << result.count << std::endl; //otherwise print out the word and the number of times it is in the tree
	}
}

AVLNode AVL::searchImpl(std::string toFind)
{
	//parameter : toFind - the string to compare against when searching
	//avl search implementation
	AVLNode currentNode = readNode(root);//start searching the tree at the root, get the root node into memory
	while (currentNode.id!=0)//as long as we haven't fallen off the tree into the "null" node
	{
		if(strcmp(toFind.c_str(),currentNode.Payload)==0)//is the node contents equal to the string "toFind"?
		{
			return currentNode;//if so we found the word we are looking for, return the current node we are at
		}
		else  if(strcmp(toFind.c_str(),currentNode.Payload)<0)//otherwise is "toFind" less than or greater than the current node
		{
			currentNode = readNode(currentNode.lChild);//if less then traverse left
		}
		else
		{
			currentNode = readNode(currentNode.rChild);//otherwise traverse right
		}
	}
	AVLNode nil;//if we fall off the the tree return a node with id =0, this is essentially equal to returning the "null" node
	return  nil;
}


void AVL::insert(std::string toAdd)
{
	//parameters: toAdd - the node to attempt to add to the tree 
	//wrapper to call private insertImpl
	insertImpl(toAdd);
}

void AVL::insertImpl(std::string toAdd)
{
	//parameters: toAdd - the node to compare against when attempting to add
	if (root == 0)//if the tree is empty
	{
		AVLNode newNode;//create a new root
		//add the data of the word inserted to the root and set child pointer and counts
		strcpy_s(newNode.Payload, toAdd.c_str());
		newNode.lChild = newNode.rChild = 0;
		newNode.count=1;
		newNode.balanceFactor = 0;
		newNode.id = ++nextNodeId; //increment the node ID tracker variable before setting this new nodes id to it.
		root = newNode.id; //set the root to the new roots id
		saveNode(newNode, newNode.id); //save the new node
		return;
	}
	int displacement = 0; //displacement to keep track of how a new node will imbalance an existing tree 
	unsigned int LastImbalanceId = root; //this variable will keep track of the last place an acceptable imbalance was seen
	unsigned int LastImbalanceParentId = 0; //this variable will keep track of the parent of the last acceptable imbalance seen
	unsigned int currentParentId = 0; // keeps track of the lagger node, or parent of the current node we are at while searching
	unsigned int lastImbalanceChildId = 0; //id of the child node to the last acceptable imbalance
	AVLNode currentNode = readNode(root);//start searching at the root of the tree
	while (currentNode.id != 0)//while we haven't fallen off the tree into the "null" node.
	{
		if (strcmp(currentNode.Payload, toAdd.c_str()) == 0)
		{
			currentNode.count++;
			saveNode(currentNode, currentNode.id);
			return;
		}
		if (currentNode.balanceFactor != 0)
		{
			LastImbalanceId = currentNode.id;
			LastImbalanceParentId = currentParentId;
		}
		if (strcmp(toAdd.c_str(), currentNode.Payload) < 0)
		{
			currentParentId = currentNode.id;
			currentNode = readNode(currentNode.lChild);
		}
		else
		{
			currentParentId = currentNode.id;
			currentNode = readNode(currentNode.rChild);
		}
	}
	strcpy_s(currentNode.Payload, toAdd.c_str());
	currentNode.rChild = 0;
	currentNode.lChild = 0;
	currentNode.balanceFactor = 0;
	currentNode.id = ++nextNodeId;
	currentNode.count = 1;
	saveNode(currentNode, currentNode.id);
	unsigned int newNodeId = currentNode.id;
	AVLNode currentParent = readNode(currentParentId);
	if (strcmp(toAdd.c_str(), currentParent.Payload) < 0)
	{
		currentParent.lChild = currentNode.id;
		
	}
	else
	{
		currentParent.rChild = currentNode.id;
		
	}
	saveNode(currentParent, currentParent.id);
	currentParent = readNode(LastImbalanceId);
	if (strcmp(toAdd.c_str(), currentParent.Payload) > 0)
	{
		lastImbalanceChildId = currentParent.rChild;
		displacement = -1;
	}
	else
	{
		lastImbalanceChildId = currentParent.lChild;
		displacement = 1;
	}
	currentNode = readNode(lastImbalanceChildId);
	while (currentNode.id != newNodeId)
	{
		if (strcmp(toAdd.c_str(), currentNode.Payload) > 0)
		{
			currentNode.balanceFactor = -1;
			saveNode(currentNode, currentNode.id);
			currentNode = readNode(currentNode.rChild);
		}
		else
		{
			currentNode.balanceFactor = 1;
			saveNode(currentNode, currentNode.id);
			currentNode = readNode(currentNode.lChild);
		}
	}
	currentNode = readNode(LastImbalanceId);
	if (currentNode.balanceFactor == 0)
	{
		currentNode.balanceFactor = displacement;
		saveNode(currentNode, currentNode.id);
		return;
	}
	if (currentNode.balanceFactor == -displacement)
	{
		currentNode.balanceFactor = 0;
		saveNode(currentNode, currentNode.id);
		return;
	}
	currentNode = readNode(lastImbalanceChildId);
	AVLNode c;
	if (displacement == 1)
	{
		if (currentNode.balanceFactor == 1)
		{
			currentParent = readNode(LastImbalanceId);
			currentParent.lChild = currentNode.rChild;
			currentNode.rChild = currentParent.id;
			currentNode.balanceFactor = currentParent.balanceFactor = 0;
			saveNode(currentNode, currentNode.id);
			saveNode(currentParent, currentParent.id);
		}
		else
		{
			currentParent = readNode(LastImbalanceId);
			c = readNode(currentNode.rChild);
			currentNode.rChild = c.lChild;
			currentParent.lChild = c.rChild;
			c.lChild = currentNode.id;
			c.rChild = currentParent.id;			
			saveNode(currentNode, currentNode.id);
			saveNode(currentParent, currentParent.id);
			saveNode(c, c.id);
			switch (c.balanceFactor)
			{
			case  0:
				currentNode.balanceFactor = 0;
				currentParent.balanceFactor = 0;
				saveNode(currentNode, currentNode.id);
				saveNode(currentParent, currentParent.id);
				break;
			case 1:
				currentNode.balanceFactor = -1;
				currentParent.balanceFactor = 0;
				saveNode(currentNode, currentNode.id);
				saveNode(currentParent, currentParent.id);
				break;
			case -1:
				currentNode.balanceFactor = 0;
				currentParent.balanceFactor = 1;
				saveNode(currentNode, currentNode.id);
				saveNode(currentParent, currentParent.id);
				break;
			}
			c.balanceFactor = 0;
			saveNode(c, c.id);
			currentNode = readNode(c.id);
		}
	}
	else
	{
		if (currentNode.balanceFactor == -1)
		{
			currentParent = readNode(LastImbalanceId);
			currentParent.rChild = currentNode.lChild;
			currentNode.lChild = currentParent.id;
			currentNode.balanceFactor = currentParent.balanceFactor = 0;
			saveNode(currentNode, currentNode.id);
			saveNode(currentParent, currentParent.id);
		}
		else
		{
			currentParent = readNode(LastImbalanceId);
			c = readNode(currentNode.lChild);
			currentNode.lChild = c.rChild;
			currentParent.rChild = c.lChild;
			c.lChild = currentParent.id;
			c.rChild = currentNode.id;			
			saveNode(currentNode, currentNode.id);
			saveNode(currentParent, currentParent.id);
			saveNode(c, c.id);
			switch (c.balanceFactor)
			{
			case 0:
				currentNode.balanceFactor = currentParent.balanceFactor = 0;
				saveNode(currentNode, currentNode.id);
				saveNode(currentParent, currentParent.id);
				break;
			case  1:
				currentNode.balanceFactor = 0;
				currentParent.balanceFactor = -1;
				saveNode(currentNode, currentNode.id);
				saveNode(currentParent, currentParent.id);
				break;
			case -1:
				currentParent.balanceFactor = 0;
				currentNode.balanceFactor = 1;
				saveNode(currentNode, currentNode.id);
				saveNode(currentParent, currentParent.id);
				break;
			}
			c.balanceFactor = 0;
			saveNode(c, c.id);
			currentNode = readNode(c.id);
		}
	}
	c = readNode(LastImbalanceParentId);
	if (c.id == 0)
	{
		root = currentNode.id;
		return;
	}
	if (c.lChild == currentParent.id)
	{
		c.lChild = currentNode.id;
		saveNode(c, c.id);
		return;
	}
	if (c.rChild == currentParent.id)
	{
		c.rChild = currentNode.id;
		saveNode(c, c.id);
		return;
	}
}


AVLNode AVL::readNode(int nodeID)
{
	readCount++;
	std::fstream stream;
	stream.open(treeLocation,std::ios::in|std::ios::binary);
	stream.seekg(sizeof(AVLNode)*nodeID);
	AVLNode toReturn;
	stream.read(reinterpret_cast<char*>(&toReturn), sizeof(AVLNode));
	stream.close();
	return toReturn;
}

void AVL::saveNode(AVLNode toSave, int nodeID)
{
	writeCount++;
	std::fstream stream;
	stream.open(treeLocation, std::ios::out | std::ios::binary | std::ios::in);
	stream.seekp(sizeof(AVLNode)*nodeID);
	stream.write(reinterpret_cast<const char*>(&toSave), sizeof(toSave));
	stream.close();
}

void AVL::list()
{
	AVLNode avlRoot = readNode(root);
	listImpl(avlRoot);
}


void AVL::listImpl(AVLNode currentNode)
{
	
	if (currentNode.lChild != 0)
	{
		listImpl(readNode(currentNode.lChild));
	}
	std::cout << currentNode.Payload << " " << currentNode.count << std::endl;
	if (currentNode.rChild != 0)
	{
		listImpl(readNode(currentNode.rChild));
	}	
	
	
}

void AVL::reportHeight()
{
	if(root!= 0)
	{
		reportHeightImpl(readNode(root), 1);
	}
	std::cout << "AVL Tree height : " << height << std::endl;
}

void AVL::reportHeightImpl(AVLNode currentNode, int currentHeight)
{
	if(currentHeight > height)
	{
		height = currentHeight;
	}
	if(currentNode.lChild != 0)
	{
		reportHeightImpl(readNode(currentNode.lChild), currentHeight + 1);
	}
	if(currentNode.rChild != 0)
	{
		reportHeightImpl(readNode(currentNode.rChild), currentHeight + 1);
	}
}

void AVL::reportTotalNumberOfNodesAndWords()
{
	if(root != 0)
	{
		reportTotalNumberOfNodesAndWordsImpl(readNode(root));
	}	
	std::cout << "total nodes (unique words): " <<totalNodes << std::endl;
	std::cout << "total items (unique and duplicates): " << totalWords << std::endl;
}

void AVL::reportTotalNumberOfNodesAndWordsImpl(AVLNode currentNode)
{
	totalWords = totalWords + currentNode.count;
	if(currentNode.lChild != 0)
	{
		totalNodes++;
		reportTotalNumberOfNodesAndWordsImpl(readNode(currentNode.lChild));
	}
	if(currentNode.rChild != 0)
	{
		totalNodes++;
		reportTotalNumberOfNodesAndWordsImpl(readNode(currentNode.rChild));
	}
}









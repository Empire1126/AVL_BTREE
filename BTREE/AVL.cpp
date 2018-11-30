#include "pch.h"
#include "AVL.h"



AVL::AVL()
{
	treeLocation = "AVL_TREE_DATA.AVLTree";
	std::fstream stream;
	stream.open(treeLocation, std::fstream::out | std::fstream::binary);
	AVLNode nil;
	stream.write(reinterpret_cast<const char*>(&nil), sizeof(AVLNode));
	stream.close();
}

AVL::AVL(std::string treeFileLocation)
{
	std::fstream stream;
	stream.open(treeFileLocation,std::fstream::out | std::fstream::binary);
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
	AVLNode result = searchImpl(toFind);
	if(result.id==0)
	{
		std::cout << "" << std::endl;
	}
	else
	{
		std::cout << result.Payload << " " << result.count << std::endl;
	}
}

AVLNode AVL::searchImpl(std::string toFind)
{
	AVLNode currentNode = readNode(root);
	while (currentNode.id!=0)
	{
		if(strcmp(toFind.c_str(),currentNode.Payload)==0)
		{
			return currentNode;
		}
		else  if(strcmp(toFind.c_str(),currentNode.Payload)<0)
		{
			currentNode = readNode(currentNode.lChild);
		}
		else
		{
			currentNode = readNode(currentNode.rChild);
		}
	}
	AVLNode nil;
	return  nil;
}


void AVL::insert(std::string toAdd)
{
	insertImpl(toAdd);
}

void AVL::insertImpl(std::string toAdd)
{
	if (root == 0)
	{
		AVLNode newNode;
		strcpy_s(newNode.Payload, toAdd.c_str());
		newNode.lChild = newNode.rChild = 0;
		newNode.count=1;
		newNode.balanceFactor = 0;
		newNode.id = ++nextNodeId;
		root = nextNodeId;
		saveNode(newNode, newNode.id);
		return;
	}
	int displacement = 0;
	unsigned int LastImbalanceId = root;
	unsigned int LastImbalanceParentId = 0;
	unsigned int currentParentId = 0;
	unsigned int lastImbalanceChildId = 0;
	AVLNode currentNode = readNode(root);
	while (currentNode.id != 0)
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





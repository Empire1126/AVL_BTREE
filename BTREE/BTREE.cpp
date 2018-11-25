#include "pch.h"
#include "BTREE.h"

BTREE::BTREE()
{
	treeLocation = "B_TREE_DATA.BTree";
	std::fstream stream;
	stream.open(treeLocation, std::fstream::out);
	BTreeNode nil;
	stream.write(reinterpret_cast<const char*>(&nil), sizeof(BTreeNode));
	stream.close();
}

BTREE::BTREE(std::string treeFileLocation)
{
	treeLocation = treeFileLocation;
	std::fstream stream;
	stream.open(treeLocation, std::fstream::out);
	BTreeNode nil;
	stream.write(reinterpret_cast<const char*>(&nil), sizeof(BTreeNode));
	stream.close();
}


BTREE::~BTREE()
{
	
}

void BTREE::SplitChild(struct BTreeNode fullNodeParent, int middleIndex)
{
	BTreeNode sibling;
	sibling.id = ++nextNodeId;
	BTreeNode FullNode = readNode(fullNodeParent.children[middleIndex]);
	sibling.isLeaf = FullNode.isLeaf;
	sibling.numberOfKeys = minimumDegree - 1;
	for(int j =1 ; j < minimumDegree -1 ; j++)
	{
		sibling.keys[j] = FullNode.keys[j + 1];
	}
	if(!FullNode.isLeaf)
	{
		for(int j = 1 ; j<minimumDegree;j++)
		{
			sibling.children[j] = FullNode.children[j + minimumDegree];
		}
	}
	FullNode.numberOfKeys = minimumDegree - 1;
	for(int j = fullNodeParent.numberOfKeys+1;j>minimumDegree+1;j--)
	{
		fullNodeParent.children[j + 1] = fullNodeParent.children[j];
	}
	fullNodeParent.children[middleIndex + 1] = sibling.id;
	for(int j = fullNodeParent.numberOfKeys;j>middleIndex;j--)
	{
		fullNodeParent.keys[j + 1] = fullNodeParent.keys[j];
	}
	fullNodeParent.keys[middleIndex] = FullNode.keys[middleIndex];
	fullNodeParent.numberOfKeys = fullNodeParent.numberOfKeys + 1;
	saveNode(FullNode, FullNode.id);
	saveNode(sibling, sibling.id);
	saveNode(fullNodeParent, fullNodeParent.id);
}

void BTREE::insert(std::string toAdd)
{
	//must get duplicates
}

void BTREE::search(std::string toAdd)
{
	
}

BTREE::BTreeNode BTREE::searchImpl(std::string toFind)
{
	
}


void BTREE::insertImpl(std::string toAdd)
{
	BTreeNode rootNode = readNode(root);
	if(rootNode.numberOfKeys == ((2*minimumDegree)-1))
	{
		BTreeNode newNode;
		newNode.id = ++nextNodeId;
		root = newNode.id;
		newNode.isLeaf = 0;
		newNode.numberOfKeys = 0;
		newNode.children[1] = rootNode.id;
		SplitChild(newNode, 1);
		insertNonFull(newNode, toAdd);
	}
	else
	{
		insertNonFull(rootNode, toAdd);
	}
}

void BTREE::insertNonFull(struct BTreeNode nonFullNode, std::string toAdd)
{
	int i = nonFullNode.numberOfKeys;
	if(nonFullNode.isLeaf)
	{
		while(i >= 1 && strcmp(toAdd.c_str(),nonFullNode.keys[i].data) < 0)
		{
			nonFullNode.keys[i + 1] = nonFullNode.keys[i];
			i--;
		}
		BTreeKey newKey;
		strcpy_s(newKey.data, toAdd.c_str());
		newKey.count++;
		nonFullNode.keys[i + 1] = newKey;
		nonFullNode.numberOfKeys++;
		saveNode(nonFullNode, nonFullNode.id);
	}
	else 
	{
		while (i>=1 && strcmp(toAdd.c_str(),nonFullNode.keys[i].data) < 0)
		{
			i--;
		}
		i++;
		BTreeNode child = readNode(nonFullNode.children[i]);
		if(child.numberOfKeys==((2*minimumDegree)-1))
		{
			SplitChild(nonFullNode, i);
			if(strcmp(toAdd.c_str(),nonFullNode.keys[i].data)>0)
			{
				i++;
			}
		}
		insertNonFull(child, toAdd);
	}
}



BTREE::BTreeNode BTREE::readNode(int nodeId)
{
	std::fstream stream;
	stream.open(treeLocation, std::fstream::in);
	stream.seekg(sizeof(BTreeNode)*nodeId);
	BTreeNode toReturn;
	stream.read(reinterpret_cast<char*>(&toReturn), sizeof(BTreeNode));
	stream.close();
	return  toReturn;
}

void BTREE::saveNode(BTreeNode toSave, int nodeId)
{
	std::fstream stream;
	stream.open(treeLocation, std::fstream::out);
	stream.seekp(sizeof(BTreeNode)*nodeId);
	stream.write(reinterpret_cast<const char*>(&toSave), sizeof(BTreeNode));
	stream.close();
}



#include "pch.h"
#include "BTREE.h"

BTREE::BTREE()
{
	treeLocation = "B_TREE_DATA.BTree";
	std::fstream stream;
	stream.open(treeLocation, std::fstream::out | std::fstream::binary);
	BTreeNode nil;
	stream.write(reinterpret_cast<const char*>(&nil), sizeof(BTreeNode));
	BTreeNode newRoot;
	newRoot.id = ++nextNodeId;
	root = newRoot.id;
	stream.write(reinterpret_cast<const char*>(&newRoot), sizeof(BTreeNode));
	stream.close();
}

BTREE::BTREE(std::string treeFileLocation)
{
	treeLocation = treeFileLocation;
	std::fstream stream;
	stream.open(treeLocation, std::fstream::out | std::fstream::binary);
	BTreeNode nil;
	stream.write(reinterpret_cast<const char*>(&nil), sizeof(BTreeNode));
	BTreeNode newRoot;	
	newRoot.id = ++nextNodeId;
	root = newRoot.id;
	stream.write(reinterpret_cast<const char*>(&newRoot), sizeof(BTreeNode));
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
	for(int j =0 ; j < minimumDegree -1 ; j++)
	{
		sibling.keys[j] = FullNode.keys[j + minimumDegree];
	}
	if(!FullNode.isLeaf)
	{
		for(int j = 0 ; j<minimumDegree;j++)
		{
			sibling.children[j] = FullNode.children[j + minimumDegree];
		}
	}
	FullNode.numberOfKeys = minimumDegree - 1;
	for(int j = fullNodeParent.numberOfKeys;j>middleIndex;j--)
	{
		fullNodeParent.children[j + 1] = fullNodeParent.children[j];
	}
	fullNodeParent.children[middleIndex + 1] = sibling.id;
	for(int j = fullNodeParent.numberOfKeys-1;j>middleIndex-1;j--)
	{
		fullNodeParent.keys[j + 1] = fullNodeParent.keys[j];
	}
	fullNodeParent.keys[middleIndex] = FullNode.keys[minimumDegree-1];
	fullNodeParent.numberOfKeys = fullNodeParent.numberOfKeys + 1;
	saveNode(FullNode, FullNode.id);
	saveNode(sibling, sibling.id);
	saveNode(fullNodeParent, fullNodeParent.id);
}

void BTREE::insert(std::string toAdd)
{
	BTreeSearchResult resultOfSearch = searchImpl(readNode(root), toAdd);
	if(resultOfSearch.node.id==0)
	{
		insertImpl(toAdd);
	}
	else
	{
		resultOfSearch.node.keys[resultOfSearch.index].count++;
		saveNode(resultOfSearch.node, resultOfSearch.node.id);
		return;
	}
	//must get duplicates
}

void BTREE::search(std::string toFind)
{
	BTreeSearchResult resultOfSearch = searchImpl(readNode(root), toFind);
	if(resultOfSearch.node.id==0)
	{
		std::cout << "" << std::endl;
	}
	else
	{
		std::cout << resultOfSearch.node.keys[resultOfSearch.index].data << resultOfSearch.node.keys[resultOfSearch.index].count << std::endl;
	}
}

BTREE::BTreeSearchResult BTREE::searchImpl(BTreeNode selector,std::string toFind)
{
	int i = 0;
	while (i <= selector.numberOfKeys && strcmp(toFind.c_str(), selector.keys[i].data) > 0)
	{
		i++;
	}
	if (i <= selector.numberOfKeys && strcmp(toFind.c_str(), selector.keys[i].data) == 0)
	{
		BTreeSearchResult result;
		result.node = selector;
		result.index = i;
		return result;
	}
	if(selector.isLeaf)
	{
		BTreeSearchResult nilResult;
		BTreeKey nilKey;		
		nilResult.node = readNode(0);
		nilResult.index = 0;
		return nilResult;
	}
	else
	{
		return searchImpl(readNode(selector.children[i]), toFind);
	}	
	
}


void BTREE::insertImpl(std::string toAdd)
{
	BTreeNode rootNode = readNode(root);
	if(rootNode.numberOfKeys == ((2*minimumDegree)-1))
	{
		BTreeNode newNode;
		newNode.id = ++nextNodeId;		
		newNode.isLeaf = 0;
		newNode.numberOfKeys = 0;			
		newNode.children[0] = rootNode.id;
		root = newNode.id;
		SplitChild(newNode, 0);
		rootNode = readNode(root);
		insertNonFull(rootNode, toAdd);
	}
	else
	{
		insertNonFull(rootNode, toAdd);
	}
}

void BTREE::insertNonFull(struct BTreeNode nonFullNode, std::string toAdd)
{
	int i = nonFullNode.numberOfKeys-1;
	if(nonFullNode.isLeaf)
	{
		while(i >= 0 && strcmp(toAdd.c_str(),nonFullNode.keys[i].data) < 0)
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
		while (i>=0 && strcmp(toAdd.c_str(),nonFullNode.keys[i].data) < 0)
		{
			i--;
		}
		i++;
		BTreeNode child = readNode(nonFullNode.children[i]);
		if(child.numberOfKeys==((2*minimumDegree)-1))
		{
			SplitChild(nonFullNode, i);
			nonFullNode = readNode(nonFullNode.id);
			if(strcmp(toAdd.c_str(),nonFullNode.keys[i].data)>0)
			{
				i++;
			}
		}		
		insertNonFull(readNode(nonFullNode.children[i]), toAdd);
	}
}



BTREE::BTreeNode BTREE::readNode(int nodeId)
{
	readCount++;
	std::fstream stream;
	stream.open(treeLocation, std::ios::in | std::ios::binary);
	stream.seekg(sizeof(BTreeNode)*nodeId);
	BTreeNode toReturn;
	stream.read(reinterpret_cast<char*>(&toReturn), sizeof(BTreeNode));
	stream.close();
	return  toReturn;
}

void BTREE::saveNode(BTreeNode toSave, int nodeId)
{
	writeCount++;
	std::fstream stream;
	stream.open(treeLocation, std::ios::out|std::ios::binary| std::ios::in);
	stream.seekp(sizeof(BTreeNode)*nodeId);
	stream.write(reinterpret_cast<const char*>(&toSave), sizeof(BTreeNode));
	stream.close();
}

void BTREE::list()
{
	BTreeNode BTreeRoot = readNode(root);
	listImpl(BTreeRoot);
}

void BTREE::listImpl(BTreeNode currentNode)
{
	if(currentNode.children[1]!=0)
	{
		listImpl(readNode(currentNode.children[1]));
	}
	for(int j =1 ; j<=currentNode.numberOfKeys;j++)
	{
		std::cout << currentNode.keys[j].data << " " << currentNode.keys[j].count << std::endl;
		if(currentNode.children[j+1]!=0)
		{
			listImpl(readNode(currentNode.children[j + 1]));
		}		
	}

}





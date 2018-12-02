/*BTREE.cpp
 *Luke Pinkney
 *EECS 2510 Fall 2018
 *11/28/2018
 *Btree source file, contains function definitions of the btree data structure
 */
#include "pch.h"
#include "BTREE.h"
#include <iomanip>

BTREE::BTREE()
{
	//constructor for a new btree
	treeLocation = "B_TREE_DATA.BTree";//default tree file name
	std::fstream stream;
	stream.open(treeLocation, std::fstream::out | std::fstream::binary);
	BTreeNode nil;//nil node will function as a "null" node
	stream.write(reinterpret_cast<const char*>(&nil), sizeof(BTreeNode));//write nil to the file
	BTreeNode newRoot;//set a new root btree node
	newRoot.id = ++nextNodeId;//increment the unique node id tracker before setting the new root nodes id to it.
	root = newRoot.id;//set the new root
	stream.write(reinterpret_cast<const char*>(&newRoot), sizeof(BTreeNode));//write the root to the file
	stream.close();
}

BTREE::BTREE(std::string treeFileLocation)
{
	//if a different file name for the tree data is desired then this constructor will used that passed iin filename instead of the default
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
	//this method will split a full node that is supposed to be the child of the btree node passed into the function, the index
	//is the index at which the full child exists in the parent node
	BTreeNode sibling;//create a new sibling node in order to contain one half of the node that will be split
	sibling.id = ++nextNodeId;//set its unique id
	BTreeNode FullNode = readNode(fullNodeParent.children[middleIndex]);//read in the full child
	sibling.isLeaf = FullNode.isLeaf;//if the child was a leaf then the sibling will be too
	sibling.numberOfKeys = minimumDegree - 1;//the sibling will get the minimum number of keys a btree node can have
	for(int j =0 ; j < minimumDegree -1 ; j++)//transfer over the greater half of the full child's keys to the new sibling node
	{
		sibling.keys[j] = FullNode.keys[j + minimumDegree];
	}
	
	if(!FullNode.isLeaf)
	{
		//if the node was not a leaf then we have to transfer the children of the full node over to the sibling
		for(int j = 0 ; j<minimumDegree;j++)
		{
			sibling.children[j] = FullNode.children[j + minimumDegree];
		}
	}
	FullNode.numberOfKeys = minimumDegree - 1;//the full node now only has the minimum number of keys a btree node can have
	for(int j = fullNodeParent.numberOfKeys;j>middleIndex;j--)//move the children of the parent node over by 1 index each up to the number of keys +1
	{
		fullNodeParent.children[j + 1] = fullNodeParent.children[j];
	}
	fullNodeParent.children[middleIndex + 1] = sibling.id;//add the new sibling to the parent's children array
	for(int j = fullNodeParent.numberOfKeys-1;j>middleIndex-1;j--)//move the keys of the parent over up to the number of keys
	{
		fullNodeParent.keys[j + 1] = fullNodeParent.keys[j];
	}
	fullNodeParent.keys[middleIndex] = FullNode.keys[minimumDegree-1];//add the middle key from the full node to the parent
	fullNodeParent.numberOfKeys = fullNodeParent.numberOfKeys + 1;//parent now has one more key
	//save all three keys
	saveNode(FullNode, FullNode.id);
	saveNode(sibling, sibling.id);
	saveNode(fullNodeParent, fullNodeParent.id);
}

void BTREE::insert(std::string toAdd)
{
	//insert public function - takes in the string to add to the tree
	BTreeSearchResult resultOfSearch = searchImpl(readNode(root), toAdd);// find if the word is already in the tree
	if(resultOfSearch.node.id==0) // if the result of the search was nil
	{
		insertImpl(toAdd);//call the insert implementation with the new word too insert
	}
	else
	{
		resultOfSearch.node.keys[resultOfSearch.index].count++;//otherwise if the node as found by the search then increment the count of the key in that node
		saveNode(resultOfSearch.node, resultOfSearch.node.id);//save the node
		return;
	}
	//must get duplicates
}

void BTREE::search(std::string toFind)
{
	//public search function
	BTreeSearchResult resultOfSearch = searchImpl(readNode(root), toFind);//call search implementation with the word to search for
	if(resultOfSearch.node.id==0)//if the result was nil
	{
		std::cout << "" << std::endl;//output blank line
	}
	else
	{
		//otherwise if node was found output the word and its count
		std::cout << resultOfSearch.node.keys[resultOfSearch.index].data << resultOfSearch.node.keys[resultOfSearch.index].count << std::endl;
	}
}

BTREE::BTreeSearchResult BTREE::searchImpl(BTreeNode selector,std::string toFind)
{
	//search implementation takes in a node to start from and a string to search for 
	int currentIndex = 0;//integer to keep track of current key we are at
	while (currentIndex <= selector.numberOfKeys && strcmp(toFind.c_str(), selector.keys[currentIndex].data) > 0)		
	{
		//as long as we haven't gotten to the end of the node and the string we are searching for is greater than the current key we are at
		//keep incrementing the current index
		currentIndex++;
	}
	
	if (currentIndex <= selector.numberOfKeys && strcmp(toFind.c_str(), selector.keys[currentIndex].data) == 0)
	{
		//if we haven't exceeded the max index in the node and the word we are looking for is equal to the current index's key
		BTreeSearchResult result;//new result to return
		result.node = selector;//store the node that the word was found
		result.index = currentIndex;//store the index we found the word at
		return result;//return the index and node
	}
	//otherwise we didn't find the word in this node
	if(selector.isLeaf)
	{
		//if the node is a leaf then the word isn't in the tree return a nil node
		BTreeSearchResult nilResult;
		BTreeKey nilKey;		//nil node with id =0
		nilResult.node = readNode(0);
		nilResult.index = 0;
		return nilResult;
	}
	else
	{
		//otherwise traverse recursively down the trees child at the current index 
		return searchImpl(readNode(selector.children[currentIndex]), toFind);
	}	
	
}


void BTREE::insertImpl(std::string toAdd)
{
	//insert implementation of the btree takes a string to insert into the tree
	BTreeNode rootNode = readNode(root);//start at the root node
	if(rootNode.numberOfKeys == ((2*minimumDegree)-1))//if the root is full
	{
		//we must make a new root and set the old root as the new roots child and split the old root
		BTreeNode newNode;
		newNode.id = ++nextNodeId;		//increment the unique node tracker and set the new roots id equal to it
		newNode.isLeaf = 0;//the new root is not a leaf
		newNode.numberOfKeys = 0;			//the new root has zeor keys
		newNode.children[0] = rootNode.id;//the new roots first child is the old root
		root = newNode.id;//new root set
		SplitChild(newNode, 0);//split the old root that is full
		rootNode = readNode(root);//read in the new root after the split
		insertNonFull(rootNode, toAdd);//insert the word into the now non full root
	}
	else
	{
		insertNonFull(rootNode, toAdd);//otherwise the root isn't full insert the word into a non full node
	}
}

void BTREE::insertNonFull(struct BTreeNode nonFullNode, std::string toAdd)
{
	//this function is for inserting into a non full node recursively, it takes a non full node and a the word to add
	int currentIndex = nonFullNode.numberOfKeys-1;//current index we are at in the current node, start at the end of the node's keys
	if(nonFullNode.isLeaf)
	{
		//if the current node is a leaf
		while(currentIndex >= 0 && strcmp(toAdd.c_str(),nonFullNode.keys[currentIndex].data) < 0)//while the current index is not zero and the node we are inserting is less than
			//the current index's keys. 
		{
			nonFullNode.keys[currentIndex + 1] = nonFullNode.keys[currentIndex];//keep moving the keys over by one 
			currentIndex--;//decrement to the next index
		}
		//once we are are either at zero in the non full node or where the current index's key is less than the word we are inserting we are at the place to insert the word
		BTreeKey newKey;//make a new key
		strcpy_s(newKey.data, toAdd.c_str());//copy the word we are inserting into the new key
		newKey.count++;//increment the count
		nonFullNode.keys[currentIndex + 1] = newKey;//insert the new key into the keys array of the current node
		nonFullNode.numberOfKeys++;//the node has another key
		saveNode(nonFullNode, nonFullNode.id);//save the node
	}
	else 
	{
		//if the current node is not a leaf
		while (currentIndex>=0 && strcmp(toAdd.c_str(),nonFullNode.keys[currentIndex].data) < 0)
		{
			//similarly to above we will decrement the current index until either we are at zero in the keys array or the current index's key is less than the word we are inserting
			currentIndex--;
		}
		currentIndex++;//increase the index to be at the appropriate child
		BTreeNode child = readNode(nonFullNode.children[currentIndex]);//read in the child 
		if(child.numberOfKeys==((2*minimumDegree)-1))//if the child is full
		{
			SplitChild(nonFullNode, currentIndex);//split the child
			nonFullNode = readNode(nonFullNode.id);//read in the new split child
			if(strcmp(toAdd.c_str(),nonFullNode.keys[currentIndex].data)>0)//if the current key brought up to this node from the full child is less than the word we are inserting
			{
				currentIndex++;//increment the current index
			}
		}		
		insertNonFull(readNode(nonFullNode.children[currentIndex]), toAdd);//regardless of whether we split or not recursively insert the word into the non full child node
	}
}



BTREE::BTreeNode BTREE::readNode(int nodeId)
{
	//this function takes a nodeId and reads in its data from the disk and return that node
	readCount++;
	std::fstream stream;
	stream.open(treeLocation, std::ios::in | std::ios::binary);
	stream.seekg(sizeof(BTreeNode)*nodeId);//seek to the position in the file that has the node we seek, based on the nodes id
	BTreeNode toReturn;//new node to return
	stream.read(reinterpret_cast<char*>(&toReturn), sizeof(BTreeNode));//read in an avlNode long amount of data into the memory allocated avl node at the current position
	stream.close();
	return  toReturn;
}

void BTREE::saveNode(BTreeNode toSave, int nodeId)
{
	//this function takes a btree node and a node id and saves the btree node to the btree file location
	writeCount++;
	std::fstream stream;
	stream.open(treeLocation, std::ios::out|std::ios::binary| std::ios::in);//in order to not truncate the btree file data you must use the std::ios::in flag
	//with the file streams open method. Still not sure why this is needed
	stream.seekp(sizeof(BTreeNode)*nodeId);//seek to the location specified by the node id parameter
	stream.write(reinterpret_cast<const char*>(&toSave), sizeof(BTreeNode));//save the btree node parameter to the location in the file
	stream.close();
}

void BTREE::list()
{
	//wrapper to call an in order traversal of the btree and print its contents
	BTreeNode BTreeRoot = readNode(root);//read in the root of the tree
	listImpl(BTreeRoot);//call the implementation with the root
}

void BTREE::listImpl(BTreeNode currentNode)
{
	//recursively calls itself to print out the contents of the btree in an in order fashion
	if(currentNode.children[0]!=0)//if the first child is not nil
	{
		listImpl(readNode(currentNode.children[0]));//traverse down it recursively
	}
	for(int j =1 ; j<=currentNode.numberOfKeys;j++)//otherwise for all keys in the tree
	{
		std::cout << currentNode.keys[j].data << " " << currentNode.keys[j].count << std::endl;//print out the key and its data
		if(currentNode.children[j+1]!=0)//if the child is not nil
		{
			listImpl(readNode(currentNode.children[j + 1]));//recursively call the in order traversal
		}		
	}

}

void BTREE::reportHeight()
{
	//wrapper to calculate the height of the tree
	if(root!=0)
	{
		reportHeightImpl(readNode(root), 1);//call the height implementation using the root and a 1 to include the root in the calculation 
	}
	std::cout << "Btree Tree Height: "<<height<<std::endl;
}

void BTREE::reportHeightImpl(BTreeNode currentNode, int currentHeight)
{
	//this method will recursively traverse the tree and calculate the height of the tree
	//every-time a a child is determined to not be nil then we recurse down that subtree and add 1 to the current height variable
	//thus every call of the function will have its own increasing height variable that if larger than the class wide height variable will replace
	//the class variable
	if(currentHeight > height)
	{
		height = currentHeight;
	}
	for(int i = 0; i<=currentNode.numberOfKeys;i++)
	{
		if(currentNode.children[i]!=0)
		{
			reportHeightImpl(readNode(currentNode.children[i]), currentHeight + 1);
		}
	}
}


void BTREE::reportTotalNumberOfNodesAndWords()
{
	//wrapper to call function to calculate and report the total words and nodes in the btree
	if(root!=0)
	{
		reportTotalNumberOfNodesAndWordsImpl(readNode(root));//calculate the total words and nodes in the tree
	}
	std::cout << "total nodes (unique words): "<< totalNodes<<std::endl;
	std::cout << "total items (unique and duplicates): " << totalWords << std::endl;
}

void BTREE::reportTotalNumberOfNodesAndWordsImpl(BTreeNode currentNode)
{
	//in this method for every key in a node we will add all its key's counts to the total words of the tree
	//we will also check all children of the node if they are nil. if not then re recurse down those subtrees
	//we also increment the total nodes variable for every node we traverse too.
	for(int i =0;i<currentNode.numberOfKeys;i++)
	{
		totalWords = totalWords + currentNode.keys[i].count;
	}
	for(int i =0;i<=currentNode.numberOfKeys;i++)
	{
		if(currentNode.children[i]!=0)
		{
			totalNodes++;
			reportTotalNumberOfNodesAndWordsImpl(readNode(currentNode.children[i]));
		}
	}
}

void BTREE::reportLoadingFactor()
{
	if(root!=0)
	{
		reportLoadingFactorImpl(readNode(root));
	}
	std::cout << "loading factor of btree: " << std::setprecision(2) << (float)(totalUsedKeys / ((((minimumDegree) * 2) - 1)*totalNodes)) << " % " << std::endl;
}

void BTREE::reportLoadingFactorImpl(BTreeNode currentNode)
{			
	totalUsedKeys = totalUsedKeys + currentNode.numberOfKeys;
	for(int i = 0;i<=currentNode.numberOfKeys;i++)
	{
		if(currentNode.children[i]!=0)
		{
			reportLoadingFactorImpl(readNode(currentNode.children[i]));
		}
	}
	
}

void BTREE::tellBtreeFileSize()
{
	std::fstream inFile;
	inFile.open(treeLocation, std::ios::binary| std::ios::in);
	inFile.seekg(0, std::ios::end);
	std::streampos btreeFileSize = inFile.tellg();
	std::cout << "btree tree file size: " << btreeFileSize << std::endl;
	inFile.close();
}








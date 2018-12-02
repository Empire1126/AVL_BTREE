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
		if (strcmp(currentNode.Payload, toAdd.c_str()) == 0)//if the current node we are at has a payload equal to the string we are trying to insert into the tree
		{
			currentNode.count++;//increment the count of the word in the tree
			saveNode(currentNode, currentNode.id);//save the node with the increased count
			return; //return we are done
		}
		//if the node isn't the word we are looking for
		if (currentNode.balanceFactor != 0) // is the current node imbalanced?
		{
			LastImbalanceId = currentNode.id;//if it is record its id as the last imbalance we saw
			LastImbalanceParentId = currentParentId; //record its parent
		}
		//determine whether to traverse left or right
		if (strcmp(toAdd.c_str(), currentNode.Payload) < 0) // is the word we are inserting less than the current node?
		{
			//if the word we are inserting is less than the current node
			currentParentId = currentNode.id;//set the parent to the current node
			currentNode = readNode(currentNode.lChild); // set the current node to the left child of the current node
		}
		else
		{
			//otherwise traverse right
			currentParentId = currentNode.id; // set the parent to the current node
			currentNode = readNode(currentNode.rChild); // set the current node to the right child of the current node
		}
	}
	//at this point the word inserted is a new unique word to insert  into the tree
	// the current node is "null" so use it as the new node to insert into the tree
	strcpy_s(currentNode.Payload, toAdd.c_str());  //copy the word into the node
	currentNode.rChild = 0;//set the right and left child of the new leaf node
	currentNode.lChild = 0;
	currentNode.balanceFactor = 0; //new leafs are balanced by default
	currentNode.id = ++nextNodeId;//increment the node if tracker and then the node id to the tracker value;
	currentNode.count = 1; //new node count is 1
	saveNode(currentNode, currentNode.id); //save the new node to the tree file
	unsigned int newNodeId = currentNode.id; // record the new nodes id
	AVLNode currentParent = readNode(currentParentId); // get the last node we looked at , the parent node
	if (strcmp(toAdd.c_str(), currentParent.Payload) < 0) //does the new node belong as the left or right child of the parent node?
	{
		//if the new node payload is less than the parent's
		currentParent.lChild = currentNode.id; //set the new node as the left child of the parent
		
	}
	else
	{
		currentParent.rChild = currentNode.id;//otherwise set the new node as the right child
		
	}
	saveNode(currentParent, currentParent.id); //save the parent
	//standard BST insert complete, now we must find and fix any potential imbalances that have occured
	currentParent = readNode(LastImbalanceId); // get the last imbalanced node we saw
	if (strcmp(toAdd.c_str(), currentParent.Payload) > 0) //is the new node inserted to the right of the last imbalance saw? 
	{
		lastImbalanceChildId = currentParent.rChild; //if so then the child of the last imbalance is the right child
		displacement = -1; //the displacement is -1 for a right child
	}
	else
	{
		lastImbalanceChildId = currentParent.lChild; //otherwise the child of the last imbalance is its left child
		displacement = 1;//left imbalances are 1
	}
	currentNode = readNode(lastImbalanceChildId); //read in the child of the last imbalance
	while (currentNode.id != newNodeId) 
	{
		//we will now traverse down until we get to the new node recently added ot the tree and adjust the balance factors in order to detect any possible imbalances
		if (strcmp(toAdd.c_str(), currentNode.Payload) > 0) //is the new word to the right of the current node?
		{
			//if so then set the balance factor of the current node to -1
			currentNode.balanceFactor = -1;
			//save that node
			saveNode(currentNode, currentNode.id);
			//traverse down the right child of the current node
			currentNode = readNode(currentNode.rChild);
		}
		else
		{
			//otherwise set the current balance factor to 1 and traverse down the left child of the subtree
			currentNode.balanceFactor = 1;
			saveNode(currentNode, currentNode.id);
			currentNode = readNode(currentNode.lChild);
		}
	}
	currentNode = readNode(LastImbalanceId); //read in the last imbalanced node (the top of our imbalanced subtree)
	if (currentNode.balanceFactor == 0)//was the top of the subtree balanced before the insert?
	{
		//if so then the subtree is acceptably imbalanced;
		currentNode.balanceFactor = displacement;//set the top of the subtrees balance to the displacement, representing 
		// the new node was to the right or left of the top of the subtree
		saveNode(currentNode, currentNode.id);
		return;
	}
	if (currentNode.balanceFactor == -displacement)
	{
		//if the top of the imbalanced subtree was already imbalanced in the opposite direction
		//the new node inserted has canceled the current imbalance
		currentNode.balanceFactor = 0;//thus the subtree is now balancedd
		saveNode(currentNode, currentNode.id);
		return;
	}
	//if none of the above returns are taken then the tree is now in an unacceptable balance and must be rotated in some way ( LL, LR, RR, or RL)
	currentNode = readNode(lastImbalanceChildId);//read in the chiild of the last imbalance
	AVLNode c;//extra node for RL and LR rotations
	if (displacement == 1)
	{
		//if the new node was to the left of the child of the last imbalance
		if (currentNode.balanceFactor == 1)		
		{
			//and the last imbalances child already was imbalanced to the left we must do a LL rotation
			// in a Left left rotation the last imbalanced node seen while inserting is the top of the imbalanced subtree 
			//this node's left child will become the right child of its imbalanced child
			//the imbalanced child's right child will become the top of the subtree (the last imbalanced node found while inserting)
			//all nodes are balanced now and we save the two nodes (the imbalanced child and the last imbalanced node, the old top of the subtree), node c is not used
			currentParent = readNode(LastImbalanceId);
			currentParent.lChild = currentNode.rChild;
			currentNode.rChild = currentParent.id;
			currentNode.balanceFactor = currentParent.balanceFactor = 0;
			saveNode(currentNode, currentNode.id);
			saveNode(currentParent, currentParent.id);
		}
		else
		{
			//if the last imbalances child was imbalanced to the right then it is a LR rotation
			//in a LR rotation the node c will be used. Here c will be right child of the imbalanced nodes child.			
			currentParent = readNode(LastImbalanceId);
			c = readNode(currentNode.rChild);
			//the last imbalanced node's child's right child gets c's left child
			currentNode.rChild = c.lChild;
			//the last imbalanced node's left child gets c's right child
			currentParent.lChild = c.rChild;
			//c's left child is the last imbalanced node's child 
			c.lChild = currentNode.id;
			//c's right child is the last imbalanced node
			c.rChild = currentParent.id;
			//save all of the nodes changed
			saveNode(currentNode, currentNode.id);
			saveNode(currentParent, currentParent.id);
			saveNode(c, c.id);
			switch (c.balanceFactor)
			{
				//now depending on the balance state of c the last imbalanced node and last imbalanced node's old child's balance factor will change
			case  0://if c was balanced before hand then both the last imbalanced node and its old child are now balanced
				currentNode.balanceFactor = 0;
				currentParent.balanceFactor = 0;
				saveNode(currentNode, currentNode.id);
				saveNode(currentParent, currentParent.id);
				break;
			case 1://if c had a left imbalance
				currentNode.balanceFactor = -1; //then the last imbalanced node old child will be right imbalanced
				currentParent.balanceFactor = 0;//the last imbalanced node is balanced
				saveNode(currentNode, currentNode.id);
				saveNode(currentParent, currentParent.id);
				break;
			case -1://if c had a right imbalance
				currentNode.balanceFactor = 0;//the last imbalanced node's old child is balanced
				currentParent.balanceFactor = 1;//the last imbalanced node's balance is now left imbalanced
				saveNode(currentNode, currentNode.id);
				saveNode(currentParent, currentParent.id);
				break;
			}
			c.balanceFactor = 0;//c is balanced now either way
			saveNode(c, c.id);//save c 
			currentNode = readNode(c.id); //read in c as the current node as it will now have to be referenced by the last imbalanced node's old parent
		}
	}
	else
	{
		//if the new node inserted into the tree was instead to the right of the last imbalanced node
		if (currentNode.balanceFactor == -1)
		{
			//and the child of the last imbalance had a right imbalance we must  do a Right Right rotation
			//in a rr rotation the last imbalanced node's right child becomes the left child of its imbalanced child
			//the imbalanced node's imbalanced child's left child is now the last imbalanced node, the old top of the imbalanced subtree
			currentParent = readNode(LastImbalanceId);
			currentParent.rChild = currentNode.lChild;
			currentNode.lChild = currentParent.id;
			//both the old top of the subtree and its child are now balanced
			currentNode.balanceFactor = currentParent.balanceFactor = 0;
			saveNode(currentNode, currentNode.id);
			saveNode(currentParent, currentParent.id);
		}
		else
		{
			//otherwise if the child of the last imbalanced node had a left displacement then it is a Right Left rotation
			//in a RL rotation we will use c as the left child of the last imbalanced node's (that is top of the imbalanced subtree) child
			currentParent = readNode(LastImbalanceId);
			c = readNode(currentNode.lChild);
			//the child of the top of the subtree will have its left child set to the right child of c
			//the top of the imbalanced subtree (the last imbalanced node found during BST insert) will have its right child set to the left child of c 
			currentNode.lChild = c.rChild;
			currentParent.rChild = c.lChild;
			//c's left child will become the top of the imbalanced subtree
			c.lChild = currentParent.id;
			//c's right child will become the imbalanced child of the top of the subtree
			c.rChild = currentNode.id;			
			saveNode(currentNode, currentNode.id);
			saveNode(currentParent, currentParent.id);
			saveNode(c, c.id);
			switch (c.balanceFactor)//adjust the balance factors based on c's balance
			{
			case 0://if c was balanced before the rotation
				currentNode.balanceFactor = currentParent.balanceFactor = 0;//the old top of the subtree and its child are now balanced
				saveNode(currentNode, currentNode.id);
				saveNode(currentParent, currentParent.id);
				break;
			case  1://if c had a left imbalance then the old top of the subtree has a right imbalance now
				currentNode.balanceFactor = 0;
				currentParent.balanceFactor = -1;
				saveNode(currentNode, currentNode.id);
				saveNode(currentParent, currentParent.id);
				break;
			case -1://if c had a right imbalance then now the last imbalanced node's child has a left imbalance
				currentParent.balanceFactor = 0;
				currentNode.balanceFactor = 1;
				saveNode(currentNode, currentNode.id);
				saveNode(currentParent, currentParent.id);
				break;
			}
			c.balanceFactor = 0;//c is now balanced
			saveNode(c, c.id);
			currentNode = readNode(c.id);//set current node to c as we will fix the last imbalanced node's parent's child pointer next
		}
	}
	c = readNode(LastImbalanceParentId);//read in the last imbalanced node's parent
	if (c.id == 0)//if the parent was null the root was the top of the imbalanced subtree (thus of the whole tree) and the root of the tree must change
	{
		root = currentNode.id;//set the root to reflect the rotation preformed
		return;
	}
	if (c.lChild == currentParent.id)
	{
		//otherwise if the left child of the imbalanced parent was the last imbalanced node
		c.lChild = currentNode.id;//change the left child of the parent to reflect the new top of the subtree
		saveNode(c, c.id);
		return;
	}
	if (c.rChild == currentParent.id)
	{
		//if the right child of the parent was the old top of the subtree
		c.rChild = currentNode.id;//set the right child of the parent to equal the new top of the subtree
		saveNode(c, c.id);
		return;
	}
	//should never be here
}


AVLNode AVL::readNode(int nodeID)
{
	//this function takes an integer representing the id of a node to read from the tree's data file on the disk drive
	readCount++;
	std::fstream stream;
	stream.open(treeLocation,std::ios::in|std::ios::binary);//open the tree's data file
	stream.seekg(sizeof(AVLNode)*nodeID);//set to the position of the node based on its id * size of avl nodes
	AVLNode toReturn;//new node to return as the data from the file
	stream.read(reinterpret_cast<char*>(&toReturn), sizeof(AVLNode)); //copy the contents of the file at our current location into the avlNode we will return
	//the stream will copy an avlnode size of data from the file into the avlnode in memory
	stream.close();
	return toReturn;//return the data filled avlnode
}

void AVL::saveNode(AVLNode toSave, int nodeID)
{
	//function will save an avl node passed in at the specified location given by the parameter nodeID
	writeCount++;
	std::fstream stream;
	stream.open(treeLocation, std::ios::out | std::ios::binary | std::ios::in);//open the file of the trees data, fstream std::ios::out will truncate the file behind a write of all data
	//this problem is resolved via adding std::ios::in to the stream. As far as I know there is no documentation on microsofts dev net so specify why adding std::ios::in fixes this. 
	//fstream::out also causes this problem as it is the same value as std::ios::out
	stream.seekp(sizeof(AVLNode)*nodeID);//navigate to the location specified by the nodeId parameter
	stream.write(reinterpret_cast<const char*>(&toSave), sizeof(toSave));//write the node provided by the toSave parameter to the location we navigated to
	stream.close();
}

void AVL::list()
{
	//wrapper to call an in order print out of the avl tree
	AVLNode avlRoot = readNode(root);
	listImpl(avlRoot);//call the implimentation
}


void AVL::listImpl(AVLNode currentNode)
{
	//does a recursive traversal of the avl tree to print out all nodes and counts in an in order fashion
	if (currentNode.lChild != 0)//if the left child of the current node is not "null"
	{
		listImpl(readNode(currentNode.lChild));//recursively call this function using the left child node
	}
	std::cout << currentNode.Payload << " " << currentNode.count << std::endl;//print out this nodes contents
	if (currentNode.rChild != 0)//similarly if the right child isnt null
	{
		listImpl(readNode(currentNode.rChild));// call this function with the right child node
	}	
	
	
}

void AVL::reportHeight()
{
	//reports the height of the avl tree
	if(root!= 0)
	{
		//if the tree isn't empty then call the height implementation
		reportHeightImpl(readNode(root), 1);
	}
	std::cout << "AVL Tree height : " << height << std::endl;//report height
}

void AVL::reportHeightImpl(AVLNode currentNode, int currentHeight)
{
	//this function will recursively call itself in order to determine the height of the current avl tree
	//parameters - the current node we are are at recursively ; currentHeight - this is the current height that the current recursive stack instance of this function sees

	if(currentHeight > height)//if the class wide height variable is less then this current recursions height
	{
		height = currentHeight;//set the trees height to the current height
	}
	if(currentNode.lChild != 0)//of the current node's left child isn't "null"
	{
		reportHeightImpl(readNode(currentNode.lChild), currentHeight + 1);//recurse down the left subtree adding one to the current height to reflect the size of the subtree
	}
	if(currentNode.rChild != 0)//similarly if the right child isn't "null" then recurse down the right subtree, adding one to reflect the size of the subtree
	{
		reportHeightImpl(readNode(currentNode.rChild), currentHeight + 1);
	}
}

void AVL::reportTotalNumberOfNodesAndWords()
{
	//reports the total number of words and nodes in the avl tree
	if(root != 0)//if the tree isn't empty
	{
		reportTotalNumberOfNodesAndWordsImpl(readNode(root));//calculate the total words and nodes
	}
	//report the total words and nodes
	std::cout << "total nodes (unique words): " <<totalNodes << std::endl;
	std::cout << "total items (unique and duplicates): " << totalWords << std::endl;
}

void AVL::reportTotalNumberOfNodesAndWordsImpl(AVLNode currentNode)
{
	//this method will recursively traverse down the avl tree and will count up all nodes and words in the tree 
	totalWords = totalWords + currentNode.count;//words are the unique word + all duplicates of it, given by the count of the node
	if(currentNode.lChild != 0)//if the left child of the current node isn't "null"
	{
		totalNodes++;//there is another node and thus unique word in the tree
		reportTotalNumberOfNodesAndWordsImpl(readNode(currentNode.lChild));//recurse down the left subtree
	}
	if(currentNode.rChild != 0)//if the right child isn't "null"
	{
		//increase the total nodes counter
		totalNodes++;
		//traverse down the right subtree
		reportTotalNumberOfNodesAndWordsImpl(readNode(currentNode.rChild));
	}
}

void AVL::tellFileSize()
{
	std::fstream inFile;
	inFile.open(treeLocation, std::ios::binary|std::ios::in);
	inFile.seekg(0, std::ios::end);
	std::streampos avlTreeFileSize = inFile.tellg();
	std::cout << "avl tree file size: " << avlTreeFileSize << std::endl;
	inFile.close();
}










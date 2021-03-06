/*AVL_BTREES.cpp
 *Luke Pinkney
 *EECS 2510 Fall 2018
 *11/28/2018
 *entry point for avl & btree project, parser for file input, and display data from runs of avl and btree
 */

#include "pch.h"
#include "time.h"
#include "AVL.h"
#include "BTREE.h"


int main()
{
	//in main we parse the file input and feed it to the two separate trees while recording statistics
	char c;
	AVL* AVL_T= new AVL(); // make a new AVL tree
	BTREE * btree = new BTREE(); //make a new Btree
	char chari[50]; // assumes no word is longer than 49 characters
	memset(chari, 0, 50); // zero the word buffer
	int iPtr;
	bool IsDelimiter = false, WasDelimiter = false;

	std::ifstream inFile;
	const std::string filename = "Shakespeare.txt";
	inFile.open(filename, std::ios::binary);
	if (inFile.fail())
	{
		std::cout << "Unable to open input file\n\n"
			<< "Program Exiting\n\nPress ENTER to exit\n";
		std::cin.get(c);
		exit(1);
	}
	//In this first loop we will just parse the file and do nothing with the input words as to calculate the time to parse the file in question by the file system.
	//This will allow us to calculate the time the trees took to store the words minus the time to parse the file inputs and thus giving a more accurate performance indicator of the trees
	iPtr = 0;
	clock_t parsingTime; //clock variable to calculate parsing time
	parsingTime = clock(); // start the clock to time the parser
	inFile.get(c); // priming read
	while (!inFile.eof())
	{
		
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' ||
			c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' ||
			c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the
		// last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;
			std::string nextString = chari;	//since the trees insert use std::string as an input parameter we must convert the c string to a std::string
			memset(chari, 0, 50); // zero the word buffer
			iPtr = 0;
		}
		else if (!IsDelimiter) // if this isn’t a delimiter, keep going
		{
			chari[iPtr] = c;
			iPtr++;
		}
		else if (IsDelimiter && WasDelimiter)
		{
			// Do nothing -- two consecutive delimiters.
		}
		WasDelimiter = IsDelimiter; // for the NEXT iteration
		inFile.get(c);
	}
	inFile.close();
	parsingTime = clock() - parsingTime; // calculate the time that the file system took to parse the entire file. 
	
	inFile.open(filename, std::ios::binary);//Now open the file again and start inserting the entire file's words into the avl tree
	iPtr = 0;
	clock_t avlTime; 
	avlTime = clock(); // start the avl timer
	inFile.get(c); // priming read
	while (!inFile.eof())
	{
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' ||
			c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' ||
			c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the
		// last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;
			std::string nextString = chari; //since the trees insert use std::string as an input parameter we must convert the c string to a std::string
			AVL_T->insert(nextString); // insert this word in the avl			
			memset(chari, 0, 50); // zero the word buffer
			iPtr = 0;
		}
		else if (!IsDelimiter) // if this isn’t a delimiter, keep going
		{
			chari[iPtr] = c;
			iPtr++;
		}
		else if (IsDelimiter && WasDelimiter)
		{
			// Do nothing -- two consecutive delimiters.
		}
		WasDelimiter = IsDelimiter; // for the NEXT iteration
		inFile.get(c);
	}
	inFile.close();
	avlTime = clock() - avlTime - parsingTime;	//the avl time the difference between the time at this line and the starting time minus the time to parse 
	std::cout << "avl time to complete: " << ((float)avlTime / CLOCKS_PER_SEC) << " seconds " << std::endl; // display the time to complete the file with avl tree
	AVL_T->reportHeight(); // report the avl tree's height
	AVL_T->reportTotalNumberOfNodesAndWords();	//report the avl trees total unique words and total words inserted
	AVL_T->tellFileSize();
	std::cout << "avl number of reads from disk: " << AVL_T->readCount << std::endl; // report the total number of reads to the disk that avl made
	std::cout << "avl number of writes to disk: " << AVL_T->writeCount<< std::endl; // report the total number of writes to the disk that avl made	
	inFile.open(filename, std::ios::binary);
	iPtr = 0;
	clock_t btreeTime;//clock variable to calculate the btree time to complete
	btreeTime = clock();//btree starting time	inFile.get(c); // priming read
	while (!inFile.eof())
	{
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' ||
			c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' ||
			c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the
		// last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;
			std::string nextString = chari;//since the trees insert use std::string as an input parameter we must convert the c string to a std::string
			btree->insert(nextString); // insert this word in the btree
			memset(chari, 0, 50); // zero the word buffer
			iPtr = 0;
		}
		else if (!IsDelimiter) // if this isn’t a delimiter, keep going
		{
			chari[iPtr] = c;
			iPtr++;
		}
		else if (IsDelimiter && WasDelimiter)
		{
			// Do nothing -- two consecutive delimiters.
		}
		WasDelimiter = IsDelimiter; // for the NEXT iteration
		inFile.get(c);
	}
	inFile.close();
	btreeTime = clock() - btreeTime - parsingTime; // calculate the time to run the file through btree. It should be the time at this line minus the btree starting time minus the parsing time
	std::cout << "btree time to complete: " << ((float)btreeTime / CLOCKS_PER_SEC) << " seconds " << std::endl;//report the time for btree to complete
	btree->reportHeight(); //report the height of the btree
	btree->reportTotalNumberOfNodesAndWords(); //report the total number of unique words and total words inserted into the btree
	btree->reportLoadingFactor();//report the btree loading factor size
	btree->tellBtreeFileSize();	
	std::cout << "btree number of reads from disk: " << btree->readCount << std::endl;//report the total disk reads of btree
	std::cout << "btree number of writes to disk: " << btree->writeCount << std::endl;//report the total disk writes of btree
	
	
}





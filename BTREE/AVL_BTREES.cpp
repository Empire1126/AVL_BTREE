

#include "pch.h"
#include "time.h"
#include "AVL.h"
#include "BTREE.h"


int main()
{
	char c;
	AVL* AVL_T= new AVL();
	BTREE * btree = new BTREE();
	char chari[50]; // assumes no word is longer than 49 characters
	memset(chari, 0, 50); // zero the word buffer
	int iPtr;
	bool IsDelimiter = false, WasDelimiter = false;

	std::ifstream inFile;
	inFile.open("Shakespeare.txt", std::ios::binary);
	if (inFile.fail())
	{
		std::cout << "Unable to open input file\n\n"
			<< "Program Exiting\n\nPress ENTER to exit\n";
		std::cin.get(c);
		exit(1);
	}
	iPtr = 0;
	clock_t parsingTime;
	parsingTime = clock();
	inFile.get(c); // priming read
	while (inFile.eof())
	{
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' ||
			c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' ||
			c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the
		// last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;
			std::string nextString = chari;						
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
	parsingTime = clock() - parsingTime;
	inFile.open("Shakespeare.txt", std::ios::binary);
	iPtr = 0;
	clock_t avlTime;
	avlTime = clock();
	inFile.get(c); // priming read
	while (inFile.eof())
	{
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' ||
			c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' ||
			c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the
		// last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;
			std::string nextString = chari;
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
	avlTime = clock() - avlTime - parsingTime;	
	AVL_T->reportHeight();
	AVL_T->reportTotalNumberOfNodesAndWords();
	std::cout << "avl time to complete: " << ((float)avlTime / CLOCKS_PER_SEC) << " seconds " << std::endl;
	inFile.open("Shakespeare.txt", std::ios::binary);
	iPtr = 0;
	clock_t btreeTime;
	btreeTime = clock();	inFile.get(c); // priming read
	while (inFile.eof())
	{
		IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' ||
			c == '!' || c == ';' || c == ':' || c == '(' || c == ')' || c == '?' ||
			c == '-');
		if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the
		// last character WASN’T, it’s the end of a word
		{
			WasDelimiter = true;
			std::string nextString = chari;
			btree->insert(nextString); // insert this word in the avl			
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
	btree->reportHeight();
	btree->reportTotalNumberOfNodesAndWords();
	btreeTime = clock() - btreeTime - parsingTime;
	std::cout << "avl time to complete: " << ((float)btreeTime/ CLOCKS_PER_SEC) << " seconds " << std::endl;
}





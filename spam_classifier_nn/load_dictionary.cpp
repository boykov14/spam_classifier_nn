#include <stdbool.h>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <fstream>

#include "load_dictionary.h"
#include "load_dictionary.h"


//constants for various ascii table values
#define comma  39
#define asciia 97
#define comval 26
#define arrlen 27

using namespace std;

trie* root;
unsigned int number = 0;

/**
* Returns true if word is in dictionary else false.
* @param the string to look for in the tree structure
*/
bool check(string word)
{
	// variables
	int length = word.length();
	int *x = new int[length];

	// pointer to root of the trie table
	trie* r = root;

	for (int i = 0; i < length; i++)
	{
		/* arranging array x with values for the position in which should be the pointers in each consecutive node
		in the trie for the given word */

		// if next character is an appostrophe
		if (word[i] == comma)
		{
			x[i] = comval;
		}
		else if (word[i] < 0 || !isalpha(word[i])) {
			delete[] x;
			return false;
		}
		else
		{
			x[i] = tolower(word[i]) - asciia;
		}

		//making sure that there are no invalid symbols
		if (x[i] > 26) {
			delete[] x;
			return false;
		}

		// checks if there is a pointer in the node at the indicated position in the array
		if (r->characters[x[i]] != NULL)
		{
			r = r->characters[x[i]];
		}
		else
		{
			delete[] x;
			return false;
		}

	}

	// checking if the boolean in in the node to which last pointer is pointing is true or false    
	if (r->isWord == true)
	{
		delete[] x;
		return true;
	}
	else
	{
		delete[] x;
		return false;
	}
}

/**
* Loads dictionary into memory.  Returns true if successful else false.
* @param pointer to file name of the dictionarry that will be loaded into the dictionary
* @return 0 or 1 depending on the success of the loading procedure
*/
bool loadDict(std::string filepath)
{
	root = new trie;

	// pointer to root of trie structure
	trie* r = root;

	// making sure all pointers in the root of the trie struct are set to null
	for (int j = 0; j< arrlen; j++)
	{
		r->characters[j] = NULL;
	}

	// opening dictionary file
	ifstream readFile;
	readFile.open(filepath);

	FILE* f = fopen(filepath.c_str(), "r");
	if (f == NULL)
	{
		printf("Could not open %s \n", filepath);
		exit(1);
	}

	if (readFile.fail()) {
		// fin.fail() returns true on failure

		cerr << "Error opening file:" << filepath << endl;
		exit(1);

	}

	// variables 
	number = 0;

	for (int ch = fgetc(f); ch != EOF; ch = fgetc(f))
	{
		if (ch == '\n')
		{
			number++;
			// set boolean in last node to true indicating the end of a word
			r->isWord = true;

			// reseting neccesary variables
			r = root;
		}
		//detect invalid characters (non alphanumeric or comma)
		else if (!isalnum(ch) && ch != comma) {

			//finish going through line until end of line reached so word is not added to tree structure
			while (!iscntrl(ch))
				ch = fgetc(f);

			//reset r without adding word to dictionary
			r = root;
		}
		else
		{
			//reset character value for use as position indicator
			if (ch == comma)
			{
				ch = comval;
			}
			else
			{
				ch = (int)(ch - asciia);
			}

			// if the pointer already points to another node then simply move to that next node
			if (r->characters[ch] != NULL)
			{
				r = r->characters[ch];
			}
			else // if(r->characters[ch] == NULL)
			{
				// if no pointer exists create a new node and make this pointer point to it
				r->characters[ch] = new trie;

				trie* loc = r->characters[ch];
				for (int j = 0; j< arrlen; j++)
				{
					loc->characters[j] = NULL;
				}
				r = r->characters[ch];
			}
		}
	}

	//close the file
	fclose(f);
	return true;
}

/**
* Returns number of words in dictionary if loaded else 0 if not yet loaded.
* @return integer representing the size of the tree structure
*/
unsigned int size(void)
{
	return number;
}

/**
* Unloads dictionary from memory.
* @Returns true if successful else false.
*/
bool unloadDictionary(void)
{
	// pointer to root for unload
	trie* r = root;

	for (int i = 0; i < arrlen; i++)
	{
		if (r->characters[i] != NULL)
		{
			unload(r->characters[i]);
		}
	}
	return true;
}

/**recursive function to unload tree data structure
* @param pointer to node in tree structure
* @return true if unloaded succesfull else false
*/
bool unload(trie* del)
{
	for (int i = 0; i < arrlen; i++)
	{
		if (del->characters[i] != NULL)
		{
			unload(del->characters[i]);
		}
	}

	delete del;

	return 0;
}
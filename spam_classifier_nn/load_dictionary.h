#ifndef LOAD_DICTIONARY_H
#define LOAD_DICTIONARY_H

#include <stdbool.h>
#include <string>
#include <iostream>

using namespace std;

//max length of a word
#define LENGTH 45

/**
* Returns true if word is in dictionary else false.
*/
bool check(string word);

/**
* Loads dictionary into memory.  Returns true if successful else false.
*/
bool loadDict(std::string filepath);

/**
* Returns number of words in dictionary if loaded else 0 if not yet loaded.
*/
unsigned int size(void);

/**
* Unloads dictionary from memory.  Returns true if successful else false.
*/
bool unloadDictionary(void);

typedef struct trie_
{
	bool isWord;
	struct trie_* characters[27];
}
trie;

extern trie* root;

// deletes elements in unload
bool unload(trie* del);

// word count for dictionary
extern unsigned int number;

#endif // DICTIONARY_H

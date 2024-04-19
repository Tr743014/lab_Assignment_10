#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode
{
    struct TrieNode* children[ALPHABET_SIZE];
    int count; // to store the count of occurrences
};

// Trie structure
struct Trie
{
    struct TrieNode* root;
};

// Function to create a new Trie node
struct TrieNode* createNode()
{
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (newNode)
    {
        newNode->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            newNode->children[i] = NULL;
    }
    return newNode;
}

// Inserts the word into the Trie
void insert(struct Trie* pTrie, char* word)
{
    struct TrieNode* current = pTrie->root;
    int index;
    for (int i = 0; i < strlen(word); i++)
    {
        index = word[i] - 'a';
        if (!current->children[index])
            current->children[index] = createNode();
        current = current->children[index];
    }
    current->count++; // Increment count for the last character of the word
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie* pTrie, char* word)
{
    struct TrieNode* current = pTrie->root;
    int index;
    for (int i = 0; i < strlen(word); i++)
    {
        index = word[i] - 'a';
        if (!current->children[index])
            return 0; // Word not found
        current = current->children[index];
    }
    return current->count;
}

// Deallocates memory for Trie nodes
void deallocateTrieUtil(struct TrieNode* node)
{
    if (node)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
            deallocateTrieUtil(node->children[i]);
        free(node);
    }
}

// Deallocates the Trie structure
struct Trie* deallocateTrie(struct Trie* pTrie)
{
    if (pTrie)
    {
        deallocateTrieUtil(pTrie->root);
        free(pTrie);
    }
    return NULL;
}

// Initializes a Trie structure
struct Trie* createTrie()
{
    struct Trie* trie = (struct Trie*)malloc(sizeof(struct Trie));
    if (trie)
    {
        trie->root = createNode();
    }
    return trie;
}

// Reads the dictionary file and returns the number of words
int readDictionary(char* filename, char** pInWords)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    int numWords = 0;
    char word[256];
    while (fscanf(file, "%s", word) != EOF)
    {
        pInWords[numWords] = _strdup(word);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char* inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    // Create and populate the Trie
    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Test the Trie with some words
    char* pWords[] = { "notaword", "ucf", "no", "note", "corg" };
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate the Trie
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}

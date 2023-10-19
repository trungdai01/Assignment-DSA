#ifndef CACHE_H
#define CACHE_H

#include "main.h"

class Node
{
public:
	Node* left;
	Node* right;
	Elem* val;

	Node(Elem* key)
	{
		left = nullptr;
		right = nullptr;
		val = key;
	}
	Node() :left(NULL), right(NULL), val(NULL) {}
};

class SearchEngine
{
public:
	Node* root;

	SearchEngine()
	{
		root = nullptr;
	}
	virtual ~SearchEngine() {}

	virtual Node* getMaxNode(Node* p) = 0;
	virtual Node* search(int idx) = 0;
	virtual void insert(Elem* key) = 0;

	virtual Node* deleteNode(Node* p, int addr) = 0;
	virtual void deleteNode(int addr) = 0;

	virtual void Preorder(Node* p) = 0;
	virtual void Inorder(Node* p) = 0;
	virtual void displayPreorder() = 0;
	virtual void displayInorder() = 0;

	virtual void clearTree(Node* pRoot) = 0;
};

class Array
{
public:
	Elem* prim;
	int count;
	int countTime;
	bool allow;

	Array(Elem* value, int count)
	{
		prim = value;
		this->count = count;
		countTime = 0;
		allow = true;
	}

	Array()
	{
		prim = nullptr;
		count = 1;
		countTime = 0;
		allow = true;
	}
};

class ReplacementPolicy
{
public:
	int current_size;	//number of elements entered cache
	Array** arr;        //Array as a heap contains Elem and count and countTime
	Elem** mainMem;		//mainMem as main memory
	Elem** HTable;		//HTable as hash table

	ReplacementPolicy()
	{
		current_size = 0;
		arr = new Array * [MAXSIZE];
		mainMem = new Elem * [MAXSIZE];
		HTable = new Elem * [MAXSIZE];
		for (int i = 0; i < MAXSIZE; i++)
		{
			arr[i] = 0;
			mainMem[i] = 0;
			HTable[i] = 0;
		}
	}

	virtual ~ReplacementPolicy()
	{
		delete[] arr;
		delete[] mainMem;
		delete[] HTable;
	}

	virtual int searchHeap(int addr) = 0;				//Searching position in heap
	virtual void insert(Array* key) = 0;				//Insertion in heap
	virtual void reheapUp(int index) = 0;				//ReheapUp

	virtual int getRPpos() = 0;							//Retrieve position in heap to be replaced
	virtual void deleteElem(int index) = 0;				//Delete element from a given index
	virtual void display() = 0;							//Print out heap

	virtual int searchMain(int addr) = 0;				//Searching position in main memory
	virtual void modifyMain(int index, Elem* cell) = 0; //Adjust element in main memory
	virtual void displayMain() = 0;						//Print out main memory

	virtual int hash(int key) = 0;						//Basic hashing code
	virtual int probing(int key) = 0;					//Linear probing collision resolution
	virtual void insertTable(Elem* cell) = 0;			//Insertion in hash table
	virtual void displayTable() = 0;					//Print out hash table
};

class MFU : public ReplacementPolicy
{
public:
	MFU() :ReplacementPolicy() {}
	~MFU() {}

	int searchHeap(int addr);
	void insert(Array* key);
	void reheapUp(int index);

	int getRPpos();
	void deleteElem(int index);
	void display();

	int searchMain(int addr);
	void modifyMain(int index, Elem* cell);
	void displayMain();

	int hash(int key);
	int probing(int key);
	void insertTable(Elem* cell);
	void displayTable();
};

class LFU : public ReplacementPolicy
{
public:
	LFU() :ReplacementPolicy() {}
	~LFU() {}

	int searchHeap(int addr);
	void insert(Array* key);
	void reheapUp(int index);

	int getRPpos();
	void deleteElem(int index);
	void display();

	int searchMain(int addr);
	void modifyMain(int index, Elem* cell);
	void displayMain();

	int hash(int key);
	int probing(int key);
	void insertTable(Elem* cell);
	void displayTable();
};

class MFRU : public ReplacementPolicy
{
public:
	MFRU() :ReplacementPolicy() {}
	~MFRU() {}

	int searchHeap(int addr);
	void insert(Array* key);
	void reheapUp(int index);

	int getRPpos();
	void deleteElem(int index);
	void display();

	int searchMain(int addr);
	void modifyMain(int index, Elem* cell);
	void displayMain();

	int hash(int key);
	int probing(int key);
	void insertTable(Elem* cell);
	void displayTable();
};

class LFRU : public ReplacementPolicy
{
public:
	LFRU() :ReplacementPolicy() {}
	~LFRU() {}

	int searchHeap(int addr);
	void insert(Array* key);
	void reheapUp(int index);

	int getRPpos();
	void deleteElem(int index);
	void display();

	int searchMain(int addr);
	void modifyMain(int index, Elem* cell);
	void displayMain();

	int hash(int key);
	int probing(int key);
	void insertTable(Elem* cell);
	void displayTable();
};

class BST : public SearchEngine
{
public:
	BST() :SearchEngine() {}
	~BST()
	{
		clearTree(root);
	}

	Node* getMaxNode(Node* p);
	Node* search(int idx);
	void insert(Elem* key);

	Node* deleteNode(Node* p, int addr);
	void deleteNode(int addr);

	void Preorder(Node* p);
	void Inorder(Node* p);
	void displayPreorder();
	void displayInorder();

	void clearTree(Node* pRoot);
};

#endif
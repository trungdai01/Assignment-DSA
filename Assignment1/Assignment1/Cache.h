#ifndef CACHE_H
#define CACHE_H

#include "main.h"
struct Node {
    Node* left;
    Node* right;
    Elem* val;
};

class BSTree {
    Node* root;
public:
    BSTree()
    {
        root = nullptr;
    }
    ~BSTree()
    {
        root = clrTree(root);
    }
    Node* clrTree(Node* tNode);
    Node* insertNode(Node* p, int key, Elem* val);
    Node* minNode(Node* p);
    Node* delNode(Node* p, int key, Elem* val);
    void inOrderBST(Node* p);
    void preOrderBST(Node* p);
    void printinOrderBST();
    void printpreOrderBST();
    void insertNode(int key, Elem* val);
    void delNode(int key, Elem* val);
};

class Cache {
    Elem** arr;
    int p;
    BSTree b;
public:
    Cache(int s) {
        arr = new Elem * [s];
        p = 0;
    }
    ~Cache() {
        delete[] arr;
    }
    Data* read(int addr);
    Elem* put(int addr, Data* cont);
    Elem* write(int addr, Data* cont);
    void print();
    void preOrder();
    void inOrder();
};
#endif
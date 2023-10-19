#include "Cache.h"

Data* Cache::read(int addr) {
    for (int i = 0; i < p; i++)
    {
        if (arr[i]->addr == addr)
        {
            return arr[i]->data;
        }
    }
    return NULL;
}
Elem* Cache::put(int addr, Data* cont) {
    Elem* cell = new Elem(addr, cont, true);
    Elem* tmp = nullptr;
    if (p != MAXSIZE)
    {
        arr[p] = cell;
        b.insertNode(addr, arr[p]);
        p++;
        return NULL;
    }
    else
    {
        if (addr % 2 == 0)
        {
            tmp = arr[0];
            for (int i = 0; i < p - 1; i++)
            {
                arr[i] = arr[i + 1];
            }
            b.delNode(tmp->addr, tmp);
            arr[p - 1] = cell;
            b.insertNode(addr, cell);
        }
        else
        {
            tmp = arr[p - 1];
            b.delNode(tmp->addr, tmp);
            arr[p - 1] = cell;
            b.insertNode(addr, cell);
        }
        return tmp;
    }
}
Elem* Cache::write(int addr, Data* cont) {
    bool found = false;
    Elem* cell = new Elem(addr, cont, false);
    Elem* tmp = nullptr;
    for (int i = 0; i < p; i++)
    {
        if (arr[i]->addr == addr)
        {
            found = true;
            arr[i]->data = cont;
            arr[i]->sync = false;
            break;
        }
    }
    if (!found)
    {
        if (p != MAXSIZE)
        {
            arr[p] = cell;
            b.insertNode(addr, cell);
            p++;
        }
        else if(p >= MAXSIZE)
        {
            if (addr % 2 == 0)
            {
                tmp = arr[0];
                for (int i = 0; i < p - 1; i++)
                {
                    arr[i] = arr[i + 1];
                }
                b.delNode(tmp->addr, tmp);
                arr[p - 1] = cell;
                b.insertNode(addr, cell);
            }
            else if (addr % 2 != 0)
            {
                tmp = arr[p - 1];
                b.delNode(tmp->addr, tmp);
                arr[p - 1] = cell;
                b.insertNode(addr, cell);
            }
        }
    }
    return tmp;
}
void Cache::print() {
    for (int i = p - 1; i >= 0; i--)
        arr[i]->print();
}
void Cache::preOrder() {
    b.printpreOrderBST();
}
void Cache::inOrder() {
    b.printinOrderBST();
}
Node* BSTree::clrTree(Node* tNode)
{
    if (tNode == nullptr)
        return nullptr;
    clrTree(tNode->left);
    clrTree(tNode->right);
    delete tNode;
    return nullptr;
}
Node* BSTree::insertNode(Node* p, int key, Elem* val)
{
    if (p == nullptr)
    {
        Node* tmp = new Node;
        tmp->left = nullptr;
        tmp->right = nullptr;
        tmp->val = val;
        return tmp;
    }
    else
    {
        if (key < p->val->addr)
        {
            p->left = insertNode(p->left, key, val);
        }
        else
        {
            p->right = insertNode(p->right, key, val);
        }
        return p;
    }
}
Node* BSTree::minNode(Node* p)
{
    Node* head = p;
    while (head && head->left != NULL)
    {
        head = head->left;
    }
    return head;
}
Node* BSTree::delNode(Node* p, int key, Elem* val)
{
    if (p == nullptr)
        return p;
    if (key < p->val->addr)
        p->left = delNode(p->left, key, val);
    else if (key > p->val->addr)
        p->right = delNode(p->right, key, val);
    else
    {
        if (p->left == nullptr && p->right == nullptr)
            return nullptr;
        else if (p->left == nullptr)
        {
            Node* tmp = p->right;
            free(p);
            return tmp;
        }
        else if (p->right == nullptr)
        {
            Node* tmp = p->left;
            free(p);
            return tmp;
        }
        Node* tmp = minNode(p->right);
        p->val = tmp->val;
        p->right = delNode(p->right, tmp->val->addr, tmp->val);
    }
    return p;
}
void BSTree::inOrderBST(Node* p)
{
    if (p != nullptr)
    {
        inOrderBST(p->left);
        p->val->print();
        inOrderBST(p->right);
    }
}
void BSTree::preOrderBST(Node* p)
{
    if (p != nullptr)
    {
        p->val->print();
        preOrderBST(p->left);
        preOrderBST(p->right);
    }
}
void BSTree::printinOrderBST()
{
    inOrderBST(root);
}
void BSTree::printpreOrderBST()
{
    preOrderBST(root);
}
void BSTree::insertNode(int key, Elem* val)
{
    root = insertNode(root, key, val);
}
void BSTree::delNode(int key, Elem* val)
{
    root = delNode(root, key, val);
}


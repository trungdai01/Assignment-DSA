#include "main.h"

Data *Cache::read(int addr)
{
    Node *find = *tree.findPtr(addr);
    return find ? find->value->data : nullptr;
}
Elem *Cache::put(int addr, Data *cont)
{
    if (p != MAXSIZE)
    {
        arr[p] = new Elem(addr, cont, true);
        tree.insert(addr, arr[p++]);
        return nullptr;
    }

    Elem **dst = (addr % 2 == 0) ? &arr[0] : &arr[p - 1];
    Elem *ret = *dst;
    tree.erase(ret->addr);
    *dst = new Elem(addr, cont, true);
    tree.insert(addr, *dst);

    return ret;
}
Elem *Cache::write(int addr, Data *cont)
{
    Elem *ret = NULL;
    Node *find = *tree.findPtr(addr);
    if (find != nullptr)
    {
        find->value->data = cont;
        find->value->sync = false;
    }
    else
    {
        put(addr, cont);
        (*tree.findPtr(addr))->value->sync = false;
    }
    return ret;
}
void Cache::print()
{
    for (int i = p - 1; i >= 0; --i)
        arr[i]->print();
}
void Cache::preOrder()
{
    tree.printPreorder();
}
void Cache::inOrder()
{
    tree.printInorder();
}
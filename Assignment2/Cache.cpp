#include "Cache.h"
Cache::Cache(SearchEngine* s, ReplacementPolicy* r) :rp(r), s_engine(s) {}
Cache::~Cache()
{
	delete rp;
	delete s_engine;
}

Data* Cache::read(int addr)
{
	Node* find = s_engine->search(addr);
	if (find != NULL)
	{
		int getCount = 1;
		int indexHeap = rp->searchHeap(addr);
		getCount += rp->arr[indexHeap]->count;

		Elem* cell = find->val;
		Array* newCell = new Array(cell, getCount);

		rp->deleteElem(indexHeap);
		rp->insert(newCell);

		return cell->data;
	}
	return NULL;
}

Elem* Cache::put(int addr, Data* cont)
{
	Elem* cell = new Elem(addr, cont, true);
	Array* newCell = new Array(cell, 1);
	Elem* res = nullptr;
	if (rp->current_size != MAXSIZE)
	{
		rp->mainMem[rp->current_size] = cell;
		s_engine->insert(cell);
		//rp->insertTable(cell);
		//cout << rp->searchTable(cell->addr) << endl;
		rp->insert(newCell);
	}
	else
	{
		int indexHeap = rp->getRPpos();
		int address = rp->arr[indexHeap]->prim->addr;

		int indexMain = rp->searchMain(address);
		rp->modifyMain(indexMain, cell);

		/*rp->removeTable(address);
		rp->insertTable(cell);*/

		res = rp->arr[indexHeap]->prim;

		s_engine->deleteNode(address);
		s_engine->insert(cell);

		rp->deleteElem(indexHeap);
		rp->insert(newCell);
	}
	return res;
}

Elem* Cache::write(int addr, Data* cont)
{
	Node* find = s_engine->search(addr);
	Elem* cell = new Elem(addr, cont, false);
	Array* newCell = new Array(cell, 1);
	Elem* result = NULL;
	if (find != NULL)
	{
		int indexMain = rp->searchMain(addr);
		rp->mainMem[indexMain]->data = cont;
		rp->mainMem[indexMain]->sync = false;

		/*int indexTable = rp->searchTable(addr);
		rp->HTable[indexTable]->data = cont;
		rp->HTable[indexTable]->sync = false;*/

		int getCount = 1;
		int indexHeap = rp->searchHeap(addr);
		getCount += rp->arr[indexHeap]->count;

		newCell->count = getCount;

		s_engine->deleteNode(addr);
		s_engine->insert(cell);

		rp->deleteElem(indexHeap);
		rp->insert(newCell);
	}
	else
	{
		if (rp->current_size != MAXSIZE)
		{
			rp->mainMem[rp->current_size] = cell;
			s_engine->insert(cell);
			rp->insertTable(cell);
			rp->insert(newCell);
		}
		else
		{
			int indexHeap = rp->getRPpos();
			//cout << "Index from heap: " << indexHeap << endl;
			int address = rp->arr[indexHeap]->prim->addr;

			int indexMain = rp->searchMain(address);
			//cout << "Index from main: " << indexMain << endl;
			rp->modifyMain(indexMain, cell);

			/*rp->removeTable(address);
			rp->insertTable(cell);*/

			result = rp->arr[indexHeap]->prim;

			s_engine->deleteNode(address);
			s_engine->insert(cell);

			rp->deleteElem(indexHeap);
			rp->insert(newCell);
		}
	}
	return result;
}

void Cache::printRP()
{
	rp->display();
}

void Cache::printSE()
{
	s_engine->displayInorder();
	s_engine->displayPreorder();
}

void Cache::printLP()
{
	rp->displayMain();
	rp->displayTable();
}


///////////////////////////////////////////////////////////////////////////////
//////////////////////// BINARY SEARCH TREE IMPLEMENTATION ////////////////////
///////////////////////////////////////////////////////////////////////////////


Node* BST::getMaxNode(Node* p)
{
	while (p && p->right != NULL)
	{
		p = p->right;
	}
	return p;
}

Node* BST::search(int idx)
{
	Node* p = root;
	while (p)
	{
		if (p->val->addr == idx)
		{
			return p;
		}
		else if (idx < p->val->addr)
		{
			p = p->left;
		}
		else if (idx > p->val->addr)
		{
			p = p->right;
		}
	}
	return NULL;
}

void BST::insert(Elem* key)
{
	Node* head = root;
	Node* p = nullptr;
	if (root == nullptr)
	{
		root = new Node(key);
		return;
	}

	while (head != nullptr)
	{
		p = head;
		if (key->addr < head->val->addr)
		{
			head = head->left;
		}
		else if (key->addr > head->val->addr)
		{
			head = head->right;
		}
		else return;
	}

	if (key->addr < p->val->addr)
	{
		p->left = new Node(key);
	}
	else p->right = new Node(key);
}

Node* BST::deleteNode(Node* p, int addr)
{
	if (p == nullptr)
	{
		return nullptr;
	}
	if (p->right == NULL && p->left == NULL)
	{
		if (p == root)
		{
			root = nullptr;
		}
		delete p;
		return NULL;
	}
	else if (addr < p->val->addr)
	{
		p->left = deleteNode(p->left, addr);
	}
	else if (addr > p->val->addr)
	{
		p->right = deleteNode(p->right, addr);
	}
	else
	{
		Node* tmp = nullptr;
		if (p->right == nullptr)
		{
			tmp = p->left;
			delete p;
			return tmp;
		}
		else if (p->left == nullptr)
		{
			tmp = p->right;
			delete p;
			return tmp;
		}
		tmp = getMaxNode(p->left);
		p->val = tmp->val;
		p->left = deleteNode(p->left, tmp->val->addr);
	}
	return p;
}

void BST::deleteNode(int addr)
{
	root = deleteNode(root, addr);
}

void BST::Preorder(Node* p)
{
	if (p)
	{
		p->val->print();
		Preorder(p->left);
		Preorder(p->right);
	}
}

void BST::displayPreorder()
{
	cout << "Print BST in preorder:\n";
	Preorder(root);
}

void BST::Inorder(Node* p)
{
	if (p)
	{
		Inorder(p->left);
		p->val->print();
		Inorder(p->right);
	}
}

void BST::displayInorder()
{
	cout << "Print BST in inorder:\n";
	Inorder(root);
}

void BST::clearTree(Node* pRoot)
{
	if (pRoot == nullptr)
		return;
	clearTree(pRoot->left);
	clearTree(pRoot->right);
	delete pRoot;
}


/////////////////////////////////////////////////////////////////////////////////
//////////////////////// MOST FREQUENTLY USED IMPLEMENTATION ////////////////////
/////////////////////////////////////////////////////////////////////////////////


int MFU::searchHeap(int addr)
{
	for (int i = 0; i < current_size; i++)
	{
		if (arr[i]->prim->addr == addr)
		{
			return i;
		}
	}
	return -1;
}

void MFU::insert(Array* key)
{
	arr[current_size] = key;
	current_size += 1;
	reheapUp(current_size - 1);
}

void MFU::deleteElem(int index)
{
	arr[index] = arr[current_size - 1];
	current_size -= 1;
	int i = index; int j = 2 * i + 1;
	while (j < current_size)
	{
		if (arr[j + 1]->count >= arr[j]->count && j + 1 < current_size)
		{
			j++;
		}
		if (arr[i]->count <= arr[j]->count)
		{
			swap(arr[i], arr[j]);
			i = j;
			j = 2 * i + 1;
		}
		else break;
	}
}

void MFU::reheapUp(int index)
{
	if (index > 0 && index < MAXSIZE)
	{
		int parent = (index - 1) / 2;
		if (arr[index]->count > arr[parent]->count)
		{
			swap(arr[index], arr[parent]);
			reheapUp(parent);
		}
	}
}

int MFU::getRPpos()
{
	return 0;
}

void MFU::display()
{
	for (int i = current_size - 1; i >= 0; i--)
	{
		arr[i]->prim->print();
	}
}

int MFU::searchMain(int addr)
{
	for (int i = 0; i < current_size; i++)
	{
		if (mainMem[i]->addr == addr)
			return i;
	}
	return -1;
}

void MFU::modifyMain(int index, Elem* cell)
{
	if (index < 0 || index >= MAXSIZE) return;
	mainMem[index] = cell;
}

void MFU::displayMain()
{
	cout << "Prime memory\n";
	for (int i = 0; i < current_size; i++)
	{
		mainMem[i]->print();
	}
}

int MFU::hash(int key)
{
	return key % MAXSIZE;
}

int MFU::probing(int key)
{
	int index = hash(key);
	int i = 0;
	while (HTable[(index + i) % MAXSIZE] != 0)
	{
		i++;
	}
	return (index + i) % MAXSIZE;
}

void MFU::insertTable(Elem* cell)
{
	int index = hash(cell->addr);
	if (HTable[index] != 0)
	{
		index = probing(cell->addr);
	}
	HTable[index] = cell;
}

void MFU::displayTable()
{
	cout << "Hash table memory\n";
	for (int i = 0; i < MAXSIZE; i++)
	{
		insertTable(mainMem[i]);
	}
	for (int i = 0; i < MAXSIZE; i++)
	{
		if (HTable[i] == 0)
		{
			cout << endl;
		}
		else HTable[i]->print();
	}
}


//////////////////////////////////////////////////////////////////////////////////
//////////////////////// LEAST FREQUENTLY USED IMPLEMENTATION ////////////////////
//////////////////////////////////////////////////////////////////////////////////


int LFU::searchHeap(int addr)
{
	for (int i = 0; i < current_size; i++)
	{
		if (arr[i]->prim->addr == addr)
		{
			return i;
		}
	}
	return -1;
}

void LFU::insert(Array* key)
{
	arr[current_size] = key;
	current_size += 1;
	reheapUp(current_size - 1);
}

void LFU::deleteElem(int index)
{
	arr[index] = arr[current_size - 1];
	current_size -= 1;
	int i = index; int j = 2 * i + 1;
	while (j < current_size)
	{
		if (arr[j + 1]->count < arr[j]->count && j + 1 < current_size)
		{
			j++;
		}
		if (arr[i]->count >= arr[j]->count)
		{
			swap(arr[i], arr[j]);
			i = j;
			j = 2 * i + 1;
		}
		else break;
	}
}

void LFU::reheapUp(int index)
{
	if (index > 0 && index < MAXSIZE)
	{
		int parent = (index - 1) / 2;
		if (arr[index]->count < arr[parent]->count)
		{
			swap(arr[index], arr[parent]);
			reheapUp(parent);
		}
	}
}

int LFU::getRPpos()
{
	return 0;
}

void LFU::display()
{
	for (int i = 0; i < current_size; i++)
	{
		arr[i]->prim->print();
	}
}

int LFU::searchMain(int addr)
{
	for (int i = 0; i < current_size; i++)
	{
		if (mainMem[i]->addr == addr)
			return i;
	}
	return -1;
}

void LFU::modifyMain(int index, Elem* cell)
{
	if (index < 0 || index >= MAXSIZE) return;
	mainMem[index] = cell;
}

void LFU::displayMain()
{
	cout << "Prime memory\n";
	for (int i = 0; i < current_size; i++)
	{
		mainMem[i]->print();
	}
}

int LFU::hash(int key)
{
	return key % MAXSIZE;
}

int LFU::probing(int key)
{
	int index = hash(key);
	int i = 0;
	while (HTable[(index + i) % MAXSIZE] != 0)  /*&& HTable[(index + i) % MAXSIZE]->addr != -1*/
	{
		i++;
	}
	return (index + i) % MAXSIZE;
}

void LFU::insertTable(Elem* cell)
{
	int index = hash(cell->addr);
	if (HTable[index] != 0) /*&& HTable[index]->addr != -1*/
	{
		index = probing(cell->addr);
	}
	HTable[index] = cell;
}

void LFU::displayTable()
{
	cout << "Hash table memory\n";
	for (int i = 0; i < MAXSIZE; i++)
	{
		insertTable(mainMem[i]);
	}
	for (int i = 0; i < MAXSIZE; i++)
	{
		if (HTable[i] == 0)
		{
			cout << endl;
		}
		else HTable[i]->print();
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////// MOST FREQUENTLY RECENTLY USED IMPLEMENTATION ////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


int MFRU::searchHeap(int addr)
{
	for (int i = 0; i < current_size; i++)
	{
		if (arr[i]->prim->addr == addr)
		{
			return i;
		}
	}
	return -1;
}

void MFRU::insert(Array* key)
{
	if (current_size > 0 && current_size <= MAXSIZE)
	{
		for (int i = 0; i < current_size; i++)
		{
			arr[i]->countTime += 1;
		}
	}
	arr[current_size] = key;
	current_size += 1;
	reheapUp(current_size - 1);
}

void MFRU::deleteElem(int index)
{
	arr[index] = arr[current_size - 1];
	current_size -= 1;
	int i = index; int j = 2 * i + 1;
	while (j < current_size)
	{
		if (arr[j + 1]->count >= arr[j]->count && j + 1 < current_size)
		{
			j++;
		}
		if (arr[i]->count <= arr[j]->count)
		{
			swap(arr[i], arr[j]);
			i = j;
			j = 2 * i + 1;
		}
		else break;
	}
}

void MFRU::reheapUp(int index)
{
	if (index > 0 && index < MAXSIZE)
	{
		int parent = (index - 1) / 2;
		if (arr[index]->count > arr[parent]->count)
		{
			swap(arr[index], arr[parent]);
			reheapUp(parent);
		}
	}
}

int MFRU::getRPpos()
{
	int mostCount = arr[0]->count;
	int time = arr[0]->countTime;
	int found = 0;
	for (int i = 0; i < current_size; i++)
	{
		if (arr[i]->count > mostCount)
		{
			time = arr[i]->countTime;
			mostCount = arr[i]->countTime;
			found = i;
		}
		else if (arr[i]->count == mostCount)
		{
			if (arr[i]->countTime < time)
			{
				time = arr[i]->countTime;
				found = i;
			}
		}
	}
	return found;
}

void MFRU::display()
{
	int j, k;
	for (int i = 0; i < current_size - 1; i++)
	{
		for (j = k = i; j < current_size; j++)
		{
			if (arr[j]->count == arr[k]->count)
			{
				if (arr[j]->countTime < arr[k]->countTime)
				{
					k = j;
				}
			}
			if (arr[j]->count > arr[k]->count)
			{
				k = j;
			}
		}
		swap(arr[i], arr[k]);
	}
	for (int i = 0; i < current_size; i++)
	{
		arr[i]->prim->print();
		//cout << "Count: " << arr[i]->count << " Time: " << arr[i]->countTime << endl;
	}
}

int MFRU::searchMain(int addr)
{
	for (int i = 0; i < current_size; i++)
	{
		if (mainMem[i]->addr == addr)
			return i;
	}
	return -1;
}

void MFRU::modifyMain(int index, Elem* cell)
{
	if (index < 0 || index >= MAXSIZE) return;
	mainMem[index] = cell;
}

void MFRU::displayMain()
{
	cout << "Prime memory\n";
	for (int i = 0; i < current_size; i++)
	{
		mainMem[i]->print();
	}
}

int MFRU::hash(int key)
{
	return key % MAXSIZE;
}

int MFRU::probing(int key)
{
	int index = hash(key);
	int i = 0;
	while (HTable[(index + i) % MAXSIZE] != 0)
	{
		i++;
	}
	return (index + i) % MAXSIZE;
}

void MFRU::insertTable(Elem* cell)
{
	int index = hash(cell->addr);
	if (HTable[index] != 0)
	{
		index = probing(cell->addr);
	}
	HTable[index] = cell;
}

void MFRU::displayTable()
{
	cout << "Hash table memory\n";
	for (int i = 0; i < MAXSIZE; i++)
	{
		insertTable(mainMem[i]);
	}
	for (int i = 0; i < MAXSIZE; i++)
	{
		if (HTable[i] == 0)
		{
			cout << endl;
		}
		else HTable[i]->print();
	}
}


///////////////////////////////////////////////////////////////////////////////////////////
//////////////////////// LEAST FREQUENTLY RECENTLY USED IMPLEMENTATION ////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


int LFRU::searchHeap(int addr)
{
	for (int i = 0; i < current_size; i++)
	{
		if (arr[i]->prim->addr == addr)
		{
			return i;
		}
	}
	return -1;
}

void LFRU::insert(Array* key)
{
	if (current_size > 0 && current_size <= MAXSIZE)
	{
		for (int i = 0; i < current_size; i++)
		{
			arr[i]->countTime += 1;
		}
	}
	arr[current_size] = key;
	current_size += 1;
	reheapUp(current_size - 1);
}

void LFRU::deleteElem(int index)
{
	arr[index] = arr[current_size - 1];
	current_size -= 1;
	int i = index; int j = 2 * i + 1;
	while (j < current_size)
	{
		if (arr[j + 1]->count < arr[j]->count && j + 1 < current_size)
		{
			j++;
		}
		if (arr[i]->count >= arr[j]->count)
		{
			swap(arr[i], arr[j]);
			i = j;
			j = 2 * i + 1;
		}
		else break;
	}
}

void LFRU::reheapUp(int index)
{
	if (index > 0 && index < MAXSIZE)
	{
		int parent = (index - 1) / 2;
		if (arr[index]->count < arr[parent]->count)
		{
			swap(arr[index], arr[parent]);
			reheapUp(parent);
		}
	}
}

int LFRU::getRPpos()
{
	int leastCount = arr[0]->count;
	int time = arr[0]->countTime;
	int found = 0;
	for (int i = 0; i < current_size; i++)
	{
		if (arr[i]->count < leastCount)
		{
			time = arr[i]->countTime;
			leastCount = arr[i]->countTime;
			found = i;
		}
		else if (arr[i]->count == leastCount)
		{
			if (arr[i]->countTime > time)
			{
				time = arr[i]->countTime;
				found = i;
			}
		}
	}
	return found;
}

void LFRU::display()
{
	int j, k;
	for (int i = 0; i < current_size - 1; i++)
	{
		for (j = k = i; j < current_size; j++)
		{
			if (arr[j]->count == arr[k]->count)
			{
				if (arr[j]->countTime > arr[k]->countTime)
				{
					k = j;
				}
			}
			if (arr[j]->count < arr[k]->count)
			{
				k = j;
			}
		}
		swap(arr[i], arr[k]);
	}
	for (int i = 0; i < current_size; i++)
	{
		arr[i]->prim->print();
		//cout << "Count: " << arr[i]->count << " Time: " << arr[i]->countTime << endl;
	}
}

int LFRU::searchMain(int addr)
{
	for (int i = 0; i < current_size; i++)
	{
		if (mainMem[i]->addr == addr)
			return i;
	}
	return -1;
}

void LFRU::modifyMain(int index, Elem* cell)
{
	if (index < 0 || index >= MAXSIZE) return;
	mainMem[index] = cell;
}

void LFRU::displayMain()
{
	cout << "Prime memory\n";
	for (int i = 0; i < current_size; i++)
	{
		mainMem[i]->print();
	}
}

int LFRU::hash(int key)
{
	return key % MAXSIZE;
}

int LFRU::probing(int key)
{
	int index = hash(key);
	int i = 0;
	while (HTable[(index + i) % MAXSIZE] != 0) 
	{
		i++;
	}
	return (index + i) % MAXSIZE;
}

void LFRU::insertTable(Elem* cell)
{
	int index = hash(cell->addr);
	if (HTable[index] != 0)
	{
		index = probing(cell->addr);
	}
	HTable[index] = cell;
}

void LFRU::displayTable()
{
	cout << "Hash table memory\n";
	for (int i = 0; i < MAXSIZE; i++)
	{
		insertTable(mainMem[i]);
	}
	for (int i = 0; i < MAXSIZE; i++)
	{
		if (HTable[i] == 0)
		{
			cout << endl;
		}
		else HTable[i]->print();
	}
}
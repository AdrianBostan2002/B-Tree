#include <iostream>
#include <vector>
#include <array>
#include <queue>

template <typename T>
class B_Tree
{
	class Node
	{
	public:
		std::vector<T>keys;
		int ksize;
		bool isLeaf;
		std::vector<Node*>legaturi;
	};

	Node* root;
	int t;
	int height;

	Node* alocare_nod_nou()
	{
		Node* nod_nou = new Node;
		nod_nou->isLeaf = false;
		nod_nou->ksize = 0;
		nod_nou->keys.resize(2 * t - 1);
		nod_nou->legaturi.resize(2 * t, nullptr);
		return nod_nou;
	}

	template<typename T>
	int cautare_binara_pozitie(Node* nod, T valoare)
	{
		if (nod->ksize != 0)
		{
			if (nod->keys[nod->ksize - 1] <= valoare)
			{
				if (nod->keys[nod->ksize - 1] == valoare)
				{
					return nod->ksize - 1;
				}
				return nod->ksize;
			}
			if (nod->keys[0] > valoare)
			{
				return 0;
			}
			int st = 0, dr = nod->ksize - 1, m;
			while (st <= dr && (st + dr) / 2 + 1 <= nod->ksize - 1)
			{
				m = (st + dr) / 2;
				if (nod->keys[m] <= valoare && nod->keys[m + 1] > valoare)
				{
					if (nod->keys[m] == valoare)
					{
						return m;
					}
					return m + 1;
				}
				if (nod->keys[m] < valoare)
				{
					st++;
				}
				else
				{
					dr--;
				}
			}
		}
		return -1;
	}

public:


	B_Tree(int t = 2)
	{
		this->t = t;
		root = alocare_nod_nou();
		root->isLeaf = true;

	}

	Node* search(Node* nod, T key)
	{
		int i;
		if (nod != nullptr)
		{
			i = cautare_binara_pozitie(nod, key);
			if (i == -1)
			{
				return nullptr;
			}
			else if (nod->keys[i] == key)
			{
				return nod;
			}
			nod = nod->legaturi[i];
			search(nod, key);
		}
		else return nullptr;
	}

	void split(Node* nod, int i)
	{
		Node* y = nod->legaturi[i];
		nod->ksize++;
		for (int k = nod->ksize - 1; k > i; k--)
		{
			nod->keys[k] = nod->keys[k - 1];
		}

		nod->keys[i] = y->keys[t - 1];

		for (int k = nod->ksize; k > i; k--)
		{
			nod->legaturi[k] = nod->legaturi[k - 1];
		}

		Node* z = alocare_nod_nou();
		z->ksize = t - 1;
		for (int k = 0; k < t - 1; k++)
		{
			z->keys[k] = y->keys[k + t];
		}
		if (y->isLeaf == false)
		{
			for (int k = 0; k < t; k++)
			{
				z->legaturi[k] = y->legaturi[k + t];
				y->legaturi[k + t] = nullptr;
			}
		}

		y->ksize = t - 1;
		z->isLeaf = y->isLeaf;
		nod->legaturi[i + 1] = z;
	}

	template<typename T>
	void insertInNodNonfull(Node* nod, T key)
	{
		int i = nod->ksize;
		if (nod->isLeaf == true)
		{
			while (i > 0 && key < nod->keys[i - 1])
			{
				nod->keys[i] = nod->keys[i - 1];
				i--;
			}
			nod->keys[i] = key;
			nod->ksize++;
		}
		else
		{
			int i = cautare_binara_pozitie(nod, key);
			//i++;
			if (nod->legaturi[i]->ksize == 2 * t - 1)
			{
				split(nod, i);
				if (key > nod->keys[i])
				{
					i++;
				}
			}
			insertInNodNonfull(nod->legaturi[i], key);
		}
	}

	void insert(T key)
	{
		if (root->ksize == 2 * t - 1)
		{
			Node* nod_nou = alocare_nod_nou();
			nod_nou->legaturi[0] = root;
			root = nod_nou;
			split(root, 0);
		}
		insertInNodNonfull(root, key);

	}

	void rot_dr(Node* nod, int i)
	{
		T k = nod->keys[i];
		Node* y = nod->legaturi[i];
		Node* z = nod->legaturi[i + 1];
		z->ksize++;
		for (int j = z->ksize - 1; j > 0; j--)
		{
			z->keys[j] = z->keys[j - 1];
			z->legaturi[j + 1] = z->legaturi[j];
		}
		z->legaturi[1] = z->legaturi[0];
		z->keys[0] = k;
		z->legaturi[0] = y->legaturi[y->ksize];
		y->ksize--;
		nod->keys[i] = y->keys[y->ksize];
	}

	void rot_st(Node* nod, int i)
	{
		T k = nod->keys[i];
		Node* y = nod->legaturi[i];
		Node* z = nod->legaturi[i+1];
		y->ksize++;
		nod->keys[i] = z->keys[0];
		y->legaturi[y->ksize] = z->legaturi[0];
		y->keys[y->ksize - 1] = k;
		for (int j = 0; j < z->ksize-1; j++)
		{
			z->keys[j] = z->keys[j+1];
			z->legaturi[j] = z->legaturi[j+1];
		}
		z->legaturi[z->ksize - 1] = z->legaturi[z->ksize];
		z->ksize--;
	}

	Node* fuziune(Node* nod, int i)
	{
		Node* y = nod->legaturi[i];
		Node* z = nod->legaturi[i + 1];
		if (nod == root && root->ksize==1)
		{
			root = y;
		}
		if (y->ksize == t - 1 && z->ksize == t - 1)
		{
			y->keys[y->ksize] = nod->keys[i];
			y->ksize++;
			for (int j = i; j < nod->ksize - 1; j++)
			{
				nod->keys[i] = nod->keys[i + 1];
			}
			nod->ksize--;
			for (int j = 0; j < z->ksize; j++)
			{
				y->keys[y->ksize + j] = z->keys[j];
			}
			for (int j = 0; j <= z->ksize; j++)
			{
				y->legaturi[y->ksize + j] = z->legaturi[j];
			}
			y->ksize += z->ksize;
			delete z;
		}
		return y;
	}

	void delete_(Node* nod, T key)
	{
		int i = cautare_binara_pozitie(nod, key);
		if (i != -1)
		{
			if (i<nod->ksize && nod->keys[i] == key)
			{
				if (nod->isLeaf == true)
				{
					if (nod->ksize >= t)
					{
						for (int j = i; j < nod->ksize - 1; j++)
						{
							nod->keys[j] = nod->keys[j + 1];
						}
						nod->ksize--;
						return;
					}
					nod->ksize--;
					return;
				}
				else if (nod->legaturi[i]->ksize >= t)
				{
					int k = getPredecesorValue(key); 
					nod->keys[i] = k;
					delete_(nod->legaturi[i], k);
					return;
				}
				else if (nod->legaturi[i]->ksize == t - 1 && nod->legaturi[i + 1]->ksize == t - 1)
				{
					Node* y = fuziune(nod, i);
					delete_(y, key);
					return;
				}
			}
			if (i < nod->ksize && nod->keys[i] == key && nod->isLeaf == false && nod->legaturi[i + 1]->ksize >= t)
			{
				int k = getSucessorValue(key); 
				nod->keys[i] = k;
				delete_(nod->legaturi[i + 1], k);
				return;
			}
			if (i>=nod->ksize || nod->keys[i] != key && nod->isLeaf == false)
			{
				if (i <= nod->ksize && nod->legaturi[i]->ksize >= t)
				{
					delete_(nod->legaturi[i], key);
					return;
				}
				else if (i>=nod->ksize || nod->legaturi[i]->ksize == t - 1)
				{
					if (i-1>=0 && nod->legaturi[i -1] != nullptr && nod->legaturi[i - 1]->ksize >= t)
					{
						rot_dr(nod, i-1);
						delete_(nod, key);
						return;
					}
					else if (i<nod->ksize && nod->legaturi[i + 1] != nullptr && nod->legaturi[i + 1]->ksize >= t)
					{
						rot_st(nod, i);
						delete_(nod, key);
						return;
					}
					else if (i == nod->ksize && nod->legaturi[i] != nullptr && nod->legaturi[i]->ksize >= t)
					{
						rot_st(nod, i);
						delete_(nod, key);
						return;
					}
					else
					{
						Node* y;
						if (nod->legaturi[i + 1] != nullptr)
						{
							y = fuziune(nod, i);
						}
						else
						{
							y = fuziune(nod, i - 1);
						}
					//	delete_(nod, key);
						delete_(y, key);
						return;
					}
				}
			}
		}
	}

	int getPredecesorValue(T key)
	{
		Node* nod=search(root, key);
		if (nod != nullptr)
		{
			Node* y = getPredecesorNode(nod, cautare_binara_pozitie(nod, key));
			return y->keys[y->ksize-1];
		}
		return -1;
	}

	Node* getPredecesorNode(Node* nod, int i)
	{
		Node* y = nod->legaturi[i];
		Node* min_node=y;
		while (y != nullptr)
		{
			min_node = y;
			y = y->legaturi[y->ksize];
		}
		return min_node;
	}

	int getSucessorValue(T key)
	{
		Node* nod = search(root, key);
		if (nod != nullptr)
		{
			Node* y = getSucessorNode(nod, cautare_binara_pozitie(nod, key)+1);
			return y->keys[0];
		}
		return -1;
	}

	Node* getSucessorNode(Node* nod, int i)
	{
		Node* y = nod->legaturi[i];
		Node* max_node = y;
		while (y != nullptr)
		{
			max_node = y;
			y = y->legaturi[0];
		}
		return max_node;
	}

	void print()
	{
		std::pair<Node*, int> aux;
		int ultimul_nivel = 0;
		std::queue<std::pair<Node*, int>>coada;
		coada.push(std::make_pair(root, 0));
		while (coada.size() != 0)
		{
			aux = coada.front();
			if (aux.second != ultimul_nivel)
			{
				std::cout << std::endl;
			}
			else
			{
				for (int i = 0; i < 5; i++)
				{
					std::cout << " ";
				}
			}
			coada.pop();
			for (int i = 0; i < aux.first->ksize; i++)
			{
				std::cout << aux.first->keys[i] << " ";
				if (aux.first->legaturi[i] != nullptr)
				{
					coada.push(std::make_pair(aux.first->legaturi[i], aux.second+1));
				}
			}
			if (aux.first->legaturi[aux.first->ksize] != nullptr)
			{
				coada.push(std::make_pair(aux.first->legaturi[aux.first->ksize], aux.second+1));
			}
			ultimul_nivel=aux.second;
		}
	}

	Node* getRoot()
	{
		return root;
	}

};

template<typename T>
void menu(B_Tree<T> b)
{
	int op = 1;
	T predecesor;
	T key;
	T sucessor;
	while (op != 0)
	{
		std::cout << "Meniu" << std::endl;
		std::cout << "1. Adaugare element in B-tree" << std::endl;
		std::cout << "2. Stergere element din B-tree" << std::endl;
		std::cout << "3. Afisare B-tree pe niveluri" << std::endl;
		std::cout << "4. Afiseaza Sucessor" << std::endl;
		std::cout << "5. Afiseaza Predecesor" << std::endl;
		std::cout << "Selecteaza optiune: ";
		std::cin >> op;
		std::cout<<std::endl;

		switch (op)
		{
		case 1:
			T key;
			std::cout << "Citeste elementul pe care doresti sa il inserezi in B-tree: ";
			std::cin >> key;
			b.insert(key);
			std::cout << std::endl;
			break;
		case 2:
			key;
			std::cout << "Citeste elementul pe care doresti sa il stergi din B-tree: ";
			std::cin >> key;
			b.delete_(b.getRoot(), key);
			std::cout << std::endl;
			break;
		case 3:
			b.print();
			std::cout << std::endl;
			break;
		case 4:
			std::cout << "Citeste elementul a carui sucessor doresti sa il aflii: ";
			std::cin >> key;
			sucessor = b.getSucessorValue(key);
			if (sucessor != -1)
			{
				std::cout << std::endl;
				std::cout << "Sucessorul tau este: " << sucessor;
			}
			else
			{
				std::cout << std::endl;
				std::cout << "Nu am gasit sucessor pentru elementul ales";
			}
			std::cout << std::endl;
			break;
		case 5:
			std::cout << "Citeste elementul a carui predecesor doresti sa il aflii: ";
			std::cin >> key;
			predecesor = b.getPredecesorValue(key);
			if (predecesor != -1)
			{
				std::cout << std::endl;
				std::cout << "Predecesorul tau este: " << predecesor;
			}
			else
			{
				std::cout << std::endl;
				std::cout << "Nu am gasit predecesor pentru elementul ales";
			}
			break;
		case 0: 
			return;
		}
		std::cout << std::endl << std::endl;

	}
}


int main()
{
	B_Tree<int> b(2);
	menu(b);
}
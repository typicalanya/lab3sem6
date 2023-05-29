#include "incudes.h"


class Node //класс узла дл€ составлени€ списка, по которому будет создана таблица дл€ кодировки
{
public:
	int a = 0;
	char c = ' ';
	Node* left, * right;

	Node() { left = right = NULL; }

	Node(Node* L, Node* R)
	{
		left = L;
		right = R;
		a = L->a + R->a;
	}
};

struct MyCompare //компаратор дл€ сортировки указателей на узлы в списке
{
	bool operator()(const Node* l, const Node* r) const { return l->a < r->a; }
};

vector<bool> code;//вектор дл€ хранени€ кода символа, глобальный, так как используем рекурсию
map<char, vector<bool> > table;//таблица кодировки, где каждому символу соответсвует свой код, глобальна€, так как используем в 2-х функци€х

void BuildTable(Node* root)//функци€ дл€ создани€ таблицы
{
	if (root->left != NULL)
	{
		code.push_back(0);
		BuildTable(root->left);
	}

	if (root->right != NULL)
	{
		code.push_back(1);
		BuildTable(root->right);
	}

	if (root->left == NULL && root->right == NULL)
	{
		table[root->c] = code;
		cout << root->c << ":";
		for (int i = 0; i < code.size(); i++)
			cout << code[i];
	}

	if (code.size())
		code.pop_back();


}

void Huffman_alg()
{
	//создаЄм таблицу частот, где смотрим какой символ, сколько раз считываетс€
	string text = "", sub_str = "", file;
	HANDLE descriptor = GetStdHandle(STD_OUTPUT_HANDLE);
	short key = 0, code = 0;
	do
	{
		system("cls");
		key = key % 2;
		code = 0;
		if (key == 0)
		{
			SetConsoleTextAttribute(descriptor, 2);
			cout << "1. ¬вод вручную" << endl;
			SetConsoleTextAttribute(descriptor, 7);
			cout << "2. ¬вод с файла" << endl;
		}
		if (key)
		{
			cout << "1. ¬вод вручную" << endl;
			SetConsoleTextAttribute(descriptor, 2);
			cout << "2. ¬вод с файла" << endl;
			SetConsoleTextAttribute(descriptor, 7);
		}
		if (code != 13)
		{
			code = _getch();
			if (code == 80 || code == 115)
				++key;
			if (code == 72 || code == 119)
				--key;
		}
	} while (code != 13);
	system("cls");
	switch (key)
	{
	case 0:
	{
		cout << "¬ведите текст: ";
		getline(cin, text);
		break;
	}
	case 1:
	{
		cout << "¬ведите название файла (с расширением), если он в одной папке с программой, либо укажите полный путь до файла, включа€ расширение: ";
		cin >> file;
		ifstream f(file, ios::in | ios::binary);
		while (!f.eof())
		{
			getline(f, sub_str);
			text += sub_str;
		}
		cout << "файл считан" << endl;
		f.close();
		break;
	}
	default:
		cout << "error found, fix it!" << endl;
		break;
	}


	map<char, int> freq;
	char c;
	for (int i = 0; i < text.length(); i++)
	{
		c = text[i];
		freq[c]++;
	}
	//создаЄм список из таблицы частот
	list<Node*> lst;
	for (map<char, int>::iterator itr = freq.begin(); itr != freq.end(); ++itr)
	{
		Node* p = new Node;
		p->c = itr->first;
		p->a = itr->second;
		lst.push_back(p);
		cout << itr->first << ":" << itr->second << endl;
	}
	cout << "список частот создан" << endl;

	//создаЄм дерево, по которому будем создавать таблицу кодировки
	while (lst.size() != 1)
	{
		lst.sort(MyCompare());

		Node* SonL = lst.front();
		lst.pop_front();
		Node* SonR = lst.front();
		lst.pop_front();

		Node* parent = new Node(SonL, SonR);
		lst.push_back(parent);

	}
	cout << "дерево создано" << endl;

	Node* root = lst.front();   //root - указатель на вершину дерева

	//создаЄм таблицу кодировки, сама таблица будет в глобальной переменной table
	BuildTable(root);
	cout << "таблица создана" << endl;
	cout << "записываем код в файл" << endl;
	//выводим коды в бинарный файл


	ofstream g("output.bin");

	int count = 0;
	char buff = 0;
	BYTE buf = 0;
	for (int i = 0; i < text.length(); i++)
	{
		c = text[i];
		vector<bool> x = table[c];
		for (int n = 0; n < x.size(); n++)
		{
			buf = buf | x[n] << (7 - count);
			count++;
			if (count == 8)
			{
				count = 0;
				g << buf;
				cout << buf;
				buf = 0;
			}
		}
		if ((i + 1) == text.length() && count != 8)//в итоговыую кодировку записываетс€ лишний символ, чтобы не потер€ть исходные символы, при декодировании, последний символ не учитывать.
		{
			g << buf;
			cout << buf;
		}
	}


	g.close();
	cout << "сжатые данные записаны в файл 'output.bin'." << endl;

	cout << "ƒекодировать файл обратно?[Y(y)/N(n)]: ";
	char choice;
	cin >> choice;
	if (choice == 'Y' || choice == 'y')
	{
		//раскодирование файла обратно и вывод результата(должно совпадать с изначальным текстом)
		ifstream F("output.bin", ios::in | ios::binary);
		Node* p = root;
		count = 0;
		char byte;
		byte = F.get();
		while (!F.eof())
		{
			bool b = byte & (1 << (7 - count));
			if (b)
				p = p->right;
			else
				p = p->left;
			if (p && (p->left == NULL && p->right == NULL))
			{
				cout << p->c;
				p = root;
			}
			count++;
			if (count == 8)
			{
				count = 0;
				byte = F.get();
			}
		}
		F.close();
	}
}
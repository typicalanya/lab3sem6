#include "incudes.h"


class Node //����� ���� ��� ����������� ������, �� �������� ����� ������� ������� ��� ���������
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

struct MyCompare //���������� ��� ���������� ���������� �� ���� � ������
{
	bool operator()(const Node* l, const Node* r) const { return l->a < r->a; }
};

vector<bool> code;//������ ��� �������� ���� �������, ����������, ��� ��� ���������� ��������
map<char, vector<bool> > table;//������� ���������, ��� ������� ������� ������������ ���� ���, ����������, ��� ��� ���������� � 2-� ��������

void BuildTable(Node* root)//������� ��� �������� �������
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
	//������ ������� ������, ��� ������� ����� ������, ������� ��� �����������
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
			cout << "1. ���� �������" << endl;
			SetConsoleTextAttribute(descriptor, 7);
			cout << "2. ���� � �����" << endl;
		}
		if (key)
		{
			cout << "1. ���� �������" << endl;
			SetConsoleTextAttribute(descriptor, 2);
			cout << "2. ���� � �����" << endl;
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
		cout << "������� �����: ";
		getline(cin, text);
		break;
	}
	case 1:
	{
		cout << "������� �������� ����� (� �����������), ���� �� � ����� ����� � ����������, ���� ������� ������ ���� �� �����, ������� ����������: ";
		cin >> file;
		ifstream f(file, ios::in | ios::binary);
		while (!f.eof())
		{
			getline(f, sub_str);
			text += sub_str;
		}
		cout << "���� ������" << endl;
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
	//������ ������ �� ������� ������
	list<Node*> lst;
	for (map<char, int>::iterator itr = freq.begin(); itr != freq.end(); ++itr)
	{
		Node* p = new Node;
		p->c = itr->first;
		p->a = itr->second;
		lst.push_back(p);
		cout << itr->first << ":" << itr->second << endl;
	}
	cout << "������ ������ ������" << endl;

	//������ ������, �� �������� ����� ��������� ������� ���������
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
	cout << "������ �������" << endl;

	Node* root = lst.front();   //root - ��������� �� ������� ������

	//������ ������� ���������, ���� ������� ����� � ���������� ���������� table
	BuildTable(root);
	cout << "������� �������" << endl;
	cout << "���������� ��� � ����" << endl;
	//������� ���� � �������� ����


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
		if ((i + 1) == text.length() && count != 8)//� ��������� ��������� ������������ ������ ������, ����� �� �������� �������� �������, ��� �������������, ��������� ������ �� ���������.
		{
			g << buf;
			cout << buf;
		}
	}


	g.close();
	cout << "������ ������ �������� � ���� 'output.bin'." << endl;

	cout << "������������ ���� �������?[Y(y)/N(n)]: ";
	char choice;
	cin >> choice;
	if (choice == 'Y' || choice == 'y')
	{
		//�������������� ����� ������� � ����� ����������(������ ��������� � ����������� �������)
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
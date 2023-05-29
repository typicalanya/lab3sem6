#include "incudes.h"


struct Segment
{
	long double left, right;
};


void ArithmeticCoding()
{
	cout << "������� �������� ����� (� �����������), ���� �� � ����� ����� � ����������, ���� ������� ������ ���� �� �����, ������� ����������: ";
	string file;
	cin >> file;
	ifstream f(file);

	map<char, double>  freq;
	char c;
	string str, original_text = "";
	double n = 0;//���-�� �������� � ������
	while (!f.eof())//������ ������� ������-���-�� ��� ���������, ��������� ����������� ����� � original_text
	{
		getline(f, str);
		for (int i = 0; i < str.length(); i++)
		{
			c = str[i];
			freq[c]++;
			n++;
			original_text += c;
		}
	}



	for (map<char, double>::iterator itr = freq.begin(); itr != freq.end(); ++itr)//������ ������� ����������� ��������� �����
	{

		itr->second = itr->second / n;
		cout << itr->first << ":" << itr->second << endl;
	}
	int num = -1;
	long factorial = 1.0;


	map<char, Segment> segments;//������ ������� ���������, ��� ������ ������ ��������� � ������� �� 0 �� 1
	long double l = 0;
	for (map<char, double>::iterator itr = freq.begin(); itr != freq.end(); ++itr)
	{
		segments[itr->first].left = l;
		segments[itr->first].right = l + itr->second;
		l = segments[itr->first].right;
		cout << setprecision(25) << itr->first << ":[" << segments[itr->first].left << ";" << segments[itr->first].right << ")" << endl;
	}


	long double left = 0, right = 1;//�������� ����������� ������ � ����� ���, �������������� ������������ ������ �� 0 �� 1
	for (int i = 0; i < original_text.size(); i++)
	{
		char ch = original_text[i];
		long double new_right = left + (right - left) * segments[ch].right;
		long double new_left = left + (right - left) * segments[ch].left;
		right = new_right;
		left = new_left;
		cout << left << ";" << right << endl;
	}
	long double result = (left + right) / 2;
	cout << "������������ � " << result << endl;
	cout << "���� ������" << endl;


	cout << "�������������: " << endl;
	string decode = "";
	for (int i = 0; i < original_text.size(); i++)
	{

		for (map<char, Segment>::iterator j = segments.begin(); j != segments.end(); j++)
		{
			if (result >= j->second.left && result < j->second.right)
			{
				decode += j->first;
				result = (result - j->second.left) / (j->second.right - j->second.left);
				break;
			}
		}
	}
	cout << decode << endl;
}

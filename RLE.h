#include "incudes.h"

void RLE()
{
    string text = "", sub_str = "";
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
            cout << "1. Ввод вручную" << endl;
            SetConsoleTextAttribute(descriptor, 7);
            cout << "2. Ввод с файла" << endl;
        }
        if (key)
        {
            cout << "1. Ввод вручную" << endl;
            SetConsoleTextAttribute(descriptor, 2);
            cout << "2. Ввод с файла" << endl;
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
        cout << "Введите текст: ";
        getline(cin, text);
        break;
    }
    case 1:
    {
        cout << "Введите название файла (с расширением), если он в одной папке с программой, либо укажите полный путь до файла, включая расширение: ";
        string file;
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
    ofstream g("output.bin");
    int n = text.length();
    for (int i = 0; i < n; i++)
    {
        sub_str = "";
        int count = 0;
        while (i < n && text[i] != text[i + 1])
        {
            sub_str += text[i];
            count++;
            i++;
        }
        if (count)
            cout << count * (-1) << sub_str;
        g << count * (-1) << sub_str;
        sub_str = "";
        count = 1;
        while (i < n && text[i] == text[i + 1])
        {
            sub_str += text[i];
            count++;
            i++;
        }
        if (i != n)
        {
            cout << count << text[i];
            g << count << text[i];
        }
    }
    g.close();
    cout << "\nсжатые данные записаны в файл 'output.bin'." << endl;
    text.clear();
    cout << "Декодировать файл обратно?[Y(y)/N(n)]: ";
    char choice;
    cin >> choice;
    if (choice == 'Y' || choice == 'y')
    {
        cout << "Декодирование:" << endl;
        ifstream F("output.bin", ios::in | ios::binary);
        while (!F.eof())
        {
            getline(F, sub_str);
            text += sub_str;
        }
        for (int i = 0; i < text.length(); i++)
        {
            sub_str = "";
            if (text[i] == '-')
            {
                i++;
                while (isdigit(text[i]))
                {
                    sub_str += text[i];
                    i++;
                }
                int count = stoi(sub_str);
                for (int j = 0; j < count; j++)
                {
                    cout << text[j + i];
                }
            }
            if (isdigit(text[i]))
            {
                while (isdigit(text[i]))
                {
                    sub_str += text[i];
                    i++;
                }
                int count = stoi(sub_str);
                for (int j = 0; j < count; j++)
                {
                    cout << text[i];
                }
            }
        }
        F.close();
    }
}

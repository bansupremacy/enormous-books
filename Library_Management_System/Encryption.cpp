#include "Encryption.h"


/*---Password encryption: Vigenere cipher---*/
string Encrypt(string Password)
{
	char Convert[63];
	char Table[63][63];
	char *Encrypt;
	string Result;
	string Key = "AS_siLXPO";
	int j = 0;
	char ch;
	for (ch = '0'; j < 63; j++, ch++)
	{
		if (ch == ':')
			ch = 'A';
		if (ch == '[')
			ch = '_';
		if (ch == '`')
			ch = 'a';
		if (ch == 'z' + 1)
			ch = '0';
		Convert[j] = ch;
	}
	j = 0;
	int i = 0;
	ch = '0';
	char ch2;
	for (i = 0; i < 63; i++, ch++)
	{
		if (ch == ':')
			ch = 'A';
		if (ch == '[')
			ch = '_';
		if (ch == '`')
			ch = 'a';
		if (ch == 'z' + 1)
			ch = '0';
		ch2 = ch;
		for (j = 0; j < 63; j++, ch2++)
		{
			if (ch2 == ':')
				ch2 = 'A';
			if (ch2 == '[')
				ch2 = '_';
			if (ch2 == '`')
				ch2 = 'a';
			if (ch2 == 'z' + 1)
				ch2 = '0';
			Table[i][j] = ch2;
		}
	}
	int l = Password.length();
	int lKey = Key.length();
	int xPass;
	int yKey;
	Encrypt = new char[l + 1];
	j = 0;
	int m = 0;
	for (i = 0; i < l; i++, j++)
	{
		if (j == lKey)
			j = 0;
		for (int k = 0; k < 63; k++)
		{
			if (Password[i] == Convert[k])
				xPass = k;
			if (Key[j] == Convert[k])
				yKey = k;
		}

		Encrypt[m] = Table[yKey][xPass];
		m++;
	}
	Encrypt[m] = '\0';
	Result = Encrypt;
	delete Encrypt;
	return Result;
}

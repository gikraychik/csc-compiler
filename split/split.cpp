#include <iostream>
#include <fstream>
#include <malloc.h>
#include <string>
#include <string.h>
#include <stdio.h>

using namespace std;
void reverse(char s[])
{
    int i, j;
    char c;
    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
	 c = s[i];
	 s[i] = s[j];
	 s[j] = c;
    }
}
char *itoa(int n)
{
	char *s = (char *)malloc(33*sizeof(char));
	int i, sign;
	if ((sign = n) < 0)  /* записываем знак */
      		n = -n;          /* делаем n положительным числом */
  		i = 0;
     		do {       /* генерируем цифры в обратном порядке */
         		s[i++] = n % 10 + '0';   /* берем следующую цифру */
     		} while ((n /= 10) > 0);     /* удаляем */
     		if (sign < 0)
         		s[i++] = '-';
     		s[i] = '\0';
     		reverse(s);
	return s;
}

int main()
{
	string str;
	ifstream infile;
	infile.open("result");
	ofstream outputFile;
	outputFile.open("0.nlt");
	const char *firstLine = "INCLUDE(regs.inc)";
	outputFile << firstLine << endl;
	int i = 0;
        while(!infile.eof()) // To get you all the lines.
        {
	        getline(infile, str); // Saves the line in str
	        const char *data = str.data();
		if (*data == '*')
		{
			outputFile.close();
			i++;
			string newFile = string(itoa(i)) + string(".nlt");  //memory leak
			outputFile.open(newFile.data());
			outputFile << firstLine << endl;
		}
		else { outputFile << str << endl; }
        }
	infile.close();
	outputFile.close();
	string delFile = string(itoa(i)) + string(".nlt");  //memory leak
	remove(delFile.data());
	cout << "Done!\n";

	return 0;
}

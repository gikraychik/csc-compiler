#include <iostream>
#include <exception>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <cstdio>

using namespace  std;
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
	int i = 0;
	while (1)
	{
		string str;
		FILE *f = (FILE *)fopen("rw", (string(itoa(i)) + string(".nlt")).data());
		while (*(str.data())!='*');		
		{
			cin >> str;
			fprintf(f, str.data());
			fprintf(f, "%s", "\n");
		}
		i++;
		if (*(str.data())==EOF) { break; }
	}
	return 0;
}

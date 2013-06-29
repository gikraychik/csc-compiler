#include <iostream>
#include <exception>

using namespace  std;

int devide(int x, int y) throw(const char *)
{
	if (y == 1) { throw("div by zero"); }	
	return x / y;
}
int main()
{
	int res = 100;
	try
	{
		int res = devide(2, 2);
	}
	catch(char const *e)
	{
		cout << "Caught: " << e << endl;
	}
	catch(...)
	{
		cout << "Caught: exception " << "e" << endl;
	}
	cout << res << endl;
	int i = 0;
	while (++i < 10) { cout << i << " "; }
	return 0;
}

#include <iostream>


int main(int argc, char const *argv[])
{
	int sum = 0;
	for(int i = 0 ;i < 100000000; i++)
	{
		sum += i;
	}
	std::cout << sum << '\n';
	return 0;
}

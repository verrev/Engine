#include "Converter.h"

int main(int argc, char** argv)
{
	for (int i = 1; i < argc; ++i)
		Converter::convertScene(argv[i]);
	char f; std::cin >> f;
	return 0;
}
#include <iostream>

#include "heap.h"
#include "ui.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cerr << "Usage: heapview <filename>" << std::endl;
		return 1;
	}

	Heap heap;
	UI ui(argc, argv);
	bool quit = false;

	heap.registerClient(ui);

	while (quit == false) {
		heap.simulate(argv[1]);
		quit = ui.poll();
	}

	heap.deregisterClient(ui);

	return 0;
}


#include "LibParser.h"
#include <conio.h>

// GO GO GADGET
int main(int argc, char **argv) {

	LibParser * parser = new LibParser();

	parser->read("C:\\Users\\Gateway\\Downloads\\Client\\Data\\Background.lib");

	_getch();

	return 0;
}

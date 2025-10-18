//#include <iostream>
#include <fstream>
//#include <vector>
#include <string>
#include <filesystem>
using namespace std;

#include "psxsound.h"





int main(int argc, char* argv[]) {
	if (argc < 3) {
		cout << "Usage: (mode) (file)" << endl;
		cout << "Modes:\n0 - Open VB and export all VAG sound\n1 - Open VH and display information";
		return -1;
	}
	if (strcmp(argv[1], "0") == 0) {
		VBFile file = PSX_ImportVB(argv[2]);
		PSX_ExportVAGFromVBFile(&file);
	}
	else {
		VHFile file = PSX_ImportVH(argv[2]);
		PSX_ShowVHData(&file);
	}


	return 0x00;
}
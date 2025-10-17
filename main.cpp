//#include <iostream>
#include <fstream>
//#include <vector>
#include <string>
#include <filesystem>
using namespace std;

#include "sound.h"





int main() {

	VBFile result = ImportVB("./x64/Debug/test2.VB");
	ExportVAGFromVBFile(&result);


	return 0x00;
}
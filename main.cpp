//#include <iostream>
#include <fstream>
//#include <vector>
#include <string>
#include <filesystem>
using namespace std;

#include "psxsound.h"





int main() {

	//VBFile result = ImportVB("./x64/Debug/test2.VB");
	//ExportVAGFromVBFile(&result);
	VHFile result = PSX_ImportVH("./x64/Debug/test1.VH");
	PSX_ShowVHData(&result);

	return 0x00;
}
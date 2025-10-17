#include "sound.h"

#include <fstream>
#include <filesystem>
#include <string>


enum ErrorCode : uint8_t {
	None = 0,
	FileOperation,
	Invalid,
};
void ThrowError(ErrorCode type, string message) {
	cout << "An error has occured (code " << type << "): " << message << endl;
	throw type;
	//exit(type);
}



int CharArrayToInt(char* inString) { // 00 00 00 07 = (int)7 
	if (sizeof(inString) < 4) {
		ThrowError(ErrorCode::Invalid, "Char array's size is too tiny!");
	}
	int returnValue = -1;
	for (uint8_t i = 0; i < 4; i++) {
		returnValue += inString[i];
	}
	return returnValue;
}




extern VHFile ImportVH(std::string fileName) {
	VHFile returnValue;
	ifstream inFile(fileName);
	if (!inFile || !inFile.is_open()) {
		ThrowError(ErrorCode::FileOperation, "Failed to open VH file.");
	}
	char* tempRead = nullptr;
	inFile.read(tempRead, 5); //Header
	if (tempRead != "pBAV") {
		ThrowError(ErrorCode::Invalid, "File has invalid header! AKA it isn't a VH file!");
	}
	inFile.read(tempRead, 4); //Version
	returnValue.version = CharArrayToInt(tempRead);
	inFile.read(tempRead, 4); //VAB ID
	returnValue.VAB_ID = CharArrayToInt(tempRead);
	inFile.read(tempRead, 4);
	returnValue.VAB_TotalFileSize = CharArrayToInt(tempRead);
	
	cout << "Sucessfully read VH File!";
	return returnValue;
}
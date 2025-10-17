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

extern VBFile ImportVB(string fileName) {
	VBFile returnValue;
	ifstream File_VB(fileName, ios::binary);
	if (!File_VB || !File_VB.is_open()) {
		cout << "Failed to open file!" << endl;
		//return returnValue;
	}
	
	size_t soundSize = 200000;
	char currentByte;

	VBBlock currentBlock;



	//filesystem::create_directory("./x64/Debug/OUTFINAL");

	while(File_VB) {
		//if (File_VB.peek() == -1) break;

		File_VB.get(currentByte);
		currentBlock.ShiftAndFilter = currentByte;



		File_VB.get(currentByte);
		currentBlock.Flag = currentByte;

		for (uint8_t ADPCMIndex = 0; ADPCMIndex < 14; ADPCMIndex++) {
			File_VB.get(currentByte);
			currentBlock.ADPCM[ADPCMIndex] = currentByte;
		}

		if (currentBlock.ShiftAndFilter == 0x00) {
			if (currentBlock.Flag == 0x00) {
				uint8_t i = 0;
				for (; i < 14; i++) {
					if (currentBlock.ADPCM[i] != 0x00) {
						break;
					}
				}
				//cout << endl;
				if (i == 14) {
					//cout << "Detected new sound at " << File_VB.tellg() << "I = " << (int)i << endl;
					returnValue.sounds.push_back(vector<VBBlock>());
					continue;
				}
			}
			else if (currentBlock.Flag == 0x07) {
				uint8_t i = 0;
				for (; i < 14; i++) {
					if (currentBlock.ADPCM[i] != 0x77 && currentBlock.ADPCM[i] != 0x07) {
						break;
					}
				}
				if (i == 14) continue;

			}
			//break;

		}

		returnValue.sounds.at(returnValue.sounds.size() - 1).push_back(currentBlock);
		//cout << "Pushing back data";

		//cout << "End of " << File_VB.tellg() << endl;
	}
	cout << "Sucessfully read information!\nInfo: Size: " << returnValue.sounds.size() << endl;
	return returnValue;
}

void ExportVAGFromVBFile(VBFile* in) {
	ofstream outFile;
	

	filesystem::create_directory("./x64/Debug/OutVersion2");

	uint8_t VB_SIZE = in->sounds.size();
	for (uint8_t VAG_Index = 0; VAG_Index < VB_SIZE; VAG_Index++) {
		vector<VBBlock>& currentVAG = in->sounds.at(VAG_Index);


		outFile.open("./x64/Debug/OutVersion2/sound" + to_string(VAG_Index + 1) + ".raw", ios::binary);
		if (!outFile || !outFile.is_open()) {
			cout << "Failed to create file." << endl;
			return;
		}
		//cout << "New VAG (ind" << (int)VAG_Index << "): " << currentVAG.size() * 16 << endl;
		for (const VBBlock& currentBlock : currentVAG) {
			outFile << currentBlock.ShiftAndFilter;
			//cout << (int)currentBlock.ShiftAndFilter << " ";
			outFile << currentBlock.Flag;
			//cout << (int)currentBlock.Flag << " ";
			for (uint8_t i = 0; i < 14; i++) {
				outFile << currentBlock.ADPCM[i];
				//cout << (int)currentBlock.ADPCM[i] << " ";
			}
			//cout << endl;
		}
		outFile.close();
	}
}
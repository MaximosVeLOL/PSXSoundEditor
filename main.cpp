//#include <iostream>
#include <fstream>
//#include <vector>
#include <string>
#include <filesystem>
using namespace std;

#include "sound.h"





int main() {
	ifstream File_VB("./x64/Debug/test2.VB", ios::binary);
	if (!File_VB || !File_VB.is_open()) {
		cout << "Failed to open file!" << endl;
		return -1;
	}

	size_t soundSize = 200000;

	vector<VBBlock> sound;
	char currentByte;

	VBBlock currentBlock;
	uint8_t soundCount = 0;
	filesystem::create_directory("./x64/Debug/OUTFINAL");
	while (File_VB) {
		if (File_VB.peek() == -1) break;


		//cout << "Read shift and filter" << endl;
		//cout << "Current: ";
		File_VB.get(currentByte);
		currentBlock.ShiftAndFilter = currentByte;
		//cout << (int)currentByte << " ";


			
		File_VB.get(currentByte);
		currentBlock.Flag = currentByte;
		//cout << (int)currentByte << " ";
		//cout << "Read ADPCM" << endl;
			
		for (uint8_t ADPCMIndex = 0; ADPCMIndex < 14; ADPCMIndex++) {
			File_VB.get(currentByte);
			currentBlock.ADPCM[ADPCMIndex] = currentByte;
			//cout << (int)currentByte << " ";
		}
		//cout << endl;
		bool soundCounts = true;

		if (currentBlock.ShiftAndFilter == 0x00) {
			//cout << "Found something!" << endl;
			//cout << (int)currentBlock.ShiftAndFilter << " " << (int)currentBlock.Flag << " ";
			/*for (uint8_t ADPCMIndex = 0; ADPCMIndex < 14; ADPCMIndex++) {
				cout << (int)currentBlock.ADPCM[ADPCMIndex] << " ";
			}*/
			//cout << endl;
			if (currentBlock.Flag == 0x00) {
				uint8_t i = 0;
				for (; i < 14; i++) {
					if (currentBlock.ADPCM[i] != 0x00) break;
				}
				if (i == 14) {
					cout << "AAAAAAAAAAAAAAAAAAA" << File_VB.tellg() << endl;
					soundCounts = false;
					soundCount++;
				}
			}
			else if (currentBlock.Flag == 0x07) {
				uint8_t i = 0;
				for (; i < 14; i++) {
					if (currentBlock.ADPCM[i] != 0x77 && currentBlock.ADPCM[i] != 0x07) break;
				}
				if (i == 14) {
					cout << "EEEEEEE!" << endl;
					soundCounts = false;
					ofstream outFile("./x64/Debug/OUTFINAL/sound" + to_string(soundCount) + ".raw", ios::binary);
					if (!outFile || !outFile.is_open()) {
						cout << "Failed to create file." << endl;
						return -1;
					}
					for (auto &element : sound) {
						outFile << element.ShiftAndFilter;
						outFile << element.Flag;
						for (uint8_t ad = 0; ad < 14; ad++) {
							outFile << element.ADPCM[ad];
						}
					}
					outFile.close();
					sound.erase(sound.begin(), sound.end());
				
				}

			}
			//break;

		}

		if (soundCounts) sound.push_back(currentBlock);

		//cout << "End of " << File_VB.tellg() << endl;
	}
	//cout << "End for loop";


	return 0x00;
}
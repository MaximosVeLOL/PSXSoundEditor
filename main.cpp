#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void ReadVBFile(string fileName) {
	ifstream VB(fileName, ios::binary);
	if (!VB || !VB.is_open()) {
		cout << "Failed to open file!" << endl;
		return;
	}
	cout << "Successfully opened file!" << endl;


	vector<uint8_t> soundPacket; //The packet that we are exporting
	char currentLine[16]; //All the data we read in a line
	//ofstream outputSound;

	bool readFile = false; //Just incase a new sound doesn't immediately appear after sound ends...
	uint16_t soundCount = 0; //We only need this for output LOL out loud

	while (VB) {
		if (VB) {
			//cout << "At " << VB.tellg() << endl;
			
			for (int i = 0; i < 16; i++) {
				VB.get(currentLine[i]);
				if (readFile) {
					soundPacket.push_back(currentLine[i]);
				}
			}
			if (currentLine[0] == 0x00) { //Check first, second, third and last byte of every line... So four bytes total needed!
				if (currentLine[1] == 0x00 && currentLine[2] == 0x00 && currentLine[15] == 0x00) {
					cout << "New sound detected!" << endl;
					soundCount++;
					readFile = true;
				}
				if (currentLine[1] == 0x07 && (currentLine[2] == 0x77 && currentLine[15] == 0x77 || currentLine[2] == 0x07 && currentLine[15] == 0x07)) {
					cout << "End sound detected!" << endl;
					ofstream outputSound("test" + char('0' + soundCount));
					if (!outputSound || !outputSound.is_open()) {
						cout << "Failed to create sound." << endl;
						return;
					}
					for (int i = 0; i < soundPacket.size(); i++) {
						outputSound << soundPacket.at(i);
					}
					outputSound.close();
					soundPacket.erase(soundPacket.begin());
					readFile = false;
				}
			}
			//cout << "Line value: " << lineValue << endl;
			//cout << "Line: " << CharAsRealValue(currentLine) << endl;
		}
	}
}



int main(int argc, const char **argv) {
	ReadVBFile("test2.VB");
}
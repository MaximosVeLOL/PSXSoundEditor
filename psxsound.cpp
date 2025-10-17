#include "psxsound.h"

#include <fstream>
#include <filesystem>
#include <string>





enum ErrorCode : uint8_t {
	None = 0,
	FileOperation,
	Invalid,
};
void ThrowError(ErrorCode type, string message) {
	cout << "An error has occured (code " << (int)type << ", at line " << __LINE__ << "): " << message << endl;
	//throw type;
	exit(type);
}






uint8_t VH_ReadAsByte(ifstream* fileStream) { //Do we really need this?
	char returnValue = 0xFF;
	if(fileStream->peek() != -1)
		fileStream->get(returnValue);
	return returnValue;
}
uint16_t VH_ReadAsShort(ifstream* fileStream) {
	uint16_t returnValue = 0;
	char read;
	for (uint8_t i = 0; i < 2; i++) {
		fileStream->get(read);
		returnValue += read;
	}
	return returnValue;
}
uint32_t VH_ReadAsInt(ifstream *fileStream) {
	uint32_t returnValue = 0;
	char read;
	for (uint8_t i = 0; i < 4; i++) {
		fileStream->get(read);
		returnValue += read;
	}
	return returnValue;
}
long VH_ReadAsLong(ifstream* fileStream) {
	short returnValue = 0;
	char read;
	for (uint8_t i = 0; i < 8; i++) {
		fileStream->get(read);
		returnValue += read;
	}
	return returnValue;
}
size_t VH_ReadAsSizeT(ifstream* fileStream) {
	size_t returnValue = 0;
	char read;
	for (uint8_t i = 0; i < 16; i++) {
		fileStream->get(read);
		returnValue += read;
	}
	return returnValue;
}


extern VHFile PSX_ImportVH(std::string fileName) {
	VHFile returnValue;
	ifstream File_VH(fileName, ios::binary);
	if (!File_VH || !File_VH.is_open()) {
		ThrowError(ErrorCode::FileOperation, "Failed to open VH file.");
	}
	const char header[4]{ 'p', 'B', 'A', 'V' };
	char tempRead;
	for(uint8_t i = 0 ; i < 4; i++) {
		File_VH.get(tempRead);
		if (tempRead != header[i])
			ThrowError(ErrorCode::Invalid, "File has invalid header! AKA it isn't a VH file!");
	}

	returnValue.version = VH_ReadAsInt(&File_VH);

	returnValue.VAB_ID = VH_ReadAsInt(&File_VH);

	returnValue.VAB_TotalFileSize = VH_ReadAsInt(&File_VH);


	returnValue.Reserved = VH_ReadAsShort(&File_VH);

	returnValue.ProgramAmount = VH_ReadAsShort(&File_VH);
	returnValue.prgAttr = new ProgramAttributes[returnValue.ProgramAmount];

	returnValue.ToneAmount = VH_ReadAsShort(&File_VH);
	returnValue.tnAttr = new ToneAttributes[returnValue.ToneAmount];

	returnValue.VagAmount = VH_ReadAsShort(&File_VH);
	returnValue.VagSizes = new short[returnValue.VagAmount];


	//Bytes are special.
	returnValue.Master_Volume = VH_ReadAsByte(&File_VH);

	returnValue.Master_Pan = VH_ReadAsByte(&File_VH);

	returnValue.BankAttr1 = VH_ReadAsByte(&File_VH);

	returnValue.BankAttr2 = VH_ReadAsByte(&File_VH);

	//TODO - Read program attributes, tone attributes, and vag file sizes.

	short i = 0;
	for (; i < returnValue.ProgramAmount; i++) {
		ProgramAttributes* p = &returnValue.prgAttr[i];

		p->ToneAmount = VH_ReadAsByte(&File_VH);

		p->Master_Volume = VH_ReadAsByte(&File_VH);

		p->priority = VH_ReadAsByte(&File_VH);
		
		p->mode = VH_ReadAsByte(&File_VH);
		
		p->Master_Pan = VH_ReadAsByte(&File_VH);
		
		p->Reserved0 = VH_ReadAsByte(&File_VH);
		
		p->Attribute = VH_ReadAsShort(&File_VH);
		
		p->Reserved1 = VH_ReadAsInt(&File_VH);
		
		p->Reserved2 = VH_ReadAsInt(&File_VH);
	}
	for (i = 0; i < returnValue.ToneAmount; i++) {
		ToneAttributes* t = &returnValue.tnAttr[i];
		t->priority = VH_ReadAsByte(&File_VH);

		t->mode = VH_ReadAsByte(&File_VH);
		
		t->vol = VH_ReadAsByte(&File_VH);
		
		t->panning = VH_ReadAsByte(&File_VH);
		
		t->center = VH_ReadAsByte(&File_VH);
		
		t->shift = VH_ReadAsByte(&File_VH);
		
		t->min = VH_ReadAsByte(&File_VH);
		
		t->max = VH_ReadAsByte(&File_VH);

		t->vibW = VH_ReadAsByte(&File_VH);
		t->vibT = VH_ReadAsByte(&File_VH);
		
		t->porW = VH_ReadAsByte(&File_VH);
		t->porT = VH_ReadAsByte(&File_VH);
		
		t->PBmin = VH_ReadAsByte(&File_VH);
		t->PBmax = VH_ReadAsByte(&File_VH);

		t->Reserved = VH_ReadAsByte(&File_VH);
		t->Reserved1 = VH_ReadAsByte(&File_VH);

		t->AttackDecay = VH_ReadAsShort(&File_VH);
		t->ReleaseSustain = VH_ReadAsShort(&File_VH);
		t->ProgramNumber = VH_ReadAsShort(&File_VH);
		t->VAG_ID = VH_ReadAsShort(&File_VH);
		t->Reserved2 = VH_ReadAsLong(&File_VH); //Had to make a function for this...
	}

	
	cout << "Sucessfully read VH File!" << endl;
	return returnValue;
}

extern void PSX_ShowVHData(VHFile* in) {
	cout << "ID: pBAV" << endl;
	cout << "Version: " << in->version << endl;
	cout << "VAB ID: " << in->VAB_ID << endl;
	cout << "VAB Total File Size: " << in->VAB_TotalFileSize << endl;
	cout << "Reserved (1/2): " << in->Reserved << endl;
	cout << "Program Amount: " << in->ProgramAmount << endl;
	cout << "Tone Amount: " << in->ToneAmount << endl;
	cout << "VAG Amount: " << in->VagAmount << endl;

	//We aren't text, We are bytes!
	cout << "Master Volume: " << (int)in->Master_Volume << endl;
	cout << "Master Pan: " << (int)in->Master_Pan << endl;
	cout << "Bank Attribute (1/2): " << (int)in->BankAttr1 << endl;
	cout << "Bank Attribute (2/2): " << (int)in->BankAttr2 << endl;


	cout << "Reserved (2/2): " << in->ReservedAgain << endl;


	short i = 0;
	for (; i < in->ProgramAmount; i++) {
		cout << "Information " << i << ':' << endl;
		ProgramAttributes* current = &in->prgAttr[i];
		cout << "	Tone Amount: " << (int)current->ToneAmount << endl;

		cout << "	Master Volume: " << (int)current->Master_Volume << endl;

		cout << "	Priority: " << (int)current->priority << endl;

		cout << "	Mode: " << (int)current->mode << endl;

		cout << "	Master Pan: " << (int)current->Master_Pan << endl;

		cout << "	Reserved (1/3): " << (int)current->Reserved0 << endl;

		cout << "	Attribute: " << current->Attribute << endl;

		cout << "	Reserved (2/3): " << current->Reserved1 << endl;
		cout << "	Reserved (3/3): " << current->Reserved2 << endl;
	}



	//Prorgam attributes go here
	//Tone attributes go here
	//Vag sizes go here


}

extern VBFile PSX_ImportVB(string fileName) {
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
				for (; i < 14; i++) { //Check ALL of the ADPCM, because even though we only need to check 4 bytes, there are still some cases where we falsely think the ADPCM is a header.
					if (currentBlock.ADPCM[i] != PSX_HEADER_SOUND_END_V1 && currentBlock.ADPCM[i] != PSX_HEADER_SOUND_END_V2) {
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

void PSX_ExportVAGFromVBFile(VBFile* in) {
	ofstream outFile;
	

	filesystem::create_directory("./x64/Debug/OutVersion2");

	uint8_t VB_SIZE = in->sounds.size();
	for (uint8_t VAG_Index = 0; VAG_Index < VB_SIZE; VAG_Index++) {
		vector<VBBlock>& currentVAG = in->sounds.at(VAG_Index);


		outFile.open("./x64/Debug/OutVersion2/sound" + to_string(VAG_Index + 1) + ".raw", ios::binary);
		if (!outFile || !outFile.is_open()) {
			ThrowError(ErrorCode::FileOperation, "Failed to create file.");
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
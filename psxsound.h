#pragma once

/*
	This is everything I know about the sound file format for the PS1.
	Here are the people who I want to give a big thanks to:
		Parappa 2 Modding Discord
		PSXDEV (discord and site)

*/


#include "stdint.h"

#include <vector>
#include <iostream>
using namespace std;






#define PSX_HEADER_SOUND_START 0x00
#define PSX_HEADER_SOUND_END_V1 0x07
#define PSX_HEADER_SOUND_END_V2 0x77






//Taken from CKDEV's comment in the Parappa Modding Discord
struct VBBlock { //A block of VAG data seen in VB files
	uint8_t ShiftAndFilter; //Don't know any of this
	uint8_t Flag; //Don't know any of this (looping is all i know)
	uint8_t ADPCM[14]; //Always 14 bytes.
};



struct VBFile { //I wanted to be able to return the vector files, so here we go!
	vector< vector<VBBlock> > sounds;
	string fileName;
};





//The following 3 structs are copied from https://problemkaputt.de/psxspx-cdrom-file-audio-sample-sets-vab-and-vh-vb-sony.htm

struct ProgramAttributes {
	uint8_t ToneAmount = 0; //Number of Tones in the Program (Yaroze: 4) (uh?)
	uint8_t Master_Volume = 0; //(Yaroze: 0..127)
	uint8_t priority = 0; //(Yaroze: N/A)
	uint8_t mode = 0; //(Yaroze: N/A)
	uint8_t Master_Pan = 0; //(Yaroze: 0..127)
	uint8_t Reserved0 = 0;
	short Attribute = 0; //(Yaroze: N/A)
	int Reserved1 = 0xFFFF;
	int Reserved2 = 0xFFFF;
};

struct ToneAttributes {
	uint8_t priority = 0; // 127 = highest, used for when the PCM buffer (?) gets overloaded, and it stops playing some sounds.
	uint8_t mode = 0; //0 = normal, 4 - reverb
	uint8_t vol = 0; //(Yaroze: 0..127)
	uint8_t panning = 0; //(Yaroze: 0..127)
	uint8_t center = 0; //Center note (in semitone units)
	uint8_t shift; // Centre note fine turning
	uint8_t min; //Note limit minimum value
	uint8_t max; //That's me! Note limit maximum value
	
	
	//These 2 are reverb? (Evidence: PSXDEV discord)
	uint8_t vibW = 0, vibT = 0;

	//What are these 2?
	uint8_t porW = 0, porT = 0;

	uint8_t PBmin = 0; //Max? value for downwards pitchbend  (Yaroze: 0..127)
	uint8_t PBmax = 0; //Max value for upwards pitchbend     (Yaroze: 0..127)
	
	uint8_t Reserved = 0;
	uint8_t Reserved1 = 0;

	short AttackDecay = 0;
	short ReleaseSustain = 0;
	short ProgramNumber = 0; // Program number that tone belongs to (Yaroze: 0..127)
	short VAG_ID = 0; //VAG Number
	long Reserved2 = 0;
};

struct VHFile {
	const char *id = "pBAV"; //This is the VAB file header but backwards??? How creative!
	int version = 7; //Version can range from 5 to 7. Also only uses 1 byte, but uses 4 instead.
	int VAB_ID = 0;
	int VAB_TotalFileSize = 0; //In bytes, or sum of .VH and .VB filesizes.
	unsigned short Reserved = 0xEEEE; //I don't know what this is, but it is always EE EE. Also, in a VAB, this is 4 bytes instead of 2, make sure to keep an eye out...
	short ProgramAmount = 0; //Why is this a short, if the maximum amount of programs can be 128?
	short ToneAmount = 0; // Subtracted by ?. Max 0x0800? aka max 0x10 per program
	short VagAmount = 0; //This is what we want! VAG = single audio file. Subtracted by ?. Max 0x00FE
	uint8_t Master_Volume = 0x7F;
	uint8_t Master_Pan = 0x40;

	uint8_t BankAttr1 = 0x00;
	uint8_t BankAttr2 = 0x00;
	int ReservedAgain = 0xFFFFFFFF;

	//This is for each VAG
	ProgramAttributes *prgAttr = nullptr;
	ToneAttributes *tnAttr = nullptr;

	short* VagSizes = nullptr; //16 bit sizes (div8) for VAG 0x00-0xFF
	

	//ADPCM data is also here, if we were a VAB!!!
};

VHFile PSX_ImportVH(string fileName);

void PSX_ShowVHData(VHFile* in);

VBFile PSX_ImportVB(string fileName);

void PSX_ExportVAGFromVBFile(VBFile* in);
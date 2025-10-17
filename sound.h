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





struct VBBlock { //Taken from CKDEV's comment in the Parappa Modding Discord
	uint8_t ShiftAndFilter; //Don't know any of this
	uint8_t Flag; //Don't know any of this (looping is all i know)
	uint8_t ADPCM[14];
};



struct VBFile {
	vector< vector<VBBlock> > sounds;
};





//The following 3 structs are copied from https://problemkaputt.de/psxspx-cdrom-file-audio-sample-sets-vab-and-vh-vb-sony.htm

struct ProgramAttributes {
	uint8_t ToneAmount = 0; //Number of Tones in the Program (Yaroze: 4) (uh?)
	uint8_t Master_Volume = 0; //(Yaroze: 0..127)
	uint8_t priority = 0; //(Yaroze: N/A)
	uint8_t mode = 0; //(Yaroze: N/A)
	uint8_t Master_Pan = 0; //(Yaroze: 0..127)
	uint8_t Reserved0;
	short Attribute; //(Yaroze: N/A)
	int Reserved1;
	int Reserved2;
};

struct ToneAttributes {
	uint8_t priority = 0; // 127 = highest
	uint8_t mode = 0; //0 = normal, 4 - reverb
	uint8_t vol = 0; //(Yaroze: 0..127)
	uint8_t panning = 0; //(Yaroze: 0..127)
	uint8_t center = 0; //Center note (in semitone units)
	uint8_t shift; // Centre note fine turning
	uint8_t min; //Note limit minimum value
	uint8_t max; //That's me! Note limit maximum value
	
	
	//These 2 are reverb?
	uint8_t vibW, vibT;

	//What are these 2?
	uint8_t porW, porT;

	uint8_t PBmin; //Max? value for downwards pitchbend  (Yaroze: 0..127)
	uint8_t PBmax; //Max value for upwards pitchbend     (Yaroze: 0..127)
	short Reserved1And2Combined;
	short AttackDecay;
	short ReleaseSustain;
	short ProgramNumber; // Program number that tone belongs to (Yaroze: 0..127)
	short VAG_ID; //VAG Number
	long reserved;
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
	ProgramAttributes *prgAttr = 0;
	ToneAttributes *tnAttr = 0;

	size_t* VagSizes = nullptr; //16 bit sizes (div8) for VAG 0x00-0xFF
	

	//ADPCM data is also here, if we were a VAB!!!
};

VHFile ImportVH(string fileName);

VBFile ImportVB(string fileName);

void ExportVAGFromVBFile(VBFile* in);
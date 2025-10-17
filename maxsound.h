#include "psxsound.h"
#include <vector>


struct Max_SoundFile {
	const char* id = "maxsoundfilehdr";
	char name[16] = "UnsetName\0";
	void SetName(const char* newName) {
		//if (sizeof(newName) > 16) ThrowError(ErrorCode::Invalid, "Name can only be less than 16 letters!");

		uint8_t i = 0;
		for (; i < sizeof(newName); i++) {
			name[i] = newName[i];
		}
		if (sizeof(newName) < 16) name[sizeof(newName) - 1] = '\0';

	}
	VBFile data;
};

void Max_ExportSoundFile() {

}
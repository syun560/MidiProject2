#pragma once
#include "global.h"

union UIntInByte {
	int i;
	char c[4];
};

struct HeaderChunk {
	UIntInByte type, size, format, track, division;
};

struct TrackChunk {
	UIntInByte type, size;
	char *data;
};

class smfIO {
	HeaderChunk header;
	TrackChunk* track;
	bool IsLittleEndian;
	void ConvertEndian(char* input, size_t s);
	void kakikomi(FILE*, UIntInByte& input, int size, bool convertFlag=true);
public:
	smfIO();
	int read(char* filename);
	int write(char* filename, char* data, int size);
	char* getTrackData(int track);
	int getTrackSize(int track);
	int getTrackNum();
	void draw();
};
#pragma once

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
	void ConvertEndian(UIntInByte& input, size_t s);
public:
	smfIO();
	int read(char* filename);
	int write(char* filename, char* data, int size);
	void draw();
};
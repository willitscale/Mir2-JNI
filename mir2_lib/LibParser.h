#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

class LibParser {

protected:

	// Not sure why I've enclosed these inside the LibParser, but
	// will I ever need them outside this scope? I doubt it?

	typedef struct lib_head {

		// Version 
		int32_t version;

		// Count
		int32_t count;
	} Header;

	typedef struct lib_shadow {

		// Dimensions
		int16_t width;
		int16_t height;

		// Position
		int16_t x;
		int16_t y;

		// Mask
		int32_t length;

	} Shadow;

	typedef struct lib_inf {

		// Dimensions
		int16_t width;
		int16_t height;

		// Position
		int16_t x;
		int16_t y;

		// Shadow
		int16_t shadowX;
		int16_t shadowY;

		// Shadow Mask
		int8_t shadow;

		// Length as array to deal with padding
		int8_t length[4];

		// Fuck you Data Structure Alignment
		int8_t paddingBit;

	} Info;

	typedef struct lib_data {

		// Offset
		int32_t offset;

		// Info 
		Info * info;

		// Mask
		Shadow * shadow;

	} Data;

	typedef struct lib_struct {

		// Header
		Header * header;

		// Data
		Data * datas;
	} Library;

public:

	// Some trinary for prosperity
	const typedef enum lib_ret {
		DONE_GOOFED = -1,
		SAFE_BRAV = 0,
		MAKIN_IT_RAIN = 1
	} RETURNS;

	LibParser();
	~LibParser();

	int8_t read(char *);

private:

	int8_t readHeader(FILE ** ptr, Library ** library);
	int8_t readData(FILE ** ptr, Library ** library);
	int8_t readShadowData(FILE ** ptr, Library ** library, int idx);
};
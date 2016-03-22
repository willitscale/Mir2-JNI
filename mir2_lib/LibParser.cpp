#include "LibParser.h"

LibParser::LibParser() {

}

LibParser::~LibParser() {

}

int8_t LibParser::read(char * file) {

	FILE * fp;

	fopen_s(&fp, file, "rb");

	if (NULL == fp) {
		return DONE_GOOFED;
	}

	Library * library = (Library *)malloc(sizeof(Library));

	readHeader(&fp, &library);
	readData(&fp, &library);

	fclose(fp);

	return SAFE_BRAV;
}

int8_t LibParser::readHeader(FILE ** ptr, Library ** library) {

	(*library)->header = (Header*)malloc(sizeof(Header));

	fread((*library)->header, sizeof(Header), 1, *ptr);

	(*library)->datas = (Data *)calloc(sizeof(Data), (*library)->header->count);

	return SAFE_BRAV;
}

int8_t LibParser::readData(FILE ** ptr, Library ** library) {

	long offset = ftell(*ptr);

	for (int i = 0; i < (*library)->header->count; i++) {

		// I feel like this is just asking for buffer overflow ... oh well
		fseek(*ptr, offset, SEEK_SET);

		fread(&(*library)->datas[i].offset, sizeof(int32_t), 1, *ptr);

		offset = ftell(*ptr);

		fseek(*ptr, (*library)->datas[i].offset, SEEK_SET);

		(*library)->datas[i].info = (Info*)malloc(sizeof(Info));

		// I should have really allocated each attribute individually,
		// but data structure alignment padding really fucked me off
		fread((*library)->datas[i].info, sizeof(Info), 1, *ptr);

		readShadowData(ptr, library, i);
	}

	return SAFE_BRAV;
}

int8_t LibParser::readShadowData(FILE ** ptr, Library ** library, int idx) {

	if (1 == (*library)->datas[idx].info->shadow >> 7) {

		// Because Fuck Data Structure Alignment (this is so unnecessary)
		int length = (*library)->datas[idx].info->length[3] << 24 & 0xFF000000 |
			(*library)->datas[idx].info->length[2] << 16 & 0xFF0000 |
			(*library)->datas[idx].info->length[1] << 8 & 0xFF00 |
			(*library)->datas[idx].info->length[0] & 0xFF;

		long offset = (*library)->datas[idx].offset +
			sizeof(Info) - 1 + length;

		fseek(*ptr, offset, SEEK_SET);

		(*library)->datas[idx].shadow = (Shadow*)malloc(sizeof(Shadow));

		fread((*library)->datas[idx].shadow, sizeof(Shadow), 1, *ptr);

		return MAKIN_IT_RAIN;
	}

	return SAFE_BRAV;
}
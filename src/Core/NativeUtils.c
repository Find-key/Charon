#include "PEBWalk.h"

bool CmpStrsIgnCase(const char* str1, const char* str2) {
	uint32_t i = 0;
	for (;; i++) {
		char a = str1[i];
		if ('a' <= a && a <= 'z') {
			a -= 32;
		}

		char b = str2[i];
		if ('a' <= b && b <= 'z') {
			b -= 32;
		}

		if (a != b) {
			return false;
		}

		if (a == 0) {
			return true;
		}
	}
}


bool CmpStrAndUniIgnCase(const char* str, const short* uni) {
	uint32_t i = 0;
	for (;; i++) {
		char a = str[i];
		if ('a' <= a && a <= 'z') {
			a -= 32;
		}

		short b = uni[i];
		if ('a' <= b && b <= 'z') {
			b -= 32;
		}

		if (a != b) {
			return false;
		}

		if (a == 0) {
			return true;
		}
	}
}

uint64_t Hash(const char* str) {
	uint64_t hash = 5381;
	
	for (uint32_t i = 0 ; str[i] != '\0' ; i++) {
		hash = ((hash << 5) + hash) + str[i];
	}

	return hash;
}


uint64_t HashUni(const short* str) {
	uint64_t hash = 5381;

	for (uint32_t i = 0; str[i] != '\0'; i++) {
		hash = ((hash << 5) + hash) + str[i];
	}

	return hash;
}
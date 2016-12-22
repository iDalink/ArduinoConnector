#include "tools.h"
#include "../kit/language/buildin.h"
#include "../kit/library/memory.h"
#include <string.h>

size_t StringCharCount(const char *string, char charactor) {
	size_t count = 0;
	while(*(string++)) {
		if (*string == charactor) {
			count ++;
		}
	}
	return count;
}

/**
 * ABCD0
 */
ssize_t SearchCharactorInString(const char *string, char charactor) {
	ssize_t location = -1;
	while(*string) {
		location ++;
		if (*string == charactor) {
			break;
		}
		string++;
	}
	return location;
}

int CharactorIsNumber(char charactor) {

	if (charactor > 47 && charactor < 58) {

		return 1;
	}
	return 0;
}

int pow(int x, int y) {
	int result = 1;
	for (int i = 0; i < y; i++) {
		result = result * x;
	}
	return result;
}

int atoi_ex(const char *string, size_t size) {
	int result = 0;
	for (int i = size - 1; i >= 0; i--) {
		result += (int)(*string - 48) * pow(10, i);
		string++;
	}
	return result;
}

int MatchedPin(const char *string, size_t size) {
	// PIN1			PIN10		A1		A12
	if (size == 4 && *string == 'P') {
		return atoi_ex(string + 3, 1);
	}
	if (size == 5 && *string == 'P') {
		return atoi_ex(string + 3, 2);
	}
	if (size== 2 && *string == 'A') {
		return atoi_ex(string + 1, 1);
	}
	if (size== 3 && *string == 'A') {
		return atoi_ex(string + 1, 2);
	}
	return -1;
}
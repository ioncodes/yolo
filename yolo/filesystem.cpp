#include "filesystem.h"
#include <cstddef>
#include <nativefiledialog/nfd.h>
#include <cstdio>
#include <cstdlib>

char* Filesystem::RequestFile(char *extensions)
{
	nfdchar_t *outPath = NULL;
	nfdresult_t result = NFD_OpenDialog(extensions, NULL, &outPath);

	if (result == NFD_OKAY)
	{
		return (char*)outPath;
	}
	return NULL;
}

char* Filesystem::ReadFile(char* path)
{
	FILE *f;
	fopen_s(&f, path, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	rewind(f);

	char *string = (char*)malloc(fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);

	string[fsize] = 0;

	return string;
}

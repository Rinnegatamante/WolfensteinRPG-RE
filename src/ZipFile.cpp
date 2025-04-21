#include <stdexcept>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

#include "ZipFile.h"
#include "SDLGL.h"

#ifdef __vita__
#include <vitasdk.h>
#endif

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

void Error(const char* fmt, ...)
{
	char errMsg[256];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(errMsg, sizeof(errMsg), fmt, ap);
	va_end(ap);

	printf("%s", errMsg);

	const SDL_MessageBoxButtonData buttons[] = {
		{ /* .flags, .buttonid, .text */        0, 0, "Ok" },
	};
	const SDL_MessageBoxColorScheme colorScheme = {
		{ /* .colors (.r, .g, .b) */
			/* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
			{ 255,   0,   0 },
			/* [SDL_MESSAGEBOX_COLOR_TEXT] */
			{   0, 255,   0 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
			{ 255, 255,   0 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
			{   0,   0, 255 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
			{ 255,   0, 255 }
		}
	};
	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_ERROR, /* .flags */
		NULL, /* .window */
		"Wolfenstein RPG Error", /* .title */
		errMsg, /* .message */
		SDL_arraysize(buttons), /* .numbuttons */
		buttons, /* .buttons */
		&colorScheme /* .colorScheme */
	};

	SDL_ShowMessageBox(&messageboxdata, NULL);
	exit(0);
}

static inline __attribute__((always_inline)) void* zip_alloc(void* ctx, unsigned int items, unsigned int size)
{
	return malloc(items * size);
}

static inline __attribute__((always_inline)) void zip_free(void* ctx, void* ptr)
{
	free(ptr);
}

static inline __attribute__((always_inline)) int16_t _ReadShort(FILE* fp)
{
	int16_t sData = 0;

	if (fp) {
		fread(&sData, sizeof(int16_t), 1, fp);
	}

	return sData;
}

static inline __attribute__((always_inline)) int32_t _ReadInt(FILE* fp)
{
	int32_t iData = 0;

	if (fp) {
		fread(&iData, sizeof(int32_t), 1, fp);
	}

	return iData;
}

ZipFile::ZipFile() {
	memset(this, 0, sizeof(ZipFile));
}

ZipFile::~ZipFile() {
}

#ifdef __vita__
void ZipFile::scanDir(const char *name) {
	int dir = sceIoDopen(name);
	if (dir < 0) {
		sceClibPrintf("Cannot find %s\n", name);
		abort();
	}
	
	//sceClibPrintf("Scanning %s\n", name);
	SceIoDirent fd;
	while (sceIoDread(dir, &fd) > 0) {
		if (SCE_S_ISDIR(fd.d_stat.st_mode)) {
			char subpath[512];
			sprintf(subpath, "%s/%s", name, fd.d_name);
			scanDir(subpath);
		} else {
			sprintf(entry[entry_count].name, "%s/%s", name, fd.d_name);
			entry[entry_count].usize = fd.d_stat.st_size;
			entry[entry_count].offset = entry_count;
			entry_count++;
		}
	}
	
	sceIoDclose(dir);
}
#endif

void ZipFile::findAndReadZipDir(int startoffset) {
#ifdef __vita__
	scanDir(this->path);
	int dir = sceIoDopen(this->path);
	if (dir < 0) {
		sceClibPrintf("Cannot find %s\n", this->path);
		abort();
	}
#else
	int sig, offset, count;
	int namesize, metasize, commentsize;
	int i;

	fseek(this->file, startoffset, SEEK_SET);

	sig = _ReadInt(this->file);
	if (sig != ZIP_END_OF_CENTRAL_DIRECTORY_SIG) {
		Error("wrong zip end of central directory signature (0x%x)", sig);
	}

	_ReadShort(this->file); // this disk
	_ReadShort(this->file); // start disk
	_ReadShort(this->file); // entries in this disk
	count = _ReadShort(this->file); // entries in central directory disk
	_ReadInt(this->file); // size of central directory
	offset = _ReadInt(this->file); // offset to central directory

	this->entry = (zip_entry_t*)calloc(count, sizeof(zip_entry_t));
	this->entry_count = count;

	fseek(this->file, offset, SEEK_SET);

	for (i = 0; i < count; i++)
	{
		zip_entry_t* entry = this->entry + i;

		sig = _ReadInt(this->file);
		if (sig != ZIP_CENTRAL_DIRECTORY_SIG) {
			Error("wrong zip central directory signature (0x%x)", sig);
		}

		_ReadShort(this->file); // version made by
		_ReadShort(this->file); // version to extract
		_ReadShort(this->file); // general
		_ReadShort(this->file); // method
		_ReadShort(this->file); // last mod file time
		_ReadShort(this->file); // last mod file date
		_ReadInt(this->file); // crc-32
		entry->csize = _ReadInt(this->file); // csize
		entry->usize = _ReadInt(this->file); // usize
		namesize = _ReadShort(this->file); // namesize
		metasize = _ReadShort(this->file); // metasize
		commentsize = _ReadShort(this->file); // commentsize
		_ReadShort(this->file); // disk number start
		_ReadShort(this->file); // int file atts
		_ReadInt(this->file); // ext file atts
		entry->offset = _ReadInt(this->file); // offset

		entry->name = (char*)malloc(namesize + 1);
		fread(entry->name, sizeof(char), namesize, this->file);
		entry->name[namesize] = 0;

		fseek(this->file, metasize, SEEK_CUR);
		fseek(this->file, commentsize, SEEK_CUR);
	}
#endif
}

void ZipFile::openZipFile(const char* name) {
#ifdef __vita__
	//sceClibPrintf("Opening %s\n", name);
	memcpy(this->path, name, strlen(name) - 4);
	this->path[strlen(name) - 4] = 0;
	//sceClibPrintf("Translated into %s\n", this->path);
	findAndReadZipDir(0);
#else
	uint8_t buf[512];
	int filesize, back, maxback;
	int i, n;

	this->file = fopen(name, "rb");
	if (this->file == NULL) {
		Error("openZipFile: cannot open file %s\n", name);
	}

	fseek(this->file, 0, SEEK_END);
	filesize = (int)ftell(this->file);
	fseek(this->file, 0, SEEK_SET);

	maxback = MIN(filesize, 0xFFFF + sizeof(buf));
	back = MIN(maxback, sizeof(buf));

	while (back < maxback)
	{
		fseek(this->file, filesize - back, SEEK_SET);
		n = sizeof(buf);
		fread(buf, sizeof(uint8_t), n, this->file);
		for (i = n - 4; i > 0; i--)
		{
			if (!memcmp(buf + i, "PK\5\6", 4)) {
				findAndReadZipDir(filesize - back + i);
				return;
			}
		}
		back += sizeof(buf) - 4;
	}

	Error("cannot find end of central directory\n");
#endif
}

void ZipFile::closeZipFile() {
#ifndef __vita__
	if (this) {
		if (this->entry) {
			free(this->entry);
			this->entry = nullptr;
		}

		if (this->file) {
			fclose(this->file);
			this->file = nullptr;
		}
	}
#endif
}

uint8_t* ZipFile::readZipFileEntry(const char* name, int* sizep) {
#ifdef __vita__
	char fullpath[512];
	sprintf(fullpath, "%s/%s", this->path, name);
	//sceClibPrintf("readZipFileEntry %s\n", fullpath);
	FILE *f = fopen(fullpath, "rb");
	if (!f) {
		sceClibPrintf("Cannot open %s\n", fullpath);
		abort();
	}
	fseek(f, 0, SEEK_END);
	*sizep = ftell(f);
	fseek(f, 0, SEEK_SET);
	uint8_t *res = (uint8_t *)std::malloc(*sizep);
	fread(res, 1, *sizep, f);
	fclose(f);
	//sceClibPrintf("Done\n");
	return res;
#else
	zip_entry_t* entry = nullptr;
	int i, sig, general, method, namelength, extralength;
	uint8_t* cdata;
	int code;

	for (i = 0; i < this->entry_count; i++)
	{
		zip_entry_t* entryTmp = this->entry + i;
		if (!SDL_strcasecmp(name, entryTmp->name)) {
			entry = this->entry + i;
			break;
		}
	}

	if (entry == NULL) {
		Error("did not find the %s file in the zip file", name);
	}

	fseek(this->file, entry->offset, SEEK_SET);

	sig = _ReadInt(this->file);
	if (sig != ZIP_LOCAL_FILE_SIG) {
		Error("wrong zip local file signature (0x%x)", sig);
	}

	_ReadShort(this->file); // version
	general = _ReadShort(this->file); // general
	if (general & ZIP_ENCRYPTED_FLAG) {
		Error("zipfile content is encrypted");
	}

	method = _ReadShort(this->file); // method
	_ReadShort(this->file); // file time
	_ReadShort(this->file); // file date
	_ReadInt(this->file); // crc-32
	_ReadInt(this->file); // csize
	_ReadInt(this->file); // usize
	namelength = _ReadShort(this->file); // namelength
	extralength = _ReadShort(this->file); // extralength

	fseek(this->file, namelength + extralength, SEEK_CUR);

	cdata = (uint8_t*) malloc(entry->csize);
	fread(cdata, sizeof(uint8_t), entry->csize, this->file);

	if (method == 0)
	{
		*sizep = entry->usize;
		return cdata;
	}
	else if (method == 8)
	{
		uint8_t* udata = (uint8_t*) malloc(entry->usize);
		z_stream stream;

		memset(&stream, 0, sizeof stream);
		stream.zalloc = zip_alloc;
		stream.zfree = zip_free;
		stream.opaque = Z_NULL;
		stream.next_in = cdata;
		stream.avail_in = entry->csize;
		stream.next_out = udata;
		stream.avail_out = entry->usize;

		code = inflateInit2(&stream, -15);
		if (code != Z_OK) {
			Error("zlib inflateInit2 error: %s", stream.msg);
		}

		code = inflate(&stream, Z_FINISH);
		if (code != Z_STREAM_END) {
			inflateEnd(&stream);
			Error("zlib inflate error: %s", stream.msg);
		}

		code = inflateEnd(&stream);
		if (code != Z_OK) {
			inflateEnd(&stream);
			Error("zlib inflateEnd error: %s", stream.msg);
		}

		free(cdata);

		*sizep = entry->usize;
		return udata;
	}
	else {
		Error("unknown zip method: %d", method);
	}

	return nullptr;
#endif
}
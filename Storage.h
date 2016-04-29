
#include <SPI.h>
#include <SdFat.h>

#ifndef nullPtr
#define nullPtr 0
#endif


class Storage {
	public:
	
	void* stuff=nullPtr;
	uint32_t size=0;
	char* fileName=(char*)"";
    SdFile myFile;

	Storage();
	~Storage();
	void persist(void*someStuff,uint32_t aSize,char*aFileName);
	void changeStuff(void*someStuff,uint32_t aSize);
	void changeFile(char*aFileName);
	int save();
	int load();
	
};
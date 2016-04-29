
#include "Storage.h"

Storage::Storage() {
	
}

Storage::~Storage() {
	if(myFile.isOpen()) myFile.close();
}

void Storage::persist(void*someStuff,uint32_t aSize,char*aFileName) {
	changeFile(aFileName);
	changeStuff(someStuff,aSize);
}

void Storage::changeStuff(void*someStuff,uint32_t aSize) {
	stuff=someStuff;
	size=aSize;
}

void Storage::changeFile(char*aFileName) {
	fileName=aFileName;
}


int Storage::save() {
  uint32_t wroteSize;
  SerialUSB.print(F("Ready to save file "));
  SerialUSB.println(fileName);
  
   if(!(myFile.open(fileName, O_RDWR | O_CREAT))) { //  | O_TRUNC
    SerialUSB.print(F("Error opening file "));
    SerialUSB.print(fileName);
    SerialUSB.println(F(" for writing."));
    return -1;
  }
  uint32_t fileSize = myFile.fileSize(); // old size, check if same?
  myFile.seekSet(0);
  if((wroteSize=myFile.write(stuff, size))!=size) {
  	myFile.close();
    SerialUSB.print(F("Error writing to file "));
    SerialUSB.print(fileName);
    SerialUSB.println(F("."));
    SerialUSB.print(F("Wrote only "));
    SerialUSB.print(wroteSize);
    SerialUSB.print(F(" bytes of the "));
    SerialUSB.print(size);
    SerialUSB.println(F(" bytes needed."));
  	return -2;
  }
  if(!myFile.sync()) {
    SerialUSB.print(F("Error syncing file "));
    SerialUSB.println(fileName);
    return -3;
  }
  if(!myFile.close()) {
    SerialUSB.print(F("Error closing file "));
    SerialUSB.println(fileName);
    // http://stackoverflow.com/questions/24477740/what-are-the-reasons-to-check-for-error-on-close/24479705#24479705
    // yea right, but.. yea, right.
    return -4;
  }
  return 0;
}

int Storage::load() {
  SerialUSB.print(F("Ready to load from file "));
  SerialUSB.println(fileName);
   if(!(myFile.open(fileName, O_READ))) {
    SerialUSB.print(F("Error opening file "));
    SerialUSB.print(fileName);
    SerialUSB.println(F(" for reading."));
    return -1;
  }
  uint32_t n = myFile.read(stuff, size);
  if (n < 0) {
    SerialUSB.print(F("Error reading "));
    SerialUSB.print(size);
    SerialUSB.print(F(" bytes from file "));
    SerialUSB.println(fileName);
    return -2;
  }
  if (n < size) {
    SerialUSB.print(F("Error reading file "));
    SerialUSB.println(fileName);
    SerialUSB.print(F(", only "));
    SerialUSB.print(n);
    SerialUSB.print(F(" bytes read, of "));
    SerialUSB.print(size);
    return -3;
  }
  if(!myFile.close()) {
    SerialUSB.print(F("Error closing file "));
    SerialUSB.println(fileName);
    // http://stackoverflow.com/questions/24477740/what-are-the-reasons-to-check-for-error-on-close/24479705#24479705
    // yea right, but.. yea, right.
    return -4;
  }
  return 0;
}

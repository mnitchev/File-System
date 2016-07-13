#include "AbstractFile.h"

AbstractFile::AbstractFile(){
	FSysNumber = 0;
	sizeInBytes = 0;
	fileType = NULL_FILE;
}

AbstractFile::AbstractFile( string parentFSysPath, int FSysNumber, FILE_TYPE fileType, string fileName){
	if (parentFSysPath != "/") {
		this->FSysPath = parentFSysPath + '/' + fileName;
	}
	else {
		this->FSysPath = parentFSysPath + fileName;
	}
	this->FSysNumber = FSysNumber;
	this->fileType = fileType;
	this->sizeInBytes = 0;
	this->lastModified = getTime();
	this->lastAccessed = getTime();
	this->metaLastModified = getTime();
	this->fileName = fileName;
}

string AbstractFile::getTime() const {
	const time_t now = time(0);
	return string(ctime(&now));
}

string AbstractFile::fileTypeToString(FILE_TYPE t){
	int value = (int)t;
	switch (value) {
	case 1: return string("text file"); break;
	case 2: return string("binary file"); break;
	case 3: return string("directory"); break;
	case 4: return string("sym-link"); break;
	default: return string("null file");
	}
	return string("null file");
}

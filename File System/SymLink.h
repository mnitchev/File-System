#ifndef SYMLINK_H_
#define SYMLINK_H_

#include "AbstractFile.h"
#include "TextFile.h"
#include "Directory.h"

class SymLink : public AbstractFile{
	TextFile* file;
public:
	SymLink(TextFile* file, int FSysNumber, string parentPath);

	/*
	* Селектор за файла към който сочи линка.
	*/
	TextFile* getLinkedFile()  { return file; }
	
	/*
	* Извежда метаданните на файла към който сочи.
	*/
	void printMeta() { cout << " Link : " << getFileName() << " linking to : " << endl; file->printMeta(); }
	void deleteFile();
};

#endif

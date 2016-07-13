#ifndef ABSTRACTFILE_H_
#define ABSTRACTFILE_H_

#include <iostream>
#include <string>
#include <ctime>
#include <time.h>
#include <fstream>
#include <queue>
#include <list>
#include <stdlib.h>

using namespace std;

#define ROOT 1

enum FILE_TYPE { NULL_FILE = 0, TXT_FILE = 1, BIN_FILE = 2, DIR = 3, SYM_LINK = 4 };

/*
* Базов виртуален клас за файл. Съдържа всички метаданни.
*/
class AbstractFile {
	string FSysPath;
	int FSysNumber;
	int sizeInBytes;
	string lastAccessed;
	string lastModified;
	string metaLastModified;
	FILE_TYPE fileType;
	string fileName;
public:
	/*
	* Конструктори и деструктори.
	*/
	AbstractFile();
	AbstractFile(string FSysPath, int FSysNumber, FILE_TYPE fileType, string fileName);
	virtual ~AbstractFile() {}
	
	/*
	* Селектори за метаданните.
	*/
	string getFSysPath() const { return FSysPath; }
	int getFSysNumber() const { return FSysNumber; }
	int getSize() const { return sizeInBytes; }
	string getLastAccess() const { return lastAccessed; }
	string getLastModified() const { return lastModified; }
	string getFileName() const { return fileName; }
	string getMetaLastModified() const { return metaLastModified; }
	string getTime() const;


	/*
	* Мутатори за метаданните
 	*/
	void updateMetaLastModified() { metaLastModified = getTime(); }
	void updateLastModified() { lastModified = getTime(); }
	void updateLastAccess() { lastAccessed = getTime(); }

	/*
	* Наследява промяна в големината на някой файл.
	*/
	void inheritSize(int bytes) { sizeInBytes += bytes; }

	/*
	* Функции за установяване на типа на файла.
	*/
	bool isType(FILE_TYPE ftype) { return fileType == ftype; }
	FILE_TYPE tellType() const { return fileType; }

	/*
	*  Превръща тип в string.
	*/
	string fileTypeToString(FILE_TYPE t);

	/*
	* Интерфейс функции.
	*/
	virtual void printMeta() = 0;
	virtual void deleteFile() = 0;

};

#endif

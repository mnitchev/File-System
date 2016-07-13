#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "Directory.h"
#include "TextFile.h"
#include "SymLink.h"

class FileSystem {
	Directory* root;
	Directory NullFile;
	int nextNumber;
public:
	/*
	* Конструктор и деструктор.
	* Деструктора изтрива root директорията и всички те ѝ наследници.
	*/
	FileSystem();
	~FileSystem() { root->deleteFile(); }

	/*
	* Селектори за root директорията и произволен файл или директория
	* по пълен път във файловата система.
	*/
	Directory* getRoot() const { return root; }
	Directory* getDir(string address);
	TextFile* getFile(string fileAddress);


	/*
	* Дава следващия пореден номер.
	*/
	int getNextNumber() { return nextNumber++; }
};


#endif // !FILESYSTEM_H_


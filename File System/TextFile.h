#ifndef TEXTFILE_H_
#define TEXTFILE_H_

#include "AbstractFile.h"

class TextFile : public AbstractFile {
	string data;
	list<AbstractFile*> links;
public:
	/*
	* Конструктори и деструктор.
	*/
	TextFile( string parentFSysPath, int FSysNumber, string data, string fileName);
	TextFile(TextFile&);
	~TextFile() {}
	
	/*
	* Селектор за данните във файла.
	*/
	string getData() const { return data; }

	/*
	* Създаване на нов линк към файл.
	*/
	void addLink(AbstractFile* link) { links.push_back(link); }

	/*
	* Конкатенация на данните.
	*/
	void concatData(string newData) { data += newData; }

	/*
	* Имплементация на виртуалните функции.
	*/
	void printMeta();
	void deleteFile();
};


#endif

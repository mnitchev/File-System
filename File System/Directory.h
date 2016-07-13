#ifndef DIRECTORY_H_
#define DIRECTORY_H_


#include "AbstractFile.h"
#include "TextFile.h"

	
class Directory : public AbstractFile{
	Directory* parent;
	list<AbstractFile*> children;
public:

	friend class SymLink;

	/*
	* Конструктори и деструктор.
	*/
	Directory();
	Directory( string parentFSysPath, int FSysNumber, string fileName, Directory* parent);
	~Directory(){}

	/*
	* Overlode на същата функция в AbstractFile.
	*/
	void inheritSize(int inheritedSize);

	/*
	* Селектор за родителската директория.
	*/
	Directory* getParent() { return parent; }
	
	/*
	* Добавя файл към списъка с файлове наследници.
	*/
	void addChild(AbstractFile*);
	
	/*
	* Функции за унищожаване файла и наследниците му.
	*/
	void deleteFile();
	void removeFile(TextFile* file) { file->deleteFile(); children.remove(file); delete[] file; }
	void removeLink(SymLink* link);
	
	/*
	* Функции за намиране на директория или файл по пълен или релативен път.
	*/
	Directory* findDirByName(string dirName);
	Directory* findDirRelative(string relativePath);
	TextFile* findFileByName(string fileName);
	TextFile* findFileRelative(string filePath);

	/*
	* Проверява дали подаденото име е свободно в тази директория.
	*/
	bool nameAvailable(string name);

	/*
	* Принтира наследниците на директорията.
	*/
	void printContent();

	/*
	* Принтира метаданните.
	*/
	void printMeta();
};

#endif

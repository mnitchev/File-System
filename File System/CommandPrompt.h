#ifndef COMMANDPROMPT_H_
#define COMMANDPROMPT_H_

#include "FileSystem.h"
#include <stack>

class CommandPrompt {
	FileSystem fs;
	Directory* currentDir;
	bool done;
public:
	CommandPrompt();
	~CommandPrompt() {}

	/*
	* Главния цикъл на програмта.
	*/
	void work();

	/*
	* Решава коя операция се иска и я извиква с подадените параметри.
	*/
	void executeOP(string operation);
private:
	/*
	* Функции дефиниращи операциите.
	*/
	void printCurrentPath();
	void setCurrentPath(string);
	void getDirectoryContent(string);
	void concatenateFiles(string);
	void copyFileInDirectory(string);
	void deleteFile(string);
	void makeDirectory(string);
	void deleteDirectory(string);
	void createLinksInDirectory(string);
	void getFileMetadata(string);

	/*
	* Помощтни функции за запазване на файл, детерминиране и намиране на директория или файл,
	* извличане на директории от string и създаване на вложени директории.
	*/
	void saveConcatenatedFile(string, string);
	Directory* determinePathAndGetDir(string path);
	TextFile* determinePathAndGetFile(string path);
	queue<string> getPaths(string parameters);
	Directory * createNestedDirectories(string path);


};



#endif

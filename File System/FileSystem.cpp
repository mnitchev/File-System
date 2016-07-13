#include "FileSystem.h"

FileSystem::FileSystem() : NullFile(){
	nextNumber = 1;
	root = new Directory( string(), nextNumber++, string(), NULL);
}

/*
* Намира директория по пълен адрес във файловата система.
* @return - адреса на търсената директория, NULL ако не съществува.
*/
Directory* FileSystem::getDir(string address){
	/*
	* Ако името е само "/" връща root директорията.
	*/
	if (address == "/") {
		return root;
	}

	/*
	* Търсим по dirName в директорията като започнем от root.
	*/
	Directory* result = root;
	string dirName;
	for (int i = 1; i <= address.size() && result != NULL; i++) {
		/*
		* Ако текущия символ в адреса е '/' или см достигнали края на адреса.
		* търсим по име в текущата директория. (Ако името е ".." връщаме назад).
		*/
		if (address[i] == '/' || i == address.size()) {
			if (dirName == "..") {
				if(result->getParent() != NULL)
				result = result->getParent();
			}
			else
				result = result->findDirByName(dirName);
			dirName = "";
		}
		else {
			dirName += address[i];
		}
	}
	return result;
}

/*
* Намира файл по пълен път във файловата система.
* @return - адреса на файла, NULL ако не съществува.
*/
TextFile* FileSystem::getFile(string fileAddress) {
	string fileDir, fileName;
	int pos = fileAddress.rfind('/') + 1;
	while (pos < fileAddress.size()) {
		fileName = fileAddress[pos++];
	}
	pos = fileAddress.rfind('/');
	fileDir = fileAddress.erase(pos, fileAddress.size() - 1);
	Directory* dir = this->getDir(fileDir);
	if (dir == NULL) {
		return NULL;
	}
	return dir->findFileByName(fileName);
}

/*
void FileSystem::addDirectory(queue<string> paths){
	string currentPath;
	string tempDirName;
	Directory* currentDir = root;
	Directory* previousDir;
	while (!paths.empty()) {
		currentPath = paths.front();
		for (int i = 0; i <= currentPath.size(); i++){
			if (currentPath[i] == '/' || i == currentPath.size()) {
				previousDir = currentDir;
				currentDir = currentDir->findDirByName(tempDirName);
				if (currentDir->isType(NULL_FILE)) {
					previousDir->addChild(new Directory(winPath,
						previousDir->getFSysPath(), 
						this->getNextNumber(),
						tempDirName,
						root));
					currentDir = previousDir->findDirByName(tempDirName);
				}
				tempDirName = "";
			}
			else {
				tempDirName += currentPath[i];
			}
		}
	}

}
*/
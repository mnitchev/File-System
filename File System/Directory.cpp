#include "Directory.h"
#include "SymLink.h"


Directory::Directory() : parent(NULL) {
}

Directory::Directory( string parentFSysPath, int FSysNumber, string fileName, Directory* parent) :
AbstractFile( parentFSysPath,FSysNumber, DIR, fileName){
	this->parent = parent;
}

/*
* Извиква същата функция в AbstractFile частта и предава наследството
* към родителската директория.
*/
void Directory::inheritSize(int inheritedSize){
	AbstractFile::inheritSize(inheritedSize);
	if (getFSysNumber() == ROOT) {
		return;
	}
	this->updateMetaLastModified();
	parent->inheritSize(inheritedSize);
}

/*
* Добавя указател към AbstractFile в списъка от наследници.
*/
void Directory::addChild(AbstractFile* file){
	if (nameAvailable(file->getFileName())) {
		children.push_back(file);
		this->updateLastModified();
	}
	else {
		cerr << "Name not available!";
	}
}

/*
* Маха линк от списъка на наследници и го трие.
*/
void Directory::removeLink(SymLink* link) { 
	link->deleteFile();
	children.remove(link); 
	delete[] link; 
}

/*
* Изтрива файла и всичките му наследници
*/
void Directory::deleteFile(){
	/*
	* Итераторът обхожда списъка.
	* Ако намери файл или линк извиква remove функцията в this.
	* Ако не значи е директория и извиква deleteFile() функцията
	* за да се изтрият и нейните налсдници.
	* Накрая директорията се изтрива от списъка на родителската и освобождава паметта.
	*/
	if (!children.empty()) {
		auto it = children.begin();
		while (!children.empty()) {
			if ((*it)->isType(TXT_FILE)){
				this->removeFile((TextFile*)*it);
			}
			else if((*it)->isType(SYM_LINK)){
				this->removeLink((SymLink*)*it);
			}
			else {
				(*it)->deleteFile();
			}
			it = children.begin();
		}
	}
	if(parent != NULL)
		parent->children.remove(this);
	delete[] this;
}

/*
* Намира директория по име измежду наследниците.
* @return - адреса на директорията, NULL ако няма такава.
*/
Directory* Directory::findDirByName(string dirName){
	auto it = children.begin();
	Directory* result = NULL;
	while (it != children.end() && result == NULL) {
		if ((*it)->getFileName() == dirName && (*it)->isType(DIR))
			result = (Directory*)*it;
		it++;
	}
	return result;
}

/*
* Намира директория по относителен път спрямо тази.
* @return - адреса на директорията, NULL ако няма такава.
*/
Directory* Directory::findDirRelative(string relativePath){
	string tempDirName;
	Directory* currentDir = this;
	for (int i = 0; i <= relativePath.size(); i++) {
		if (relativePath.c_str()[i] == '/' || i == relativePath.size()) {
			if (tempDirName == "..") {
				if (currentDir->parent != NULL)
					currentDir = currentDir->parent;
			}
			else
				currentDir = currentDir->findDirByName(tempDirName);
			tempDirName = "";
			if (currentDir == NULL) {
				cerr << "Directory not found!" << endl;
				return NULL;
			}
		}
		else {
			tempDirName += relativePath.c_str()[i];
		}
	}
	return currentDir;
}

/*
* Намира файл по име измежду наследниците.
* @return - адреса на файла, NULL ако не съществува.
*/
TextFile* Directory::findFileByName(string fileName) {
	TextFile* result = NULL;
	if (fileName.rfind('/') == string::npos) {
		auto it = children.begin();
		while (it != children.end()) {
			if ((*it)->getFileName() == string(fileName) && ((*it)->isType(TXT_FILE) || (*it)->isType(SYM_LINK))) {
				/*
				* Ако намерим линк към файл връщаме файла към който сочи.
				*/
				if ((*it)->isType(SYM_LINK)) {
					SymLink* temp = (SymLink*)(*it);
					if (temp->getLinkedFile() == NULL) {
						return NULL;
					}
					result = temp->getLinkedFile();
				}
				else
					result = (TextFile*)*it;
			}
			it++;
		}
	}
	return result;
}

/*
* Намира файл по относителен път спрямо тази директория.
* @return - адреса на файла, NULL ако няма такъв.
*/
TextFile* Directory::findFileRelative(string filePath) {
	string fileName;
	int pos = filePath.rfind('/') + 1;
	while (pos < filePath.size()) {
		fileName += filePath[pos++];
	}
	pos = filePath.rfind('/');
	filePath.erase(pos, filePath.size());
	Directory* tempDir = this->findDirRelative(filePath);
	if (tempDir == NULL)
		return NULL;
	return tempDir->findFileByName(fileName);
}

/*
* Обхожда наследниците и проверява дали името е заето.
* @return - true ако е свободно, false ако не е.
*/
bool Directory::nameAvailable(string name){
	bool result = true;
	auto it = children.begin();
	while (it != children.end() && result) {
		if ((*it)->getFileName() == name)
			result = false;
		it++;
	}
	return result;
}

/*
* Извежда имената на наследниците на един ред, след това принтира endl.
*/
void Directory::printContent(){
	auto it = children.begin();
	while (it != children.end()) {
		cout << (*it)->getFileName() << " ";
		it++;
	}	
	cout << endl;
}

/*
* Принтира метаданните.
*/
void Directory::printMeta(){
	cout << "File name : " << getFileName() << endl
		<< "File type : " << fileTypeToString(tellType()) << endl
		<< "Size : " << getSize() << endl
		<< "Last accessed : " << getLastAccess() << endl
		<< "Last modified : " << getLastModified() << endl
		<< "Metadata last modified : " << getMetaLastModified() << endl;
}

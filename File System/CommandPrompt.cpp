#include "CommandPrompt.h"

CommandPrompt::CommandPrompt() : fs(){
	currentDir = fs.getRoot();
	done = false;
}

void CommandPrompt::work(){
	char line[1000];
	string temp;
	cout << "$ ";
	while (!done) {
		cin.getline(line, 1000);
		temp = line;
		this->executeOP(temp);
		cout << "$ ";
	}
}

/*
* Разделя операцията от параметрите и извиква съответната функция.
*/
void CommandPrompt::executeOP(string operation){
	string opcode;
	string parameters;
	bool flag = false;
	for (int i = 0; i < operation.size(); i++) {
		if (!flag) {
			if (operation[i] == ' ')
				flag = true;
			else
				opcode += operation[i];
		}
		else {
			parameters += operation[i];
		}
	}
	if (opcode == string("pwd")) {
		this->printCurrentPath();
		return;
	}
	if (opcode == string("cd")) {
		this->setCurrentPath(parameters);
		return;
	}
	if (opcode == string("ls")) {
		this->getDirectoryContent(parameters);
		return;
	}
	if (opcode == string("cat")) {
		this->concatenateFiles(parameters);
		return;
	}
	if (opcode == string("cp")) {
		this->copyFileInDirectory(parameters);
		return;
	}
	if (opcode == string("rm")) {
		this->deleteFile(parameters);
		return;
	}
	if (opcode == string("mkdir")) {
		this->makeDirectory(parameters);
		return;
	}
	if (opcode == string("rmdir")) {
		this->deleteDirectory(parameters);
		return;
	}
	if (opcode == string("ln")) {
		this->createLinksInDirectory(parameters);
		return;
	}
	if (opcode == string("stat")) {
		this->getFileMetadata(parameters);
		return;
	}
	if (opcode == string("quit")) {
		done = true;
		return;
	}
	if (opcode == string("clear")) {
		system("cls");
		return;
	}
	cerr << "No such operation!" << endl;
	return;
}

/*
* Принтира текущия път.
*/
void CommandPrompt::printCurrentPath(){
	cout << currentDir->getFSysPath() << endl;
}

/*
* Намира подадения път и променя currentPath.
*/
void CommandPrompt::setCurrentPath(string path){
	if (path == "..") {
		if (currentDir->getParent() != NULL)
			currentDir = currentDir->getParent();
		return;
	}
	Directory* temp = determinePathAndGetDir(path);
	if (temp == NULL || temp->isType(NULL_FILE)) {
		cerr << "No such directory found!" << endl;
		return;
	}
	currentDir = temp;
	currentDir->updateLastAccess();
}

/*
* Решава пътя и извиква printContent() na namerenata директория
*/
void CommandPrompt::getDirectoryContent(string path){
	if (path.size() == 0) {
		currentDir->printContent();
		return;
	}
	if (path.find_first_of('/') == string::npos) {
		Directory* temp;
		temp = currentDir->findDirByName(path);
		if (temp == NULL || temp->isType(NULL_FILE)) {
			cerr << "Directory not found!" << endl;
			return;
		}
		temp->printContent();
	}
	else {
		Directory* temp;
		temp = fs.getDir(path);
		if (temp != NULL)
			temp->printContent();
		else
			cerr << "Directory not found!" << endl;
	}
}

/*
* Операция за конкатенация, извеждане и въвеждане на файл.
*/
void CommandPrompt::concatenateFiles(string filePaths){
	queue<string> paths;
	string outputFilePath;
	string temp;
	for (int i = 0; i <= filePaths.size(); i++) {
		/*
		* Ако сме достигнали '>' записва адреса на параметър ако е останал незаписан
		* и започва четенето на outputFilePath.
		*/
		if (filePaths[i] == '>' || i == filePaths.size()) {
			if(!temp.empty())
				paths.push(temp);
			if (i != filePaths.size()) {
				i += 2;
				while (i < filePaths.size()) {
					outputFilePath += filePaths[i++];
				}
			}
		}
		else if(filePaths[i] == ' ') {
			paths.push(temp);
			temp = "";
		}
		else {
			temp += filePaths[i];
		}
	}

	/*
	* Конкатенира данните на всички файлове в опашката и извиква
	* saveConcatenatedFile с параметри данните и outputFilePath.
	*/
	string resultData;
	TextFile* currentFile;
	while (!paths.empty()) {
		temp = paths.front();
		currentFile = this->determinePathAndGetFile(temp);
		if (currentFile != NULL)
			resultData += currentFile->getData();
		else
			cerr << "File not found!" << endl;
		paths.pop();
		if (!paths.empty())
			resultData += '\n';
	}
	this->saveConcatenatedFile(outputFilePath, resultData);
}


void CommandPrompt::copyFileInDirectory(string parameters){
	/*
	* Разделя параметрите.
	*/
	queue<string> filePaths = this->getPaths(parameters);

	/*
	* Намира файловете по адрес и ги записва в опашка.
	* Последният адрес е на destination directory.
	*/
	queue<TextFile*> files;
	TextFile* tempFile;
	while (filePaths.size() > 1) {
		tempFile = determinePathAndGetFile(filePaths.front());
		if (tempFile != NULL)
			files.push(tempFile);
		else
			cerr << "File not found!" << endl;
		filePaths.pop();
	}

	/*
	* Намираме адреса на директорията. Ако е NULL връщаме, ако не създаваме копия.
	*/
	Directory* destinationDir = this->determinePathAndGetDir(filePaths.front());
	filePaths.pop();
	if (destinationDir == NULL)
		cerr << "Destination directory not found!" << endl;
	else {
		while (!files.empty()){
			destinationDir->addChild(new TextFile(*files.front()));
			files.pop();
		}
	}
}

/*
* Изтрива файловете с адреси подадени в parameters.
*/
void CommandPrompt::deleteFile(string parameters){
	/*
	* Извлича параметрите и ги записва в опашка.
	*/
	queue<string> filePaths = this->getPaths(parameters);
	
	/*
	* Тъй като ни трябва директорията в която се намира файла за да го изтрием,
	* не може да ползваме determinePathAndGetFile().
	*/
	queue<TextFile*> files;
	string fileName;
	string fileDir;
	Directory* tempDir;
	TextFile* fileAddress;
	int pos;
	while (!filePaths.empty()) {
		/*
		* Първо разделяме пътя от името на файла
		*/ 
		fileDir = filePaths.front();
		pos = fileDir.rfind('/') + 1;
		if (pos - 1 != string::npos) {
			while (pos < fileDir.size()) {
				fileName += fileDir[pos++];
			}
			pos = fileDir.rfind('/');
			fileDir = fileDir.erase(pos, fileDir.size());

			/*
			* Намираме директорията.
			*/
			tempDir = this->determinePathAndGetDir(fileDir);

			/*
			* Ако не съществува извеждаме грешка, иначе трием.
			*/
			if (tempDir == NULL)
				cerr << "File directory not found!" << endl;
			else {
				fileAddress = tempDir->findFileByName(fileName);
				if (fileAddress == NULL)
					cerr << "File not found!" << endl;
				else
					tempDir->removeFile(fileAddress);
			}
		}
		else {
			/*
			* Ако е подадено само името на файла в currentDir търсим и трием.
			*/
			fileAddress = currentDir->findFileByName(fileDir);
			if (fileAddress == NULL)
				cerr << "File not found!" << endl;
			else
				currentDir->removeFile(fileAddress);
		}
		filePaths.pop();
	}
}


/*
* Създава всички директории подадени в параметрите. 
*/
void CommandPrompt::makeDirectory(string parameters){
	/*
	* Извличаме адресите и извикваме createNestedDirectories за всеки.
	*/
	queue<string> filePaths = getPaths(parameters);
	while (!filePaths.empty()) {
		createNestedDirectories(filePaths.front());
		filePaths.pop();
	}
}

/*
* Изтрива всички директории в параметрите.
* Root директорият не може да се изтрие.
*/
void CommandPrompt::deleteDirectory(string parameters){
	/*
	* Извличаме адресите от параметрите.
	*/
	queue<string> dirPaths = getPaths(parameters);

	/*
	* Намираме директория по дадения път и викаме deleteFile().
	* Ако е NULL извеждаме грешка. 
	* Ако текущата директория се съдържа в директорията която трием, я применяме на root.
	* Root директорията не може да се изтрие.
	*/
	Directory* dir;
	while (!dirPaths.empty()) {
		dir = this->determinePathAndGetDir(dirPaths.front());
		if (dir != NULL) {
			if(currentDir->getFSysPath().find(dir->getFSysPath()) != string::npos){
				currentDir = fs.getRoot();
				cout << "Current directory now root!" << endl;
			}
			if (dir == fs.getRoot())
				cerr << "Cannot delete root directory!" << endl;
			else
				dir->deleteFile();
		}
		else
			cerr << "Directory not found!" << endl;
		dirPaths.pop();
	}
}

/*
* Създава линкове на файловете в подадена директория.
*/
void CommandPrompt::createLinksInDirectory(string parameters){
	stack<string> filePaths;
	string tempPath;
	/*
	* Извлича адресите в стек (защото последния адрес е директория).
	*/
	for (int i = 0; i <= parameters.size(); i++) {
		if (parameters[i] == ' ' || i == parameters.size()) {
			filePaths.push(tempPath);
			tempPath = "";
		}
		else {
			tempPath += parameters[i];
		}
	}

	/*
	* Намира директорията. Ако не я намери извежда грешка и връща.
	*/
	Directory* destinationDir = this->determinePathAndGetDir(filePaths.top());
	filePaths.pop();
	if (destinationDir == NULL) {
		cerr << "Destination directory not found!" << endl;
		return;
	}

	/*
	* Намира файловете по адрес и създава линкове към тях в директорията.
	*/
	TextFile* tempFile;
	while (!filePaths.empty()) {
		tempFile = this->determinePathAndGetFile(filePaths.top());
		if (tempFile == NULL) {
			cerr << "File not found!" << endl;
		}
		else {
			destinationDir->addChild(new SymLink(tempFile, fs.getNextNumber(), destinationDir->getFSysPath()));
		}
		filePaths.pop();
	}
}


/*
* Извежда метаданните на всички файлове в параметрите.
*/
void CommandPrompt::getFileMetadata(string parameters){
	/*
	* Ако няма параметри извежда метаданните на текущата директория.
	*/
	if (parameters.empty()) {
		currentDir->printMeta();
		return;
	}

	/*
	* Извлича адресите от параметрите и за всеки намира директорията или файла 
	*/
	queue<string> dirPaths = getPaths(parameters);
	TextFile* file;
	while (!dirPaths.empty()) {
		file = this->determinePathAndGetFile(dirPaths.front());
		if (file != NULL)
			file->printMeta();
		dirPaths.pop();
	}
}

/*
* Помощтна функция, за запазване/въвеждане на файлове.
*/
void CommandPrompt::saveConcatenatedFile(string path, string data){
	/*
	* Ако няма данни и няма подаден адрес за запис връща.
	*/
	if (path.empty() && data.empty())
		return;

	/*
	* Ако няма данни се въвеждат.
	*/
	if (data.empty()) {
		char line[1000];
		string sline;
		while (sline != string(".")) {
			cin.getline(line, 1000);
			sline = line;
			if(sline != ".")
				data += sline;
		}
	}

	/*
	* Ако няма outputFile се извеждат данните, ако има
	* се намира/създава директория и се създава вайл в нея.
	* Наследява промените по големината на директориите.
	*/
	if (path.empty())
		cout << data << endl;
	else {
		string fileName = path;
		Directory* dir = fs.getRoot();
		int pos = path.rfind('/') + 1;
		if (pos - 1 != string::npos) {
			fileName = "";
			while (pos < path.size())
				fileName += path[pos++];
			pos = path.rfind('/');
			path = path.erase(pos, path.size());
			dir = this->createNestedDirectories(path);
		}
		else
			dir = currentDir;
		if (dir->nameAvailable(fileName)) {
			dir->addChild(new TextFile(dir->getFSysPath(),
				fs.getNextNumber(),
				data,
				fileName));
			dir->inheritSize(data.size());
		}
		else {
			dir->findFileByName(fileName)->concatData(data);
			dir->inheritSize(data.size());
		}
	}
}

/*
* Определя какъв е адреса и връща директорията.
* @return - адреса на директорията, NULL ако няма такава.
*/
Directory* CommandPrompt::determinePathAndGetDir(string path){
	/*
	* Ако е празен string връща NULL.
	*/ 
	if (path.empty()) {
		return NULL;
	}

	/*
	* Ако няма '/' се счита че е име на директория в текущата. Може да върне NULL ако няма такава.
	*/
	if (path.find('/') == string::npos) {
		return currentDir->findDirByName(path);
	}
	else {
		/*
		* Ако първия символ е '/' търси във файловата система.
		*/
		if (path[0] == '/') {
			return fs.getDir(path);
		}
		/*
		* Иначе релативен път в текущата директория.
		*/
		else {
			return currentDir->findDirRelative(path);
		}
	}
}

/*
* Определя пътя и връща файл.
* @return - адреса на търсения файл, NULL ако няма такъв.
*/
TextFile* CommandPrompt::determinePathAndGetFile(string path){
	if (path.find('/') == string::npos) {
		return currentDir->findFileByName(path);
	}
	else {
		if (path[0] == '/') {
			return fs.getFile(path);
		}
		else {
			return currentDir->findFileRelative(path);
		}
	}
}

/*
* Разделя адресите от string и ги записва в опашка.
*/
queue<string> CommandPrompt::getPaths(string parameters) {
	queue<string> filePaths;
	string tempPath;
	for (int i = 0; i <= parameters.size(); i++) {
		if (parameters[i] == ' ' || i == parameters.size()) {
			filePaths.push(tempPath);
			tempPath = "";
		}
		else {
			tempPath += parameters[i];
		}
	}
	return filePaths;
}

/*
* Определя какъв е адреса и го обхожда име по име.
* Ако намери директория която не съществува я създава.
* @return - търсената директория, ако поради някакви причини не се е създала връща root.
*/
Directory* CommandPrompt::createNestedDirectories(string path) {
	string currentName;
	Directory* result = NULL;
	Directory* tempDir = fs.getRoot();
	if (path.find('/') == string::npos) {
		if ((tempDir = tempDir->findDirByName(path)) == NULL) {
			result = new Directory(currentDir->getFSysPath(),
				fs.getNextNumber(),
				path,
				currentDir);
			currentDir->addChild(result);
			return result;
		}
			return tempDir;
	}
	else {
		for (int i = 0; i <= path.size(); i++) {
			if ((path[i] == '/' || i == path.size()) && !currentName.empty()) {	
				if (tempDir->findDirByName(currentName) == NULL) {
					result = new Directory(tempDir->getFSysPath(),
						fs.getNextNumber(),
						currentName,
						tempDir);
					tempDir->addChild(result);
				}
				tempDir = tempDir->findDirByName(currentName);
				currentName = "";
			}
			else if(path[i] != '/') {
				currentName += path[i];
			}
		}
	}
	if (result == NULL)
		return tempDir;
	return result;
}
	
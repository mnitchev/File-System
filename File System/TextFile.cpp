#include "TextFile.h"

TextFile::TextFile( string parentFSysPath, int FSysNumber, string data, string fileName) : AbstractFile( parentFSysPath, FSysNumber, TXT_FILE, fileName){
	this->data = data;
}

TextFile::TextFile(TextFile& T) : AbstractFile(T.getFSysPath(),
	T.getFSysNumber(),
	T.tellType(),
	T.getFileName()){
	this->data = T.data;
}


/*
* Извеждане на метаданните.
*/
void TextFile::printMeta(){
	cout << "File name : " << getFileName() << endl
		<< "File type : " << fileTypeToString(tellType()) << endl
		<< "Size : " << getSize() << endl
		<< "Last accessed : " << getLastAccess() << endl
		<< "Last modified : " << getLastModified() << endl
		<< "Metadata last modified : " << getMetaLastModified() << endl;
 }

/*
* Променя линковете които сочат към файла на NULL.
* Директорията която съдържа файла сама трябва да се погрижи
* за изтриването му.
*/
void TextFile::deleteFile(){
	auto it = links.begin();
	while (it != links.end()) {
		(*it)->deleteFile();
		it++;
	}
}

#include "SymLink.h"

SymLink::SymLink(TextFile* txtFile, int number, string path) : AbstractFile( path, number, SYM_LINK, txtFile->getFileName() + "ln") {
	file = txtFile;
	txtFile->addLink(this);
}

void SymLink::deleteFile(){
	file = NULL;
}

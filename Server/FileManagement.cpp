#include "stdafx.h"
#include "string.h"
#include <fstream> 
#include <queue>
#include "FileManagement.h"

#pragma warning(disable:4996)

using namespace std;
namespace fs = std::experimental::filesystem;

//------------------------------Directory--------------------
char appPath[256];

/*
Set application's data folder
*/
void setAppPath() {
	appPath[0] = '\0';
	strcat_s(appPath, getenv("HOMEDRIVE"));
	strcat_s(appPath, getenv("HOMEPATH"));
	strcat_s(appPath, "\\FileStoreApp");
}

/*
Add a directory/file's name to the end of the given path

input: (char* current path, char* name which needed to add to the path)
output: void
*/
void addToPath(char* path, char* name) {
	strcat(path, "\\");
	strcat(path, name);
}

/*
Get user's directory path
*/
char* getOwnerPath(char* owner) {
	char ownerPath[256] = "\0";
	strcat(ownerPath, "\\");
	strcat(ownerPath, owner);
	return ownerPath;
}

/*
Create new directory at `path` with the given name

input: (char* contain directory path, char* name of the new directory)
output: path to the new directory
*/
char* createDirectory(char* path, char* name) {
	char newDirectoryPath[1024];
	strcpy(newDirectoryPath, path);
	addToPath(newDirectoryPath, name);
	if (!fs::is_directory(newDirectoryPath)) {
		fs::create_directories(newDirectoryPath);
	}
	return newDirectoryPath;
}

/*
Create new directory

input: (char* directory's path)
output: void
*/
void createDirectoryByPath(char* path) {
	if (!fs::is_directory(path)) {
		fs::create_directories(path);
	}
}

//------------------------------Directory tree------------------

/*
Support function
source: https://stackoverflow.com/questions/26522583/c-strtok-skips-second-token-or-consecutive-delimiter
*/
char *strtok_new(char * string, char const * delimiter) {
	static char *source = NULL;
	char *p, *riturn = 0;
	if (string != NULL)         source = string;
	if (source == NULL)         return NULL;

	if ((p = strpbrk(source, delimiter)) != NULL) {
		*p = 0;
		riturn = source;
		source = ++p;
	}
	return riturn;
}

/*
Split string by delimiter

input:
char* a char array (must not be a constant string),
char* a string contain 1 character only
output: a vector contain char pointers of separated string
*/
vector<char*> split(char* string, char* delimiter) {
	int offSet = 0;
	if (string[0] == *delimiter) {
		offSet = 1;
	}
	vector<char*> re;
	char* p = strtok_new(string + offSet, delimiter);

	while (p) {
		if (*p) {
			re.push_back(p);
		}
		else {
			re.push_back("");
		}
		//cout << p << endl;
		p = strtok_new(NULL, delimiter);
	}
	return re;
}

Node* navigateInTree(Node* root, char* tracePath) {
	vector<char*> indexes = split(tracePath, ",");
	Node* curNode = root;
	for (int i = 0; i < indexes.size(); i++) {
		curNode = curNode->children[atoi(indexes[i])];
	}
	return curNode;
}

/*
Create a tree for the folder structure that the string represent

input:
char* the string contain folder structure,
char* the folder owner id
output:
Node* a pointer to the tree's root
*/
Node* stringToTree(char* inputString, char* owner) {
	queue<Node*> nodeQueue;
	Node* root = new Node(false, "\\", NULL, owner);
	nodeQueue.push(root);
	vector<char*> folderContent = split(inputString, "/");
	for (int i = 0; i < folderContent.size(); i++) {
		Node* curNode = nodeQueue.front();
		vector<char*> subFolder = split(folderContent[i], "|");

		for (int j = 0; j < subFolder.size(); j++) {

			if (subFolder[j][0] == '*') {
				// Is a file
				int start = strcspn(subFolder[j], "<");
				int end = strcspn(subFolder[j], ">");
				char* name = (char*)malloc(start);
				strncpy(name, subFolder[j] + 1, start - 1);
				name[start - 1] = '\0';
				char* storingName = (char*)malloc(end - start);
				strncpy(storingName, subFolder[j] + start + 1, end - start - 1);
				storingName[end - start - 1] = '\0';
				// TODO: check for exception
				Node* temp = new Node(true, name, storingName, owner);
				curNode->children.push_back(temp);
			}
			else {
				// Is a folder
				Node* temp = new Node(false, subFolder[j], NULL, owner);
				curNode->children.push_back(temp);
				nodeQueue.push(temp);
			}

		}
		nodeQueue.pop();
	}
	return root;
}

char* treeToString(Node* root) {
	string result = "/";

	Node* curNode;
	unsigned short curChildrenLen;
	queue<unsigned short> childrenLen;
	queue<Node*> nodeQueue;

	for (int i = 0; i < root->children.size(); i++) {
		nodeQueue.push(root->children[i]);
	}
	childrenLen.push(root->children.size());

	while (nodeQueue.size() > 0 || childrenLen.size() > 0) {
		curChildrenLen = childrenLen.front();
		childrenLen.pop();

		for (int j = 0; j < curChildrenLen; j++) {
			curNode = nodeQueue.front();
			nodeQueue.pop();

			if (curNode->isFile) {
				result += "*";
				result += curNode->name;
				result += "<";
				result += curNode->storingName;
				result += ">|";
			}
			else {
				result += curNode->name;
				result += "|";

				childrenLen.push(curNode->children.size());
				for (int i = 0; i < curNode->children.size(); i++) {
					nodeQueue.push(curNode->children[i]);
				}
			}
		}
		result += "/";
	}
	char* resultChar = new char[result.length() + 1];
	strcpy(resultChar, result.c_str());
	return resultChar;
}

Node* goToNodeWithName(char* name, Node* root) {
	if (root->isFile) {
		return NULL;
	}
	for (int i = 0; i < root->children.size(); i++) {
		if (strcmp(name, root->children[i]->name) == 0) {
			return root->children[i];
		}
	}
	return NULL;
}

/*
	Create a new Node as child of the given Node

	input: ....
	output:
		-1: given node is a file, cannot have child,
		-2: file/folder name existed,
		else: ok
*/
int addNewNode(Node* parent, bool _isFile, char* _name, char* _storingName, char* _owner) {
	if (parent->isFile) {
		return -1;
	}
	for (int i = 0; i < parent->children.size(); i++) {
		if (strcmp(_name, parent->children[i]->name) == 0) {
			return -2;
		}
	}

	Node* newNode = new Node(_isFile, _name, _storingName, _owner);
	parent->children.push_back(newNode);
	return 0;
}

//-------------------------------File-------------------------

/*
Get file's size

input: file's name
output: file's length
on error: -1 (file isn't exist,...)
*/
int getFileLength(char* fileName) {
	ifstream file;
	file.open(fileName, ifstream::binary);
	if (!file.is_open()) {
		return -1;
	}
	file.seekg(0, file.end);
	int fileLength = file.tellg();
	file.close();
	return fileLength;
}

/*
Read file start from the given position

input:
file's name,
starting point (>=0),
maximum reading length,
location to write readed data
output:
-1: error ( file not exist, wrong input, ...)
0: reached end of file
else: length of readed data
*/
int readFile(char* fileName, int start, int buffLen, char* buff) {
	ifstream file;
	file.open(fileName, ifstream::binary);
	if (!file.is_open() || start < 0) {
		return -1;
	}

	file.seekg(0, file.end);
	int fileLength = file.tellg();
	file.seekg(start, file.beg);

	if (start >= fileLength) {
		file.close();
		return 0;
	}

	int readLen;

	if (start + buffLen <= fileLength) {
		readLen = buffLen;
	}
	else {
		readLen = fileLength - start;
	}
	file.read(buff, readLen);

	if (!file) {
		// could only read <file.gcount()> bytes
		file.close();
		return file.gcount();
	}

	file.close();
	return readLen;
}

/*
Open file and append to the end of file

input:
file's name,
writing data,
length of writing data
output:
-1: error,
else: ok
*/
int appendToFile(char* fileName, char *buff, int buffLen) {
	ofstream file;

	file.open(fileName, ios::out | ios::app | ios::binary);
	if (!file.is_open()) {
		return -1;
	}

	file.write(buff, buffLen);
	file.close();
	return 0;
}

/*
Open file and overwrite file's data

input:
file's name,
writing data,
length of writing data
output:
-1: error,
else: ok
*/
int writeToFile(char* fileName, char *buff, int buffLen) {
	ofstream file;

	file.open(fileName, ios::out | ios::trunc | ios::binary);
	if (!file.is_open()) {
		return -1;
	}

	file.write(buff, buffLen);
	file.close();
	return 0;
}

/*
Delete file

input: file's name
output:
-1: error
else: ok
*/
int deleteFile(char* fileName) {
	if (remove(fileName) != 0)
		return -1;
	else
		return 0;
}

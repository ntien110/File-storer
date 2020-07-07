#pragma once

#ifndef FILE_MANAGEMENT_H 
#define FILE_MANAGEMENT_H

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include <vector>

#pragma warning(disable:4996)

// Global variables
extern char appPath[256];

// Node for directory tree structure
struct Node;

// Functions
void setAppPath();

char* createDirectory(char* path, char* name);

void createDirectoryByPath(char* path);

void addToPath(char* path, char* name);

Node* stringToTree(char* inputString, char* owner);

char* treeToString(Node* root);

int deleteFile(char* fileName);

int writeToFile(char* fileName, char *buff, int buffLen);

int appendToFile(char* fileName, char *buff, int buffLen);

int readFile(char* fileName, int start, int buffLen, char* buff);

int getFileLength(char* fileName);

struct Node {
	bool isFile;
	char* name;
	char* storingName;
	char* owner;
	std::vector<Node*> children;

	Node(bool _isFile, char* _name, char* _storingName, char* _owner) {
		isFile = _isFile;
		name = _name;
		storingName = _storingName;
		owner = _owner;
	}

	char* getPath() {
		char path[1024];
		strcat_s(path, appPath);
		addToPath(path, owner);
		addToPath(path, "data");
		addToPath(path, storingName);
		return path;
	}
};

#endif
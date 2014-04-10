#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <direct.h>
#include <Windows.h>
#include <stdio.h>

using namespace std;

const static string VERSIONNUM = "1.7";

//global vars
string dir, extension, prefix, suffix, removeThis, replaceOriginal, replaceNew, insertHere, insertThis, singleFileEdit, searchThis, searchNumber,
	addExtension, startDelete, endDelete, advancedFile, advancedNewFile, oldDIR, newDIR;
bool repeatAction, hasParam, insertI, capitalize, ALLCAPS, allLower;

//functions
char checkParam(int, char *);
void storeParam(int, char, int, char*[]);
bool nextIsParamOrBlank(int, int, char *[]);
bool stringEquals(string, string);
bool isPosNum(string);
void getFiles(vector<string>&, string);
string removeExtension(string);
void removeExtensions(vector<string>&);
string findExtension(string);
void findExtensions(vector<string>, vector<string>&);
void showVersion();
void help();
void badSyntax();
string getCurrentDir();

void main(int argc, char *argv[])
{
	//local vars
	vector<string> extensions;
	vector<string> files;
	vector<string> renamed;
	vector<string> oldFiles;
	vector<string> newFiles;
	vector<string> commands;
	string::size_type position;

	string command;

	//initialize vars
	dir = getCurrentDir();

	extension = "*";
	prefix.clear();
	suffix.clear();
	removeThis.clear();
	replaceOriginal.clear();
	replaceNew.clear();
	insertHere.clear();
	insertThis.clear();
	singleFileEdit.clear();
	searchThis.clear();
	addExtension.clear();
	startDelete.clear();
	endDelete.clear();
	advancedFile.clear();
	advancedNewFile.clear();
	oldDIR.clear();
	newDIR.clear();
	searchNumber = "1";
	repeatAction = false;
	hasParam = true;
	insertI = false;
	capitalize = false;
	ALLCAPS = false;
	allLower = false;

	//check all params entered and set vars
	if (argc == 1)
	{
		help();
	}
	else
	{
		for (int count=1;count<argc;count++)
		{
			if (argv[count][0] == '/')
			{
				storeParam(count, checkParam(count, argv[count]), argc, argv);
				if (hasParam)
				{				
					if (argv[count][1] == 'S' || argv[count][1] == 'i' || argv[count][1] == 'I' || argv[count][1] == 'n' || argv[count][1] == 'D'
						|| argv[count][1] == 'a' || argv[count][1] == 'M')
					{
						if (!nextIsParamOrBlank(count,argc,argv) &&!nextIsParamOrBlank(count+1,argc,argv))
						count++;
					}
					count++;
				}
			}
			else
				badSyntax();
		}
	}
	if (allLower && ALLCAPS)
		badSyntax();

	//directory
	_chdir(dir.c_str());

	//get files
	if (singleFileEdit.empty())
	{
		getFiles(files,dir);
		findExtensions(files,extensions);
		renamed = files;
	}
	else
	{
		files.push_back(singleFileEdit);
		findExtensions(files,extensions);
		renamed = files;
	}	

#pragma region replacing
	//replacing
	if ((!replaceOriginal.empty() && replaceNew.empty()) || (replaceOriginal.empty() && !replaceNew.empty()))
		badSyntax();
	else if (!replaceOriginal.empty() && !replaceNew.empty())
	{
		if (extension != "*")
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (stringEquals(extension, extensions[i]))
				{
					if (repeatAction)
					{
						while (renamed[i].find(replaceOriginal) != string::npos)
						{
							renamed[i].replace(renamed[i].find(replaceOriginal), replaceOriginal.length(), replaceNew);
							command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
							system(command.c_str());
							files = renamed;
						}
					}
					else
					{
						if (renamed[i].find(replaceOriginal) != string::npos)
						{
							renamed[i].replace(renamed[i].find(replaceOriginal), replaceOriginal.length(), replaceNew);
							command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
							system(command.c_str());						
						}
					}
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (repeatAction)
				{
					while (renamed[i].find(replaceOriginal) != string::npos)
					{
						renamed[i].replace(renamed[i].find(replaceOriginal), replaceOriginal.length(), replaceNew);
						command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
						system(command.c_str());
						files = renamed;
					}
				}
				else
				{
					if (renamed[i].find(replaceOriginal) != string::npos)
					{
						renamed[i].replace(renamed[i].find(replaceOriginal), replaceOriginal.length(), replaceNew);
						command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
						system(command.c_str());						
					}
				}
			}
		}
	}
#pragma endregion

	//reinitialize files vector to new names
	files = renamed;

#pragma region removing
	//removing
	if (!removeThis.empty())
	{
		if (extension != "*")
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (stringEquals(extension, extensions[i]))
				{
					if (repeatAction)
					{
						while (renamed[i].find(removeThis) != string::npos)
						{
							renamed[i].replace(renamed[i].find(removeThis), removeThis.length(), "");
							command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
							system(command.c_str());
							files = renamed;
						}
					}
					else
					{
						if (renamed[i].find(removeThis) != string::npos)
						{
							renamed[i].replace(renamed[i].find(removeThis), removeThis.length(), "");
							command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
							system(command.c_str());
							files = renamed;
						}
					}
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (repeatAction)
				{
					while (renamed[i].find(removeThis) != string::npos)
					{
						renamed[i].replace(renamed[i].find(removeThis), removeThis.length(), "");
						command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
						system(command.c_str());
					}
				}
				else
				{
					if (renamed[i].find(removeThis) != string::npos)
					{
						renamed[i].replace(renamed[i].find(removeThis), removeThis.length(), "");
						command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
						system(command.c_str());
					}
				}
			}
		}
	}
#pragma endregion

	//reinitialize files vector to new names
	files = renamed;

#pragma region removing section
	//removing section
		
	if ((!startDelete.empty() && endDelete.empty()) || (startDelete.empty() && !endDelete.empty()))
		badSyntax();
	else if (!startDelete.empty() && !endDelete.empty())
	{
		if (extension != "*")
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (stringEquals(extension, extensions[i]))
				{					
					if (startDelete == endDelete)
						renamed[i].erase(renamed[i].begin()+atoi(startDelete.c_str())-1);
					else
						renamed[i].erase(renamed[i].begin()+atoi(startDelete.c_str())-1,renamed[i].begin()+atoi(endDelete.c_str()));
					command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
					system(command.c_str());						
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (startDelete == endDelete)
					renamed[i].erase(renamed[i].begin()+atoi(startDelete.c_str())-1);
				else
					renamed[i].erase(renamed[i].begin()+atoi(startDelete.c_str())-1,renamed[i].begin()+atoi(endDelete.c_str()));
				command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
				system(command.c_str());
			}
		}
	}
#pragma endregion

	//reinitialize files vector to new names
	files = renamed;

#pragma region inserting
	//inserting
	if ((!insertHere.empty() && insertThis.empty()) || (insertHere.empty() && !insertThis.empty()))
		badSyntax();
	else if (!insertHere.empty() && !insertThis.empty())
	{
		if (extension != "*")
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (stringEquals(extension, extensions[i]))
				{				
					if (!insertI)
					{
						renamed[i].insert(renamed[i].begin()+atoi(insertHere.c_str())-1,insertThis.begin(),insertThis.end());
						command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
						system(command.c_str());
					}
					else
					{
						position = renamed[i].find(insertHere);
						if (position != string::npos)
						{
							renamed[i].insert(renamed[i].begin()+position,insertThis.begin(),insertThis.end());
							command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
							system(command.c_str());
						}
					}					
					files = renamed;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (!insertI)
				{
					renamed[i].insert(renamed[i].begin()+atoi(insertHere.c_str())-1,insertThis.begin(),insertThis.end());
					command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
					system(command.c_str());
				}
				else
				{
					position = renamed[i].find(insertHere);
					if (position != string::npos)
					{
						renamed[i].insert(renamed[i].begin()+position,insertThis.begin(),insertThis.end());
						command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
						system(command.c_str());
					}
				}
				files = renamed;
			}
		}
	}
#pragma endregion
	//reinitialize files vector to new names
	files = renamed;

#pragma region prefix
	//prefix
	if (!prefix.empty())
	{
		if (extension != "*")
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (stringEquals(extension, extensions[i]))
				{
					renamed[i] = prefix + renamed[i];
					command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
					system(command.c_str());
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{			
				renamed[i] = prefix + renamed[i];
				command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
				system(command.c_str());
			}
		}
	}
#pragma endregion

	//reinitialize files vector to new names
	files = renamed;

#pragma region suffix
	//suffix
	if (!suffix.empty())
	{
		unsigned int count;
		if (extension != "*")
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (stringEquals(extension, extensions[i]))
				{			
					count = 1;
					while (count<renamed[i].length()&&*(renamed[i].end()-count)!='.')
						count++;

					if (count == renamed[i].length())
						renamed[i] = renamed[i] + suffix;
					else
					{
						renamed[i].insert(renamed[i].end()-count,suffix.begin(),suffix.end());
					}
					command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
					system(command.c_str());
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < files.size(); i++)
			{			
				count = 1;
				while (count<renamed[i].length()&&*(renamed[i].end()-count)!='.')
					count++;

				if (count == renamed[i].length())
					renamed[i] = renamed[i] + suffix;
				else
				{
					renamed[i].insert(renamed[i].end()-count,suffix.begin(),suffix.end());
				}
				command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
				system(command.c_str());
			}
		}
	}
#pragma endregion
	
	//reinitialize files vector to new names
	files = renamed;

#pragma region add extension
	//add extension
	if (!addExtension.empty())
	{
		if (extension != "*")
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (stringEquals(extension, extensions[i]))
				{			
					renamed[i] = renamed[i] + addExtension;
					command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
					system(command.c_str());
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < files.size(); i++)
			{			
				renamed[i] = renamed[i] + addExtension;
				command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
				system(command.c_str());
			}
		}
	}

#pragma endregion

	//reinitialize files vector to new names
	files = renamed;

#pragma region search
	//search
	if (!searchThis.empty())
	{
		if (repeatAction)
			badSyntax();
		else if (extension != "*")
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (stringEquals(extension, extensions[i]))
				{				
					position = renamed[i].find(searchThis);
					if (atoi(searchNumber.c_str()) > 1)
					{
						for (int j = 1; j < atoi(searchNumber.c_str()); j++)
							position = renamed[i].find(searchThis, position+1);
					}
					if (position != string::npos)
						position++;

					if (position == string::npos)
						cout << "\nDid not find \"" + searchThis + "\" in " + renamed[i];
					else
						cout << "\nFound \"" + searchThis + "\" in " + renamed[i] + " at position: " << position;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{				
				position = renamed[i].find(searchThis);
				if (atoi(searchNumber.c_str()) > 1)
				{
					for (int j = 1; j < atoi(searchNumber.c_str()); j++)
						position = renamed[i].find(searchThis, position+1);
				}
				if (position != string::npos)
					position++;

				if (position == string::npos)
					cout << "\nDid not find \"" + searchThis + "\" in " + renamed[i];
				else
					cout << "\nFound \"" + searchThis + "\" in " + renamed[i] + " at position: " << position;
			}
		}
	}
#pragma endregion

	//reinitialize files vector to new names
	files = renamed;

#pragma region all lower
	//all lower
	if (allLower)
	{
		if (extension != "*")
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (stringEquals(extension, extensions[i]))
				{
					for (unsigned int j = 0; j < renamed[i].size()-extensions[i].size(); j++)
					{
						if (isalpha(renamed[i][j]))
						{
							renamed[i][j] = tolower(renamed[i][j]);
							command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
							system(command.c_str());
							files = renamed;
						}
					}					
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				for (unsigned int j = 0; j < renamed[i].size()-extensions[i].size(); j++)
				{
					if (isalpha(renamed[i][j]))
					{
						renamed[i][j] = tolower(renamed[i][j]);
						command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
						system(command.c_str());
						files = renamed;						
					}
				}				
			}
		}
	}
#pragma endregion 

	//reinitialize files vector to new names
	files = renamed;

#pragma region capitalize
	//capitalize
	if (capitalize)
	{
		if (extension != "*")
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (stringEquals(extension, extensions[i]))
				{
					bool found = false;
					for (unsigned int j = 0; j < renamed[i].size()-extensions[i].size(); j++)
					{
						if (found && !isalpha(renamed[i][j]))
							found = false;
						else if (!found && isalpha(renamed[i][j]))
						{
							renamed[i][j] = toupper(renamed[i][j]);
							command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
							system(command.c_str());
							files = renamed;
							found = true;
						}
					}					
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				bool found = false;
				for (unsigned int j = 0; j < renamed[i].size()-extensions[i].size(); j++)
				{
					if (found && !isalpha(renamed[i][j]))
						found = false;
					else if (!found && isalpha(renamed[i][j]))
					{
						renamed[i][j] = toupper(renamed[i][j]);
						command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
						system(command.c_str());
						files = renamed;
						found = true;
					}
				}				
			}
		}
	}
#pragma endregion

	//reinitialize files vector to new names
	files = renamed;

#pragma region ALLCAPS
	//ALLCAPS
	if (ALLCAPS)
	{
		if (extension != "*")
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (stringEquals(extension, extensions[i]))
				{
					for (unsigned int j = 0; j < renamed[i].size()-extensions[i].size(); j++)
					{
						if (isalpha(renamed[i][j]))
						{
							renamed[i][j] = toupper(renamed[i][j]);
							command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
							system(command.c_str());
							files = renamed;
						}
					}					
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				for (unsigned int j = 0; j < renamed[i].size()-extensions[i].size(); j++)
				{
					if (isalpha(renamed[i][j]))
					{
						renamed[i][j] = toupper(renamed[i][j]);
						command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
						system(command.c_str());
						files = renamed;						
					}
				}				
			}
		}
	}
#pragma endregion 

	files = renamed;

#pragma region advanced rename
	//advanced rename
	if (!advancedFile.empty() && !advancedNewFile.empty())
	{
		command = "REN \"" + advancedFile + "\" \"" + advancedNewFile + findExtension(advancedFile) +"\"";
		system(command.c_str());
	}
		
#pragma endregion

#pragma region directory rename
	//rename directory files to other directory file names
	if (!oldDIR.empty() && !newDIR.empty())
	{
		getFiles(oldFiles, oldDIR); getFiles(newFiles, newDIR);
		if (oldFiles.size() != newFiles.size())
		{
			cout << "\nCannot rename files unless both directories have an equal number of files.\n";
			exit(0);
		}

		commands.clear();
		cout << "\nFile names will be renamed as such:\n\n";
		for (unsigned int i = 0; i < oldFiles.size(); i++)
		{
			commands.push_back("REN \"" + newFiles[i] + "\" \"" + removeExtension(oldFiles[i]) + findExtension(newFiles[i]) +"\"");
			cout << newFiles[i] + " -> " + removeExtension(oldFiles[i]) + findExtension(newFiles[i]) + "\n";
		}
		cout << "\nContinue with operation? (Y\\N): ";
		string verify; cin >> verify;
		bool verifying = true;
		
		while (verifying)
		{
			if (verify == "Y" || verify == "y")
			{
				_chdir(newDIR.c_str());
				for (unsigned int i=0;i<commands.size();i++)
				{
					system(commands[i].c_str());
				}
				cout << "\nFiles have been successfully renamed\n";
				verifying = false;
			}
			else if (verify == "N" || verify == "n")
			{
				cout << "\nDirectory renaming canceled\n";	
				verifying = false;
			}
			else
			{
				cout << "\nPlease enter Y or N to verify if you want to continue with renaming: ";
			}
		}
	}

#pragma endregion
}

void getFiles(vector<string> &files, string fileDIR)
{
	ifstream fin;
	string file;
	string brenFile =".bren";
	string command;

	command = "DIR /B \"" + fileDIR + "\" > \"" + brenFile + "\"";
	system(command.c_str());

	fin.open(brenFile);
	while (getline(fin,file))
	{
		if (file != ".bren")
			files.push_back(file);
	}
	fin.close(); fin.clear();

	remove(brenFile.c_str());
}

string removeExtension(string file)
{
	string newFile=""; unsigned int i = 1;
	while (file.size() > 0 && *(file.end()-1) != '.')
	{
		file.resize(file.size()-1);			
	}
	if (*(file.end()-1)=='.')
		file.resize(file.size()-1);

	return file;
}

void removeExtensions(vector<string> &files)
{
	for (unsigned int i=0;i<files.size();i++)
	{
		removeExtension(files[i]);
	}
}

string findExtension(string file)
{
	string extension=""; unsigned int i;
	for (i=1;i<file.length()&&*(file.end()-i)!='.';i++)
	{
		extension = *(file.end()-i)+extension;			
	}

	if (i<file.length()&&*(file.end()-i)=='.')
		extension="."+extension;
	else
		extension.clear();
	return extension;
}

void findExtensions(vector<string> files, vector<string> &extensions)
{
	string extension="";
	for (unsigned int count=0;count<files.size();count++)
	{
		extension = findExtension(files[count]); 
		extensions.push_back(extension); 
		extension.clear();
	}
}

char checkParam(int pos, char *param)
{
	string arg = param;

	if (arg.size() != 2)
		return 'q';
	else
		return param[1];
}

void storeParam(int pos, char option, int argc, char *argv[])
{
	switch(option)
	{
	case 'h':
		if (argc > 2)
			badSyntax();
		else
		{
			help();
			hasParam = true;
		}
		break;
	case 'a':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else if (!nextIsParamOrBlank(pos+1,argc,argv))
			advancedNewFile = argv[pos+2];

		advancedFile = argv[pos+1];
		hasParam = true;
		break;
	case 'i':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else if (!nextIsParamOrBlank(pos+1,argc,argv))
		{
			insertHere = argv[pos+2];
			if (!isPosNum(insertHere))
				badSyntax();			
		}

		insertThis = argv[pos+1];
		hasParam = true;
		insertI = false;
		break;
	case 'I':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else if (!nextIsParamOrBlank(pos+1,argc,argv))
			insertHere = argv[pos+2];

		insertThis = argv[pos+1];
		hasParam = true;
		insertI = true;
		break;
	case 'c':
		capitalize = true;
		hasParam = false;
		break;
	case 'C':
		ALLCAPS = true;
		hasParam = false;
		break;
	case 'd':
		if (nextIsParamOrBlank(pos, argc, argv))
			break;
		else
		{
			dir = argv[pos+1];
			while (dir.find("\\") != string::npos)
				dir.replace(dir.find("\\"),1,"/");
			hasParam = true;
		}
		break;
	case 'D':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;		
		else if (!nextIsParamOrBlank(pos+1,argc,argv))
		{
			endDelete = argv[pos+2];

			if (!isPosNum(endDelete))
				badSyntax();
		}

		startDelete = argv[pos+1];
		if (!isPosNum(startDelete))
			badSyntax();		

		if (atoi(endDelete.c_str()) < atoi(startDelete.c_str()))
			badSyntax();

		hasParam = true;
		break;
	case 'e':
		if (nextIsParamOrBlank(pos, argc, argv))
			break;
		else
		{
			addExtension = argv[pos+1];
			hasParam = true;
		}
		break;
	case 'f':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
		{
			extension = ".";
			extension = extension + argv[pos+1];
			hasParam = true;
		}
		break;
	case 'F':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
		{
			singleFileEdit = argv[pos+1];
			hasParam = true;
		}
		break;
	case 'L':
		allLower = true;
		hasParam = false;
		break;
	case 'p':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
		{
			prefix = argv[pos+1];
			hasParam = true;
		}
		break;
	case 'r':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
		{
			removeThis = argv[pos+1];
			hasParam = true;
		}
		break;
	case 'M':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else if (!nextIsParamOrBlank(pos+1,argc,argv))
		{
			oldDIR = argv[pos+1];
			newDIR = argv[pos+2];
		}
	case 'n':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else if (!nextIsParamOrBlank(pos+1,argc,argv))
			replaceNew = argv[pos+2];

		replaceOriginal = argv[pos+1];
		hasParam = true;
		break;
	case 'R':
		repeatAction = true;
		hasParam = false;
		break;
	case 's':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
		{
			suffix = argv[pos+1];
			hasParam = true;
		}
		break;
	case 'S':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else if (!nextIsParamOrBlank(pos+1,argc,argv))
		{
			searchNumber = argv[pos+2];
			if (!isPosNum(searchNumber))
				badSyntax();			
		}
		searchThis = argv[pos+1];
		hasParam = true;
		break;
	case 'v':
		if (argc > 2)
			badSyntax();
		else
		{
			showVersion();
			hasParam = true;
		}
		break;
	default:
		badSyntax();
	}
}

bool nextIsParamOrBlank(int pos, int argc, char *argv[])
{
	if (pos+1 >= argc)
		return true;
	else if (argv[pos+1][0] == '/')
		return true;
	else
		return false;
}

bool stringEquals(string original, string compare)
{
	string orig = original, comp = compare;
	for (unsigned int count = 0; count < orig.size(); count++)
	{
		if (isalpha(orig[count]))
			orig[count] = tolower(orig[count]);		
	}

	for (unsigned int count = 0; count < comp.size(); count++)
	{
		if (isalpha(comp[count]))
			comp[count] = tolower(comp[count]);
	}

	if (orig == comp)
		return true;
	else 
		return false;
}

bool isPosNum(string check)
{
	for (unsigned int i = 0; i < check.size(); i++)
	{
		if (!isdigit(check[i]))
			return false;
	}
	if (atoi(check.c_str()) < 1)
		return false;

	return true;
}

string getCurrentDir() 
{
	char cCurrentPath[FILENAME_MAX];
	_getcwd(cCurrentPath, sizeof(cCurrentPath));
	return cCurrentPath;
}

void badSyntax()
{
	cout << "\nIncorrect Syntax\n"
		<< "Please try bren /h\n";
	exit(1);
}

void showVersion()
{
	cout << "Batch Rename Version " + VERSIONNUM + "\n";
	exit(0);
}

void help()
{
	cout << "\nBatch Rename\n" 
		<< "Usage: bren [options]\n\n"
		<< "Options\n\n"
		<< " /h\t\t\t\tBrings up this help dialog\n\n"	
		<< " /a <filename> <new filename>\tAdvanced Rename\n\t\t\t\t(Attempts to preserve extension while renaming)\n\n"
		<< " /c\t\t\t\tCapitalize\n\n"
		<< " /C\t\t\t\tALL CAPS (UMADBRO)\n\n"
		<< " /d <directory>\t\t\tDirectory to rename \n\t\t\t\t(defaults to current directory)\n\n"	
		<< " /D <start> <end>\t\tDelete section between start and end locations\n\n"
		<< " /e <extension>\t\t\tAdd extension\n\n"
		<< " /f <extension>\t\t\tFile extension to rename (defaults to *)\n\n"
		<< " /F <filename>\t\t\tRename only this file\n\n"
		<< " /i <value> <location>\t\tInsert at location\n\n"
		<< " /I <value> <string>\t\tInsert at string location\n\n"
		<< " /L\t\t\t\tAll lowercase\n\n"
		<< " /M <old Dir> <new Dir>\t\tAttempts to rename new Dir files to the old Dir\n\t\t\t\tfile names in alpha order\n\n"
		<< " /n <string> <value>\t\tReplace string with value\n\n"
		<< " /p <prefix>\t\t\tAdd prefix to file names\n\n"
		<< " /r <string>\t\t\tRemove string from file names\n\n"
		<< " /R\t\t\t\tRepeat action through whole file\n\n"
		<< " /s <suffix>\t\t\tAdd suffix to file names\n\t\t\t\t(Attempts to append suffix before extension)\n\n"
		<< " /S <string> <iteration>\tSearch for string position \n\t\t\t\t(Iteration Optional)\n\n"
		<< " /v\t\t\t\tDisplays Batch Rename Version Number\n"
		;
	exit(0);
}
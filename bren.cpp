#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <direct.h>
#include <Windows.h>
#include <stdio.h>

using namespace std;

const static string VERSIONNUM = "1.0";

//global vars
string dir, extension, prefix, suffix, removeThis, replaceOriginal, replaceNew;

//functions
char checkParam(int, char *);
void storeParam(int, char, int, char*[]);
bool nextIsParamOrBlank(int, int, char *[]);
bool stringEquals(string, string);
void getFiles(vector<string>&);
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

	string command;

	//initialize vars
	dir = getCurrentDir();

	extension = "*";
	prefix.clear();
	suffix.clear();
	removeThis.clear();
	replaceOriginal.clear();
	replaceNew.clear();

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
				count++;
			}
			else
				badSyntax();
		}
	}

	//directory
	_chdir(dir.c_str());

	//get files
	getFiles(files);
	findExtensions(files,extensions);
	renamed = files;

	//replacing
	if ((!replaceOriginal.empty() && replaceNew.empty()) || (replaceOriginal.empty() && !replaceNew.empty()))
		badSyntax();
	else
	{
		if (extension != "*")
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
			{
				if (stringEquals(extension, extensions[i]) && renamed[i].find(replaceOriginal) != string::npos)
				{
					renamed[i].replace(renamed[i].find(replaceOriginal), replaceOriginal.length(), replaceNew);
					command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
					system(command.c_str());
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < renamed.size(); i++)
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

	//reinitialize files vector to new names
	files = renamed;

	//removing
	if (extension != "*")
	{
		for (unsigned int i = 0; i < renamed.size(); i++)
		{
			if (stringEquals(extension, extensions[i]) && renamed[i].find(removeThis) != string::npos)
			{
				renamed[i].replace(renamed[i].find(removeThis), removeThis.length(), "");
				command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
				system(command.c_str());
			}
		}
	}
	else
	{
		for (unsigned int i = 0; i < renamed.size(); i++)
		{
			if (renamed[i].find(removeThis) != string::npos)
			{
				renamed[i].replace(renamed[i].find(removeThis), removeThis.length(), "");
				command = "REN \"" + files[i] + "\" \"" + renamed[i] +"\"";
				system(command.c_str());
			}
		}
	}

	//reinitialize files vector to new names
	files = renamed;

	//prefix
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

	//reinitialize files vector to new names
	files = renamed;

	//suffix
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

void getFiles(vector<string> &files)
{
	ifstream fin;
	string file;

	system("DIR /B > .bren");

	fin.open(".bren");
	while (getline(fin,file))
	{
		if (file != ".bren")
			files.push_back(file);
	}
	fin.close(); fin.clear();

	system("DEL /F .bren");
}

void findExtensions(vector<string> files, vector<string> &extensions)
{
	string extension=""; unsigned int i;
	for (unsigned int count=0;count<files.size();count++)
	{
		for (i=1;i<files[count].length()&&*(files[count].end()-i)!='.';i++)
		{
			extension = *(files[count].end()-i)+extension;			
		}

		if (i<files[count].length()&&*(files[count].end()-i)=='.')
		{
			extension="."+extension; extensions.push_back(extension); extension.clear();
		}
		else
		{
			extensions.push_back(""); extension.clear();
		}
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
			help();
		break;
	case 'd':
		if (nextIsParamOrBlank(pos, argc, argv))
			break;
		else
		{
			dir = argv[pos+1];
			while (dir.find("\\") != string::npos)
				dir.replace(dir.find("\\"),1,"/");
		}
		break;
	case 'f':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
		{
			extension = ".";
			extension = extension + argv[pos+1];
		}
		break;
	case 'o':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
			replaceOriginal = argv[pos+1];
		break;
	case 'n':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
			replaceNew = argv[pos+1];
		break;
	case 'p':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
			prefix = argv[pos+1];
		break;
	case 'r':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
			removeThis = argv[pos+1];
		break;
	case 's':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
			suffix = argv[pos+1];
		break;
	case 'v':
		if (argc > 2)
			badSyntax();
		else
			showVersion();
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
		<< "Options\n"
		<< "\t/h\tBrings up this help dialog\n\n"
		<< "\t/d\tDirectory to rename (defaults to current directory)\n\n"		
		<< "\t/f\tFile extension (defaults to *)\n\n"
		<< "\t/o\tOriginal string to be replaced\n\n"
		<< "\t/n\tNew string to replace original\n\n"
		<< "\t/p\tAdd prefix to file names\n\n"
		<< "\t/r\tString to remove\n\n"
		<< "\t/s\tAdd suffix to file names\n\n"
		<< "\t/v\tDisplays Batch Rename Version Number\n"
		;
	exit(0);
}
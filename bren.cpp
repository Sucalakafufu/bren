#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <direct.h>
#include <Windows.h>
#include <stdio.h>

using namespace std;

const static string VERSIONNUM = "1.8.3.2";

//global vars
string dir, extension, singleFileEdit, addExtension, oldDIR, newDIR, needsSubString;
bool repeatAction, hasParam, capitalize, ALLCAPS, allLower, includeThis;

//iterators
int advancedFileItr, insertingItr, prefixItr, replacingItr, removeTheseItr, removeSectionItr, searchItr, suffixItr, whitespaceItr;

vector<string> vectorStringBuffer;
vector<bool> insertIs;
vector<string> advancedFiles;
vector<string> advancedNewFiles;
vector<string> excludeThese;
vector<string> params;
vector<string> startDeletes;
vector<string> endDeletes;
vector<string> insertHeres;
vector<string> insertThese;
vector<string> prefixes;
vector<string> suffixes;
vector<string> searchThese;
vector<string> searchNumbers;
vector<string> whitespaceOptions;
vector<vector<string>> removeThese;
vector<vector<string>> replaceOriginals;
vector<vector<string>> replaceNews;

//functions
char checkParam(int, char *);
void storeParam(int, char, int, char*[]);
bool nextIsParamOrBlank(int, int, char *[]);
bool stringEquals(string, string);
bool stringCaseEquals(string, string);
bool isPosNum(string);
void getFiles(vector<string>&, string);
bool hasInt(vector<int>,int);
bool hasString(vector<string>, string);
vector<int> includesString(vector<string>, string);
string removeExtension(string);
void removeExtensions(vector<string>&);
vector<int> renameAtLocations;
string findExtension(string);
void findExtensions(vector<string>, vector<string>&);
string removeWhitespace(string, string);
void resetItrs();
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
	string::size_type position;

	//initialize vars
	dir = getCurrentDir();

	extension = "*";
	needsSubString.clear();
	removeThese.clear();
	singleFileEdit.clear();
	addExtension.clear();
	oldDIR.clear();
	newDIR.clear();
	repeatAction = false;
	hasParam = true;
	capitalize = false;
	ALLCAPS = false;
	allLower = false;
	includeThis = true;
	resetItrs();

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
				if (!params.empty())
				{				
					if (argv[count][1] == 'S' || argv[count][1] == 'i' || argv[count][1] == 'I' || argv[count][1] == 'n' || argv[count][1] == 'D'
						|| argv[count][1] == 'a' || argv[count][1] == 'M')
					{
						if (!nextIsParamOrBlank(count,argc,argv))
							count++;

						if (!nextIsParamOrBlank(count,argc,argv))
							count++;
					}
					else if (argv[count][1] == 'd' || argv[count][1] == 'e' || argv[count][1] == 'F' || argv[count][1] == 'f' || argv[count][1] == 'H'
						|| argv[count][1] == 'p' || argv[count][1] == 's' || argv[count][1] == 'w') {
						if (!nextIsParamOrBlank(count,argc,argv))
							count++;
					}
					else if (argv[count][1] == 'r')
					{
						for (unsigned int i = 0; i < removeThese.at(removeTheseItr-1).size(); i++)
						{
							count++;
						}
					}
					else if (argv[count][1] == 'E')
					{
						for (unsigned int i = 0; i < excludeThese.size(); i++)
						{
							count++;
						}
					}
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
		renameAtLocations = includesString(files, needsSubString);
		renamed = files;
	}
	else
	{
		files.push_back(singleFileEdit);
		findExtensions(files,extensions);
		renamed = files;
	}	

	resetItrs();

	for (unsigned int paramItr = 0; paramItr < params.size(); paramItr++)
	{

	#pragma region replacing
		//replacing
		if (stringCaseEquals(params.at(paramItr), "n"))
		{		
			if ((!replaceOriginals.at(replacingItr).empty() && replaceNews.at(replacingItr).empty()) || (replaceOriginals.at(replacingItr).empty() && !replaceNews.at(replacingItr).empty()))
				badSyntax();
			else if (!replaceOriginals.at(replacingItr).empty() && !replaceNews.at(replacingItr).empty())
			{
				if (extension != "*")
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (stringEquals(extension, extensions.at(i)))
							{
								if (repeatAction)
								{
									for (unsigned int j = 0; j < replaceOriginals.at(replacingItr).size(); j++)
									{						
										while (renamed.at(i).find(replaceOriginals.at(replacingItr).at(j)) != string::npos)
										{
											renamed.at(i).replace(renamed.at(i).find(replaceOriginals.at(replacingItr).at(j)), replaceOriginals.at(replacingItr).at(j).length(), replaceNews.at(replacingItr).at(j));
											rename(files.at(i).c_str(), renamed.at(i).c_str());
											files = renamed;
										}
									}
								}
								else
								{
									for (unsigned int j = 0; j < replaceOriginals.at(replacingItr).size(); j++)
									{					
										if (renamed.at(i).find(replaceOriginals.at(replacingItr).at(j)) != string::npos)
										{
											renamed.at(i).replace(renamed.at(i).find(replaceOriginals.at(replacingItr).at(j)), replaceOriginals.at(replacingItr).at(j).length(), replaceNews.at(replacingItr).at(j));
											rename(files.at(i).c_str(), renamed.at(i).c_str());	
											files = renamed;
										}
									}
								}
							}
						}
						includeThis = true;
					}
				}
				else
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (repeatAction)
							{
								for (unsigned int j = 0; j < replaceOriginals.at(replacingItr).size(); j++)
								{
									while (renamed.at(i).find(replaceOriginals.at(replacingItr).at(j)) != string::npos)
									{
										renamed.at(i).replace(renamed.at(i).find(replaceOriginals.at(replacingItr).at(j)), replaceOriginals.at(replacingItr).at(j).length(), replaceNews.at(replacingItr).at(j));
										rename(files.at(i).c_str(), renamed.at(i).c_str());
										files = renamed;
									}
								}
							}
							else
							{
								for (unsigned int j = 0; j < replaceOriginals.at(replacingItr).size(); j++)
								{
									if (renamed.at(i).find(replaceOriginals.at(replacingItr).at(j)) != string::npos)
									{
										renamed.at(i).replace(renamed.at(i).find(replaceOriginals.at(replacingItr).at(j)), replaceOriginals.at(replacingItr).at(j).length(), replaceNews.at(replacingItr).at(j));
										rename(files.at(i).c_str(), renamed.at(i).c_str());
										files = renamed;
									}
								}
							}
						}
						includeThis = true;
					}
				}
			}
			replacingItr++;
		}
	#pragma endregion

	#pragma region removing
		//removing
		else if (stringCaseEquals(params.at(paramItr), "r"))
		{
			if (!removeThese.at(removeTheseItr).empty())
			{
				if (extension != "*")
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (stringEquals(extension, extensions.at(i)))
							{
								if (repeatAction)
								{
									for (unsigned int j = 0; j < removeThese.at(removeTheseItr).size(); j++)
									{
										while (renamed.at(i).find(removeThese.at(removeTheseItr).at(j)) != string::npos)
										{
											renamed.at(i).replace(renamed.at(i).find(removeThese.at(removeTheseItr).at(j)), removeThese.at(removeTheseItr).at(j).length(), "");
											rename(files.at(i).c_str(), renamed.at(i).c_str());
											files = renamed;
										}
									}
								}
								else
								{
									for (unsigned int j = 0; j < removeThese.at(removeTheseItr).size(); j++)
									{
										if (renamed.at(i).find(removeThese.at(removeTheseItr).at(j)) != string::npos)
										{
											renamed.at(i).replace(renamed.at(i).find(removeThese.at(removeTheseItr).at(j)), removeThese.at(removeTheseItr).at(j).length(), "");
											rename(files.at(i).c_str(), renamed.at(i).c_str());
											files = renamed;
										}
									}
								}
							}
						}
						includeThis = true;
					}
				}
				else
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (repeatAction)
							{
								for (unsigned int j = 0; j < removeThese.at(removeTheseItr).size(); j++)
								{
									while (renamed.at(i).find(removeThese.at(removeTheseItr).at(j)) != string::npos)
									{
										renamed.at(i).replace(renamed.at(i).find(removeThese.at(removeTheseItr).at(j)), removeThese.at(removeTheseItr).at(j).length(), "");
										rename(files.at(i).c_str(), renamed.at(i).c_str());
										files = renamed;
									}
								}
							}
							else
							{
								for (unsigned int j = 0; j < removeThese.at(removeTheseItr).size(); j++)
								{
									if (renamed.at(i).find(removeThese.at(removeTheseItr).at(j)) != string::npos)
									{
										renamed.at(i).replace(renamed.at(i).find(removeThese.at(removeTheseItr).at(j)), removeThese.at(removeTheseItr).at(j).length(), "");
										rename(files.at(i).c_str(), renamed.at(i).c_str());
										files = renamed;
									}
								}
							}
						}
						includeThis = true;
					}
				}
			}
			removeTheseItr++;
		}
	#pragma endregion

	#pragma region removing section
		//removing section
		
		else if (stringCaseEquals(params.at(paramItr), "D"))
		{
			if ((!startDeletes.at(removeSectionItr).empty() && endDeletes.at(removeSectionItr).empty()) || (startDeletes.at(removeSectionItr).empty() && !endDeletes.at(removeSectionItr).empty()))
				badSyntax();
			else if (!startDeletes.at(removeSectionItr).empty() && !endDeletes.at(removeSectionItr).empty())
			{
				if (extension != "*")
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (stringEquals(extension, extensions.at(i)))
							{					
								if (startDeletes.at(removeSectionItr) == endDeletes.at(removeSectionItr))
									renamed.at(i).erase(renamed.at(i).begin()+atoi(startDeletes.at(removeSectionItr).c_str())-1);
								else
									renamed.at(i).erase(renamed.at(i).begin()+atoi(startDeletes.at(removeSectionItr).c_str())-1,renamed.at(i).begin()+atoi(endDeletes.at(removeSectionItr).c_str()));
								rename(files.at(i).c_str(), renamed.at(i).c_str());	
								files = renamed;
							}
						}
						includeThis = true;
					}
				}
				else
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (startDeletes.at(removeSectionItr) == endDeletes.at(removeSectionItr))
								renamed.at(i).erase(renamed.at(i).begin()+atoi(startDeletes.at(removeSectionItr).c_str())-1);
							else
								renamed.at(i).erase(renamed.at(i).begin()+atoi(startDeletes.at(removeSectionItr).c_str())-1,renamed.at(i).begin()+atoi(endDeletes.at(removeSectionItr).c_str()));
							rename(files.at(i).c_str(), renamed.at(i).c_str());
							files = renamed;
						}
						includeThis = true;
					}
				}
			}
			removeSectionItr++;
		}
	#pragma endregion

	#pragma region inserting
		//inserting
		else if (stringEquals(params.at(paramItr), "i"))
		{
			if ((!insertHeres.at(insertingItr).empty() && insertThese.at(insertingItr).empty()) || (insertHeres.at(insertingItr).empty() && !insertThese.at(insertingItr).empty()))
				badSyntax();
			else if (!insertHeres.at(insertingItr).empty() && !insertThese.at(insertingItr).empty())
			{
				if (extension != "*")
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (stringEquals(extension, extensions.at(i)))
							{				
								if (!insertIs.at(insertingItr))
								{
									renamed.at(i).insert(renamed.at(i).begin()+atoi(insertHeres.at(insertingItr).c_str())-1,insertThese.at(insertingItr).begin(),insertThese.at(insertingItr).end());
									rename(files.at(i).c_str(), renamed.at(i).c_str());
								}
								else
								{
									position = renamed.at(i).find(insertHeres.at(insertingItr));
									if (position != string::npos)
									{
										renamed.at(i).insert(renamed.at(i).begin()+position,insertThese.at(insertingItr).begin(),insertThese.at(insertingItr).end());
										rename(files.at(i).c_str(), renamed.at(i).c_str());
									}
								}					
								files = renamed;
							}
						}
						includeThis = true;
					}
				}
				else
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (!insertIs.at(insertingItr))
							{
								renamed.at(i).insert(renamed.at(i).begin()+atoi(insertHeres.at(insertingItr).c_str())-1,insertThese.at(insertingItr).begin(),insertThese.at(insertingItr).end());
								rename(files.at(i).c_str(), renamed.at(i).c_str());
							}
							else
							{
								position = renamed.at(i).find(insertHeres.at(insertingItr));
								if (position != string::npos)
								{
									renamed.at(i).insert(renamed.at(i).begin()+position,insertThese.at(insertingItr).begin(),insertThese.at(insertingItr).end());
									rename(files.at(i).c_str(), renamed.at(i).c_str());
								}
							}
							files = renamed;
						}
						includeThis = true;
					}
				}
			}
			insertingItr++;
		}
	#pragma endregion

	#pragma region prefix
		//prefix
		else if (stringCaseEquals(params.at(paramItr), "p"))
		{
			if (!prefixes.at(prefixItr).empty())
			{
				if (extension != "*")
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (stringEquals(extension, extensions.at(i)))
							{
								renamed.at(i) = prefixes.at(prefixItr) + renamed.at(i);
								rename(files.at(i).c_str(), renamed.at(i).c_str());	
								files = renamed;
							}
						}
						includeThis = true;
					}
				}
				else
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{	
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							renamed.at(i) = prefixes.at(prefixItr) + renamed.at(i);
							rename(files.at(i).c_str(), renamed.at(i).c_str());
							files = renamed;
						}
						includeThis = true;
					}
				}
			}
			prefixItr++;
		}
	#pragma endregion

	#pragma region suffix
		//suffix
		else if (stringCaseEquals(params.at(paramItr), "s"))
		{
			if (!suffixes.at(suffixItr).empty())
			{
				unsigned int count;
				if (extension != "*")
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (stringEquals(extension, extensions.at(i)))
							{			
								count = 1;
								while (count<renamed.at(i).length()&&*(renamed.at(i).end()-count)!='.')
									count++;

								if (count == renamed.at(i).length())
									renamed.at(i) = renamed.at(i) + suffixes.at(suffixItr);
								else
								{
									renamed.at(i).insert(renamed.at(i).end()-count,suffixes.at(suffixItr).begin(),suffixes.at(suffixItr).end());
								}
								rename(files.at(i).c_str(), renamed.at(i).c_str());
								files = renamed;
							}
						}
						includeThis = true;
					}
				}
				else
				{
					for (unsigned int i = 0; i < files.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							count = 1;
							while (count<renamed.at(i).length()&&*(renamed.at(i).end()-count)!='.')
								count++;

							if (count == renamed.at(i).length())
								renamed.at(i) = renamed.at(i) + suffixes.at(suffixItr);
							else
							{
								renamed.at(i).insert(renamed.at(i).end()-count,suffixes.at(suffixItr).begin(),suffixes.at(suffixItr).end());
							}
							rename(files.at(i).c_str(), renamed.at(i).c_str());
							files = renamed;
						}
						includeThis = true;
					}
				}
			}
			suffixItr++;
		}
	#pragma endregion

	#pragma region add extension
		//add extension
		else if (stringCaseEquals(params.at(paramItr), "e"))
		{
			if (!addExtension.empty())
			{
				if (extension != "*")
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (stringEquals(extension, extensions.at(i)))
							{			
								renamed.at(i) = renamed.at(i) + addExtension;
								rename(files.at(i).c_str(), renamed.at(i).c_str());
								files = renamed;
							}
						}
						includeThis = true;
					}
				}
				else
				{
					for (unsigned int i = 0; i < files.size(); i++)
					{	
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							renamed.at(i) = renamed.at(i) + addExtension;
							rename(files.at(i).c_str(), renamed.at(i).c_str());
							files = renamed;
						}
						includeThis = true;
					}
				}
			}
		}
	#pragma endregion

	#pragma region search
		//search
		else if (stringCaseEquals(params.at(paramItr), "S"))
		{
			if (!searchThese.at(searchItr).empty())
			{
				if (repeatAction)
					badSyntax();
				else if (extension != "*")
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (stringEquals(extension, extensions.at(i)))
							{				
								position = renamed.at(i).find(searchThese.at(searchItr));
								if (atoi(searchNumbers.at(searchItr).c_str()) > 1)
								{
									for (int j = 1; j < atoi(searchNumbers.at(searchItr).c_str()); j++)
										position = renamed.at(i).find(searchThese.at(searchItr), position+1);
								}
								if (position != string::npos)
									position++;

								if (position == string::npos)
									cout << "\nDid not find \"" + searchThese.at(searchItr) + "\" in " + renamed.at(i);
								else
									cout << "\nFound \"" + searchThese.at(searchItr) + "\" in " + renamed.at(i) + " at position: " << position;
							}
						}
						includeThis = true;
					}
				}
				else
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{	
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							position = renamed.at(i).find(searchThese.at(searchItr));
							if (atoi(searchNumbers.at(searchItr).c_str()) > 1)
							{
								for (int j = 1; j < atoi(searchNumbers.at(searchItr).c_str()); j++)
									position = renamed.at(i).find(searchThese.at(searchItr), position+1);
							}
							if (position != string::npos)
								position++;

							if (position == string::npos)
								cout << "\nDid not find \"" + searchThese.at(searchItr) + "\" in " + renamed.at(i);
							else
								cout << "\nFound \"" + searchThese.at(searchItr) + "\" in " + renamed.at(i) + " at position: " << position;
						}
						includeThis = true;
					}
				}
			}
			searchItr++;
		}
	#pragma endregion

	#pragma region all lower
		//all lower
		else if (stringCaseEquals(params.at(paramItr), "L"))
		{
			if (allLower)
			{
				if (extension != "*")
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (stringEquals(extension, extensions.at(i)))
							{
								for (unsigned int j = 0; j < renamed.at(i).size()-extensions.at(i).size(); j++)
								{
									if (isalpha(renamed[i][j]))
									{
										renamed[i][j] = tolower(renamed[i][j]);
										rename(files.at(i).c_str(), renamed.at(i).c_str());
										files = renamed;
									}
								}					
							}
						}
						includeThis = true;
					}
				}
				else
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						} 

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							for (unsigned int j = 0; j < renamed.at(i).size()-extensions.at(i).size(); j++)
							{
								if (isalpha(renamed[i][j]))
								{
									renamed[i][j] = tolower(renamed[i][j]);
									rename(files.at(i).c_str(), renamed.at(i).c_str());
									files = renamed;						
								}
							}
						}
						includeThis = true;
					}
				}
			}
		}
	#pragma endregion 

	#pragma region capitalize
		//capitalize
		else if (stringCaseEquals(params.at(paramItr), "c"))
		{
			if (capitalize)
			{
				if (extension != "*")
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (stringEquals(extension, extensions.at(i)))
							{
								bool found = false;
								for (unsigned int j = 0; j < renamed.at(i).size()-extensions.at(i).size(); j++)
								{
									if (found && !isalpha(renamed[i][j]))
										found = false;
									else if (!found && isalpha(renamed[i][j]))
									{
										renamed[i][j] = toupper(renamed[i][j]);
										rename(files.at(i).c_str(), renamed.at(i).c_str());
										files = renamed;
										found = true;
									}
								}					
							}
						}
						includeThis = true;
					}
				}
				else
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							bool found = false;
							for (unsigned int j = 0; j < renamed.at(i).size()-extensions.at(i).size(); j++)
							{
								if (found && !isalpha(renamed[i][j]))
									found = false;
								else if (!found && isalpha(renamed[i][j]))
								{
									renamed[i][j] = toupper(renamed[i][j]);
									rename(files.at(i).c_str(), renamed.at(i).c_str());
									files = renamed;
									found = true;
								}
							}
						}
						includeThis = true;
					}
				}
			}
		}
	#pragma endregion

	#pragma region ALLCAPS
		//ALLCAPS
		else if (stringCaseEquals(params.at(paramItr), "C"))
		{
			if (ALLCAPS)
			{
				if (extension != "*")
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (stringEquals(extension, extensions.at(i)))
							{
								for (unsigned int j = 0; j < renamed.at(i).size()-extensions.at(i).size(); j++)
								{
									if (isalpha(renamed[i][j]))
									{
										renamed[i][j] = toupper(renamed[i][j]);
										rename(files.at(i).c_str(), renamed.at(i).c_str());
										files = renamed;
									}
								}					
							}
						}
						includeThis = true;
					}
				}
				else
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							for (unsigned int j = 0; j < renamed.at(i).size()-extensions.at(i).size(); j++)
							{
								if (isalpha(renamed[i][j]))
								{
									renamed[i][j] = toupper(renamed[i][j]);
									rename(files.at(i).c_str(), renamed.at(i).c_str());
									files = renamed;						
								}
							}
						}
						includeThis = true;
					}
				}
			}
		}
	#pragma endregion 

	#pragma region whitespace
		//remove whitespace
		else if (stringCaseEquals(params.at(paramItr), "w"))
		{
			if (!whitespaceOptions.at(whitespaceItr).empty())
			{
				if (repeatAction)
					badSyntax();
				else if (extension != "*")
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							if (stringEquals(extension, extensions.at(i)))
							{
								renamed.at(i) = removeWhitespace(renamed.at(i), whitespaceOptions.at(whitespaceItr));
								rename(files.at(i).c_str(), renamed.at(i).c_str());
								files = renamed;
							}
						}
						includeThis = true;
					}
				}
				else
				{
					for (unsigned int i = 0; i < renamed.size(); i++)
					{	
						if (!needsSubString.empty())
						{
							if (!hasInt(renameAtLocations, i))
								includeThis = false;
						}

						if (!hasString(excludeThese, renamed.at(i)) && includeThis)
						{
							renamed.at(i) = removeWhitespace(renamed.at(i), whitespaceOptions.at(whitespaceItr));
							rename(files.at(i).c_str(), renamed.at(i).c_str());
							files = renamed;
						}
						includeThis = true;
					}
				}
			}
			whitespaceItr++;
		}
	#pragma endregion

	#pragma region advanced rename
		//advanced rename
		else if (stringCaseEquals(params.at(paramItr), "a"))
		{
			if (!advancedFiles.at(advancedFileItr).empty() && !advancedNewFiles.at(advancedFileItr).empty())
			{
				advancedNewFiles.at(advancedFileItr) = advancedNewFiles.at(advancedFileItr)+findExtension(advancedFiles.at(advancedFileItr));
				rename(advancedFiles.at(advancedFileItr).c_str(), advancedNewFiles.at(advancedFileItr).c_str());
				files = renamed;
			}
			advancedFileItr++;
		}
	#pragma endregion

	#pragma region directory rename
		//rename directory files to other directory file names
		else if (stringCaseEquals(params.at(paramItr), "M"))
		{
			if (!oldDIR.empty() && !newDIR.empty())
			{
				getFiles(oldFiles, oldDIR); getFiles(newFiles, newDIR);
				if (oldFiles.size() != newFiles.size())
				{
					cout << "\nCannot rename files unless both directories have an equal number of files.\n";
					exit(0);
				}

				cout << "\nFile names will be renamed as such:\n\n";
				for (unsigned int i = 0; i < oldFiles.size(); i++)
				{
					cout << newFiles.at(i) + " -> " + removeExtension(oldFiles.at(i)) + findExtension(newFiles.at(i)) + "\n";
				}
				cout << "\nContinue with operation? (Y\\N): ";
				string verify; cin >> verify;
				bool verifying = true;

				while (verifying)
				{
					if (verify == "Y" || verify == "y")
					{
						_chdir(newDIR.c_str());
						for (unsigned int i=0; i<oldFiles.size(); i++)
						{
							string buffer = removeExtension(oldFiles.at(i)) + findExtension(newFiles.at(i));
							rename(newFiles.at(i).c_str(), buffer.c_str());
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
		}
	#pragma endregion

	#pragma region series rename
		//rename files based on a series
	#pragma endregion

	}
}

void getFiles(vector<string> &files, string fileDIR)
{
	ifstream fin;
	string file;
	string brenFile = ".bren";
	string command;

	command = "DIR /B \"" + fileDIR + "\" > \"" + brenFile + "\"";
	system(command.c_str());

	fin.open(brenFile);
	while (getline(fin,file))
	{
		if (file != brenFile)
			files.push_back(file);
	}
	fin.close(); fin.clear();

	remove(brenFile.c_str());
}

bool hasInt(vector<int> searchVector, int hasThis)
{
	for (unsigned int i = 0; i < searchVector.size(); i++)
	{
		if (searchVector.at(i) == hasThis)
			return true;
	}

	return false;
}

bool hasString(vector<string> searchVector, string hasThis)
{
	for (unsigned int i = 0; i < searchVector.size(); i++)
	{
		if (stringEquals(searchVector.at(i), hasThis))
			return true;
	}

	return false;
}

vector<int> includesString(vector<string> searchVector, string includesThis)
{
	vector<int> stringLocations;

	for (unsigned int i = 0; i < searchVector.size(); i++)
	{
		if (searchVector.at(i).find(includesThis) != string::npos)
			stringLocations.push_back(i);
	}

	return stringLocations;
}

int stringLocation(string searchString, string findThis) {
	size_t location = searchString.find(findThis);

	if (location != string::npos)
		return (int)location;

	return -1;
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
		removeExtension(files.at(i));
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

string removeWhitespace(string file, string option)
{
	if (option.empty())
		return file;
	else if (option == "l")
	{
		while (file.at(0) == ' ' && file.at(0) != string::npos)
			file.replace(file.begin(), file.begin()+1, "");
	}
	else if (option == "t")
	{
		string ext = findExtension(file);

		if (ext.empty())
		{
			while (*(file.end()-1) == ' ' && file.end() != file.begin())
				file.replace(file.end()-1, file.end(), "");
		}
		else
		{
			while (*(file.end()-ext.length()-1) == ' ' && file.end()-ext.length() != file.begin())
				file.replace(file.end()-ext.length()-1, file.end()-ext.length(), "");
		}
	}
	else if (option == "*")
	{
		while (file.find(" ") != string::npos)
			file.replace(file.find(" "), 1, "");
	}

	return file;
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
			params.push_back("h");
		}
		break;
	case 'a':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else if (!nextIsParamOrBlank(pos+1,argc,argv))
			advancedNewFiles.push_back(argv[pos+2]);

		advancedFiles.push_back(argv[pos+1]);
		params.push_back("a");
		break;
	case 'c':
		capitalize = true;
		params.push_back("c");
		break;
	case 'C':
		ALLCAPS = true;
		params.push_back("C");
		break;
	case 'd':
		if (nextIsParamOrBlank(pos, argc, argv))
			break;
		else
		{
			dir = argv[pos+1];
			while (dir.find("\\") != string::npos)
				dir.replace(dir.find("\\"),1,"/");
			params.push_back("d");
		}
		break;
	case 'D':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;		
		else if (!nextIsParamOrBlank(pos+1,argc,argv))
		{
			endDeletes.push_back(argv[pos+2]);

			if (!isPosNum(endDeletes.back()))
				badSyntax();
		}

		startDeletes.push_back(argv[pos+1]);
		if (!isPosNum(startDeletes.back()))
			badSyntax();		

		if (atoi(endDeletes.back().c_str()) < atoi(startDeletes.back().c_str()))
			badSyntax();

		params.push_back("D");
		break;
	case 'e':
		if (nextIsParamOrBlank(pos, argc, argv))
			break;
		else if (!addExtension.empty())
			break;
		else
		{
			addExtension = argv[pos+1];
			params.push_back("e");
		}
		break;
	case 'E':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
		{
			int i = 0;
			while (!nextIsParamOrBlank(pos+i,argc,argv))
			{
				excludeThese.push_back(argv[pos+1+i]);
				i++;
				params.push_back("E");
			}
		}
		break;
	case 'f':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
		{
			extension = ".";
			extension = extension + argv[pos+1];
			params.push_back("f");
		}
		break;
	case 'F':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
		{
			singleFileEdit = argv[pos+1];
			params.push_back("F");
		}
		break;
	case 'H':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
		{
			needsSubString = argv[pos+1];
			params.push_back("H");
		}
		break;
	case 'i':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else if (!nextIsParamOrBlank(pos+1,argc,argv))
		{
			insertHeres.push_back(argv[pos+2]);
			if (!isPosNum(insertHeres.back()))
				badSyntax();			
		}

		insertThese.push_back(argv[pos+1]);
		params.push_back("i");
		insertIs.push_back(false);
		break;
	case 'I':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else if (!nextIsParamOrBlank(pos+1,argc,argv))
			insertHeres.push_back(argv[pos+2]);

		insertThese.push_back(argv[pos+1]);
		params.push_back("I");
		insertIs.push_back(true);
		break;
	case 'L':
		allLower = true;
		params.push_back("L");
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
		{
			replaceNews.push_back(vectorStringBuffer);
			replaceNews.at(replacingItr).push_back(argv[pos+2]);
		}

		replaceOriginals.push_back(vectorStringBuffer);
		replaceOriginals.at(replacingItr).push_back(argv[pos+1]);
		params.push_back("n");
		replacingItr++;
		break;
	case 'p':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
		{
			prefixes.push_back(argv[pos+1]);
			params.push_back("p");
		}
		break;
	case 'r':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
		{
			int i = 0;
			removeThese.push_back(vectorStringBuffer);
			while (!nextIsParamOrBlank(pos+i,argc,argv))
			{
				removeThese.at(removeTheseItr).push_back(argv[pos+1+i]);
				i++;				
			}
			params.push_back("r");
			removeTheseItr++;
		}
		break;	
	case 'R':
		repeatAction = true;
		params.push_back("R");
		break;
	case 's':
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else
		{
			suffixes.push_back(argv[pos+1]);
			params.push_back("s");
		}
		break;
	case 'S':
		searchNumbers.push_back("1");
		if (nextIsParamOrBlank(pos,argc,argv))
			break;
		else if (!nextIsParamOrBlank(pos+1,argc,argv))
		{
			searchNumbers.back() = argv[pos+2];
			if (!isPosNum(searchNumbers.back()))
				badSyntax();			
		}
		searchThese.push_back(argv[pos+1]);
		params.push_back("S");
		break;
	case 'v':
		if (argc > 2)
			badSyntax();
		else
		{
			showVersion();
			params.push_back("v");
		}
		break;
	case 'w':
		if (!nextIsParamOrBlank(pos, argc, argv))
		{
			whitespaceOptions.push_back(argv[pos+1]);
			if (whitespaceOptions.back().length() > 1 || (whitespaceOptions.back() != "l" && whitespaceOptions.back() != "t"))
				badSyntax();			
		}
		else
			whitespaceOptions.push_back("*");
		params.push_back("w");
		break;
	default:
		badSyntax();
	}
}

bool nextIsBlank(int pos, int argc, char *argv[])
{
	if (pos+1 >= argc)
		return true;
	else
		return false;
}

bool nextIsParam(int pos, int argc, char *argv[])
{
	if (argv[pos+1][0] == '/')
		return true;
	else 
		return false;
}

bool nextIsParamOrBlank(int pos, int argc, char *argv[])
{
	if (nextIsBlank(pos, argc, argv))
		return true;
	else if (nextIsParam(pos, argc, argv))
		return true;
	else
		return false;
}

bool stringCaseEquals(string original, string compare)
{
	if (original == compare)
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

void resetItrs()
{
	advancedFileItr = 0;
	insertingItr = 0;
	prefixItr = 0;
	removeSectionItr = 0;
	removeTheseItr = 0;
	replacingItr = 0;	
	searchItr = 0;
	suffixItr = 0;
	whitespaceItr = 0;
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
		<< " /E <filename(s)>\t\tExclude file(s) from renaming\n\n"
		<< " /f <extension>\t\t\tFile extension to rename (defaults to *)\n\n"
		<< " /F <filename>\t\t\tRename only this file\n\n"
		<< " /H <string>\t\t\tRename files that have string in filename\n\n"
		<< " /i <value> <location>\t\tInsert at location\n\n"
		<< " /I <value> <string>\t\tInsert at string location\n\n"
		<< " /L\t\t\t\tAll lowercase\n\n"
		<< " /M <old Dir> <new Dir>\t\tAttempts to rename new Dir files to the old Dir\n\t\t\t\tfile names in alpha order\n\n"
		<< " /n <string> <value>\t\tReplace string with value\n\n"
		<< " /p <prefix>\t\t\tAdd prefix to file names\n\n"
		<< " /r <string(s)>\t\t\tRemove string(s) from file names\n\n"
		<< " /R\t\t\t\tRepeat action through whole file\n\n"
		<< " /s <suffix>\t\t\tAdd suffix to file names\n\t\t\t\t(Attempts to append suffix before extension)\n\n"
		<< " /S <string> <iteration>\tSearch for string position\n\t\t\t\t(Iteration Optional)\n\n"
		<< " /v\t\t\t\tDisplays Batch Rename Version Number\n\n"
		<< " /w <options>\t\t\tRemove whitespace\n\t\t\t\t(Options: l = leading, t = trailing)\n\t\t\t\tBlank options will remove all whitespace\n\n"
		;
	exit(0);
}
//============================================================================
// Name        : SWC.cpp
// Author      : Michael Bastos
// Version     :
// Copyright   : This is open source software
// Description : C++ Crawler
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <locale>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <sys/types.h>
#include <algorithm>   // remove_if()
#include <cctype>      // isspace()
#include <functional>  // ptr_fun <>
//#include <boost/regex.hpp>
using namespace std;

vector<string> page;

void downloadUrl (const char *path, string url, string urlhash);
void loadPage(const char *p, string url, string urlhash);
void parsePage(vector<string> page, string pattern);
void parseLine(string line, string pattern);
void processArguments(std::string inputs);
void checkForCacheFolder();
void generateUrlHash(std::string inputs);
void parseArgument(std::string argument);
string delSpaces(string &str);
void regexLine(string line, string pattern);

std::string match(const char *string, char *pattern) {
// Adapted from: http://pubs.opengroup.org/onlinepubs/009695399/functions/regcomp.html
    int    status;
    regex_t    re;
    regmatch_t rm;

    if (regcomp(&re, pattern, REG_EXTENDED) != 0) {
        return "Bad pattern";
    }
    status = regexec(&re, string, 1, &rm, 0);
    regfree(&re);
    if (status != 0) {
        return "No Match";
    }
    return std::string(string+rm.rm_so, string+rm.rm_eo);
}

void checkForCacheFolder(){
	if(mkdir("cache",0777)==-1)//creating a directory
		{
		        //cerr<<"Caching..." <<endl;
		}
}

void processArguments(std::string inputs){
	if(inputs.find("SWC") != string::npos){
		//Check for application argument ./SWC
	} else if(inputs.find("http://") != string::npos || inputs.find("https://") != string::npos){
		generateUrlHash(inputs);
	} else {
		parseArgument(inputs);
	}
}

void parseArgument(std::string argument){
	cout << argument << endl;
	parsePage(page, argument);

}

void generateUrlHash(std::string url){
	locale loc;
	const collate<char>& coll = use_facet<collate<char> >(loc);
	long urlhash = coll.hash(url.data(),url.data()+url.length());
	std::string hash;
	std::stringstream strstream;
	strstream << urlhash;
	strstream >> hash;
	string path = "cache/" + hash;
	const char *p;
	p=path.c_str();
	//cout << p << endl;
	cout << " " << url << " " << hash << endl;
	loadPage(p,url,hash);
}

void loadPage(const char *path, string url, string urlhash){
	//page.clear();
	string line;
	ifstream read (path);//reading a file
	if (read.is_open()) {
		while (! read.eof() ) {
			getline (read,line);
			page.push_back (line);
			cout<<line<<endl;
	    }
	    read.close();
	} else {
		downloadUrl(path,url,urlhash);
	}
}

void downloadUrl (const char *path, string url, string urlhash){
	std::string command = "cd cache && wget " + url + " --output-document=" + urlhash + " --continue --force-html";
	system(command.c_str());
	//cout << command << endl;
	loadPage(path,url,urlhash);
}

void parsePage(vector<string> page, string pattern){
	for (vector<string>::iterator line = page.begin();line != page.end();++line)
	{
		parseLine(*line, pattern);
	}

}

void parseLine(string line, string pattern){

		//delSpaces(line);
		//const char *l;
			//l=line.c_str();
		//char *pat;
		//	pat=pattern.c_str();
		//std::cout << match(*l, *pat) << "\n";
		//cout << l << endl;
		regexLine(line, pattern);
}

string delSpaces(string &line)
{
	std::string::iterator end_pos = std::remove(line.begin(), line.end(), ' ');
	line.erase(end_pos, line.end());
	return line;
}

void regexLine(string line, string pattern){
		regex_t reg;

			//string pattern = "[^tpr]{2,}";

			regmatch_t matches[1];

			regcomp(&reg,pattern.c_str(),REG_EXTENDED|REG_ICASE);

			if (regexec(&reg,line.c_str(),1,matches,0)==0) {
			  //cout << "Match ";
			  cout << line.substr(matches[0].rm_so,matches[0].rm_eo-matches[0].rm_so);
			  //cout << " found starting at: ";
			  //cout << matches[0].rm_so;
			  //cout << " and ending at ";
			  //cout << matches[0].rm_eo;
			  //cout << " - " << line;
			  cout << endl;
			} else {
			  //cout << "Match not found.";
			  //cout << endl;
			}
			regfree(&reg);


}

//int main(int ac, char **av) {
    // e.g. usage: ./program abcdefg 'c.*f'
    //std::cout << match(av[1], av[2]) << "\n";
//}

int main(int argc, char *argv[]) {
	checkForCacheFolder();
    for (int i=0; i < argc; i++){
        processArguments(argv[i]);
    }
    return 0;
}

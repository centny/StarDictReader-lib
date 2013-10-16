/*
 * Sdr.h
 *
 *  Created on: Aug 28, 2013
 *      Author: cny
 */

#ifndef SDR_H_
#define SDR_H_
#include<string>
#include<fstream>
#include<iostream>
#include<map>
#include<sstream>
namespace sdr {
using namespace std;
#define EDX_HSIZE 12
#define CDX_HSIZE 12

void int2byte(int val, char* buf);
void int2byte2(int val, unsigned char* buf);
void long2byte(long val, char* buf);
int byte2int(char* buf);
int byte2int2(unsigned char* buf);
long byte2long(char* buf, int count = 8);
string fsub(string& s, char t);
//
struct SdrRes {
	long edxBeg;
	long edxLen;
	long idxBeg;
	long idxEnd;
	string matched;
	string word;
	string content;
	string msg;
};
//
class Sdr {
private:
	string rpath;
	string name;
	fstream *idx;
	fstream *dict;
	//
	istream *edx;
	fstream *edx_f;
	stringstream *edx_s;
	//
	int edxCharSize; //the edx char size.
	int edxOffsetSize; //the edx offset size.
	int edxEntryCount; //the edx entry count.
	int edxEntrySize; //the edx entry size.
	map<string, string> infoes;
	bool medx; //load the edx file to memory.
	//

private:

public:
	//constructor,rpath:dict root path,name:dict file name,medx:if load edx to memory.
	Sdr(string rpath, string name, bool medx = false);
	virtual ~Sdr();
	string idxpath(); //the .idx path.
	string edxpath(); //the .edx path.
//	string cdxpath(); //the .cdx path.
	string dictpath(); //the .dict path.
	string infopath(); //the .ifo path.
	//
	//get the value in .ifo by name.
	string info(string key);
	//get the dict version.
	string version();
	//get the word count in dict.
	int wordcount();
	//get idx file size.
	long idxfilesize();
	//get the book name.
	string bookname();
	//the .dict format type.
	string sametypesequence();
	//check if dict version less tart version.
	bool lessVersion(string tver);
	//
	//load dict info,return error message,return empty string when success.
	string loadDictInfo();
    //check if dict already loaded.
    bool isDictLoaded();
	//load dict,return error message,return empty string when success.
	string loadDict();
	//free the dict.
	void unloadDict();
	//find the dict by word.
	SdrRes find(string word);
	//find the dict by .idx offset.
	SdrRes find(long beg_idx, long end_idx, string word);
	//find the m type dict.
	string dictm(long beg, long size);
	//
//	string loadCdx();
	//create the .edx file by edx size,oft64:if use 64 bit offset.
	string createEdx(int ecount, bool oft64 = false);
//	string createCdx(int ccount, bool oft64 = false);
};

} /* namespace sdr */
#endif /* SDR_H_ */

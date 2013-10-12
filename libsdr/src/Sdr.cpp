/*
 * Sdr.cpp
 *
 *  Created on: Aug 28, 2013
 *      Author: cny
 */

#include "Sdr.h"
namespace sdr {
void long2byte(long val, char* buf) {
	for (int i = 0; i < 8; i++) {
		buf[7 - i] = (unsigned char) (val >> (i * 8));
	}
}
void int2byte(int val, char* buf) {
	for (int i = 0; i < 4; i++) {
		buf[3 - i] = (unsigned char) (val >> (i * 8));
	}
}
void int2byte2(int val, unsigned char* buf) {
	for (int i = 0; i < 4; i++) {
		buf[3 - i] = (unsigned char) (val >> (i * 8));
	}
}
long byte2long(char* buf, int count) {
	long val = 0;
	for (int i = 0; i < count; i++) {
		val += (unsigned char) buf[i];
		if (i < count - 1) {
			val = val << 8;
		}
	}
	return val;
}
int byte2int(char* buf) {
	int val = 0;
	for (int i = 0; i < 4; i++) {
		val += (unsigned char) buf[i];
		if (i < 3) {
			val = val << 8;
		}
	}
	return val;
}
int byte2int2(unsigned char* buf) {
	int val = 0;
	for (int i = 0; i < 4; i++) {
		val += (unsigned char) buf[i];
		if (i < 3) {
			val = val << 8;
		}
	}
	return val;
}

bool fexist(const char* path) {
	fstream fs_(path, ios::in);
	bool res = fs_.is_open();
	fs_.close();
	return res;
}
int ccmp_c(const char* a, const char* b, int count) {
	int dis = 'a' - 'A';
	char ta[count], tb[count];
	for (int i = 0; i < count; i++) {
		if (a[i] >= 'A' && a[i] <= 'Z') {
			ta[i] = a[i] + dis;
		} else {
			ta[i] = a[i];
		}
		if (b[i] >= 'A' && b[i] <= 'Z') {
			tb[i] = b[i] + dis;
		} else {
			tb[i] = b[i];
		}
	}
	return strncmp(ta, tb, count);
}
string fsub(string& s, char t) {
	size_t idx = s.find(t);
	string sub;
	if (idx == string::npos) {
		sub = s;
		s = "";
	} else {
		sub = s.substr(0, idx);
		if (idx == s.length() - 1) {
			s = "";
		} else {
			s = s.substr(idx + 1);
		}
	}
	return sub;
}
Sdr::Sdr(string rpath, string name, bool medx) :
		medx(medx) {
	this->rpath = rpath;
	this->name = name;
	this->edx = 0;
	this->idx = this->dict = this->edx_f = 0;
	this->edx_s = 0;
	this->edxCharSize = this->edxEntryCount = this->edxEntrySize = 0;
	this->edxOffsetSize = 0;
}

Sdr::~Sdr() {
	this->unloadDict();
}
string Sdr::idxpath() {
	return this->rpath + "/" + this->name + ".idx";
}
string Sdr::edxpath() {
	return this->rpath + "/" + this->name + ".edx";
}
//string Sdr::cdxpath() {
//	return this->rpath + "/" + this->name + ".cdx";
//}
string Sdr::dictpath() {
	return this->rpath + "/" + this->name + ".dict";
}
string Sdr::infopath() {
	return this->rpath + "/" + this->name + ".ifo";
}
string Sdr::info(string key) {
	return this->infoes[key];
}
string Sdr::version() {
	return this->info("version");
}
int Sdr::wordcount() {
	return atoi(this->info("wordcount").c_str());
}
long Sdr::idxfilesize() {
	return atol(this->info("idxfilesize").c_str());
}
string Sdr::bookname() {
	return this->info("bookname");
}
string Sdr::sametypesequence() {
	return this->info("sametypesequence");
}
bool Sdr::lessVersion(string tver) {
	if (tver.length() < 1) {
		return false;
	}
	string sver = this->version();
	for (int i = 0; i < 3; i++) {
		string a = fsub(sver, '.');
		string b = fsub(tver, '.');
		int va = atoi(a.c_str());
		int vb = atoi(b.c_str());
		if (va == vb) {
			continue;
		} else {
			return va < vb;
		}
	}
	return false;
}
//
string Sdr::loadDictInfo() {
	string fpath = this->infopath();
	fstream info(fpath.c_str(), ios::in | ios::binary);
	if (!info.is_open()) {
		return "open .ifo error:" + fpath;
	}
	string sbuf;
	getline(info, sbuf);
	while (!info.eof()) {
		sbuf.clear();
		getline(info, sbuf);
		size_t p = sbuf.find("=");
		if (p == string::npos) {
			continue;
		}
		this->infoes[sbuf.substr(0, p)] = sbuf.substr(p + 1);
	}
	info.close();
	return "";
}
//
string Sdr::loadDict() {
	string epath = this->edxpath();
	string ipath = this->idxpath();
	string dpath = this->dictpath();

	this->edx_f = new fstream(epath.c_str(), ios::in);
	if (!this->edx_f->is_open()) {
		this->unloadDict();
		return "open .edx error:" + epath;
	}
	char cbuf[2048];
	if (this->medx) {
		this->edx_s = new stringstream();
		while (!this->edx_f->eof()) {
			this->edx_f->read(cbuf, 2048);
			size_t rsize = this->edx_f->gcount();
			this->edx_s->write(cbuf, rsize);
		}
		this->edx_f->close();
		delete this->edx_f;
		this->edx_s->seekg(0);
		this->edx_f = 0;
		this->edx = this->edx_s;
	} else {
		this->edx = this->edx_f;
	}
	this->edx->seekg(0);
	this->edx->read(cbuf, EDX_HSIZE);
	this->edxCharSize = byte2int(cbuf);
	this->edxOffsetSize = byte2int(cbuf + 4);
	this->edxEntryCount = byte2int(cbuf + 8);
	this->edxEntrySize = this->edxCharSize + this->edxOffsetSize;
	this->idx = new fstream(ipath.c_str(), ios::in);
	if (!this->idx->is_open()) {
		this->unloadDict();
		return "open .idx error:" + ipath;
	}
	this->dict = new fstream(dpath.c_str(), ios::in);
	if (!this->dict->is_open()) {
		this->unloadDict();
		return "open .dict error:" + dpath;
	}
	return "";
}
void Sdr::unloadDict() {
	if (this->dict) {
		this->dict->close();
		delete this->dict;
		this->dict = 0;
		this->dict = 0;
	}
	if (this->edx_f) {
		this->edx_f->close();
		delete this->edx_f;
		this->edx_f = 0;
		this->edx = 0;
	}
	if (this->edx_s) {
		delete this->edx_s;
		this->edx_s = 0;
		this->edx = 0;
	}
	if (this->idx) {
		this->idx->close();
		delete this->idx;
		this->idx = 0;
		this->idx = 0;
	}
}
//
SdrRes Sdr::find(string word) {
    SdrRes nfound = { 0, 0, 0, 0, "", word, "", "not found" };
    if(!this->edx){
        nfound.msg="edx not initial.";
        return nfound;
    }
	const char* tword = word.c_str();
	int wsize = word.length();
	int beg = 0, end = this->edxEntryCount;
	char bufa[100], bufb[100];
	int cmp = wsize > this->edxCharSize ? this->edxCharSize : word.size();
	//reset edx stream state.
	this->edx->clear();
	//check the begin if matched.
	this->edx->seekg(EDX_HSIZE);
	this->edx->read(bufa, this->edxEntrySize);
	if (ccmp_c(bufa, tword, cmp) == 0) {
		this->edx->read(bufb, this->edxEntrySize);
		return this->find(
				byte2long(bufa + this->edxCharSize, this->edxOffsetSize),
				byte2long(bufb + this->edxCharSize, this->edxOffsetSize), word);
	}
	//check the end if matched.
	this->edx->seekg(
			this->edxEntryCount * this->edxEntrySize + EDX_HSIZE
					- this->edxEntrySize);
	this->edx->read(bufb, this->edxEntrySize);
	if (ccmp_c(bufb, tword, cmp) == 0) {
		return this->find(
				byte2long(bufb + this->edxCharSize, this->edxOffsetSize), -1,
				word);
	}
	int center, cres, oft;
	while (end - beg > 1) {
		center = (end + beg) / 2;
		oft = center * this->edxEntrySize + EDX_HSIZE;
		this->edx->seekg(oft);
		this->edx->read(bufa, this->edxEntrySize);
		cres = ccmp_c(bufa, tword, cmp);
		if (cres == 0) {
			if (cmp == this->edxCharSize) {
				this->edx->read(bufb, this->edxEntrySize);
				SdrRes res = this->find(
						byte2long(bufa + this->edxCharSize,
								this->edxOffsetSize),
						byte2long(bufb + this->edxCharSize,
								this->edxOffsetSize), word);
				res.edxBeg = center;
				res.edxLen = 1;
				return res;
			} else {
				int tbeg, tend, tcenter;
				//find the real begin.
				tbeg = beg;
				tend = center;
				while (tend - tbeg > 1) {
					tcenter = (tend + tbeg) / 2;
					oft = tcenter * this->edxEntrySize + EDX_HSIZE;
					this->edx->seekg(oft);
					this->edx->read(bufa, this->edxEntrySize);
					if (ccmp_c(bufa, tword, cmp) == 0) {
						tend = tcenter;
					} else {
						tbeg = tcenter;
					}
				}
				int rbeg = tend;
				//find the real end.
				tbeg = center;
				tend = end;
				while (tend - tbeg > 1) {
					tcenter = (tend + tbeg) / 2;
					oft = tcenter * this->edxEntrySize + EDX_HSIZE;
					this->edx->seekg(oft);
					this->edx->read(bufa, this->edxEntrySize);
					if (ccmp_c(bufa, tword, cmp) == 0) {
						tend = tcenter;
					} else {
						tbeg = tcenter;
					}
				}
				int rend = tend;
				//
				oft = rbeg * this->edxEntrySize + EDX_HSIZE;
				this->edx->seekg(oft);
				this->edx->read(bufa, this->edxEntrySize);
				oft = rend * this->edxEntrySize + EDX_HSIZE;
				this->edx->seekg(oft);
				this->edx->read(bufb, this->edxEntrySize);
				SdrRes res = this->find(
						byte2long(bufa + this->edxCharSize,
								this->edxOffsetSize),
						byte2long(bufb + this->edxCharSize,
								this->edxOffsetSize), word);
				res.edxBeg = rbeg;
				res.edxLen = rend - rbeg;
				return res;
			}
		} else if (cres < 0) {
			beg = center;
		} else {
			end = center;
		}
	}
	return nfound;
}
SdrRes Sdr::find(long beg_idx, long end_idx, string word) {
	cout << "find idx<<" << endl;
	SdrRes nfound = { 0, 0, 0, 0, "", word, "", "not found" };
	long idxsize = this->idxfilesize();
	if (end_idx < 0) {
		end_idx = idxsize;
	}
	if (beg_idx >= end_idx || end_idx >= idxsize) {
		return nfound;
	}
	this->idx->seekg(beg_idx);
	string sbuf;
	char cbuf[1024];
	const char* tword = word.c_str();
	int wsize = word.length();
	int idx_size = this->lessVersion("3.0.0") ? 4 : 8;
	while (this->idx->tellg() < end_idx) {
		sbuf.clear();
		getline(*this->idx, sbuf, '\0');
		this->idx->read(cbuf, idx_size + 4);
		if (ccmp_c(sbuf.c_str(), tword, wsize) == 0) {
			string mtype = this->sametypesequence();
			if (mtype == "m") {
				long dofft = byte2long(cbuf, idx_size);
				int dsize = byte2int(cbuf + idx_size);
				string dstr = this->dictm(dofft, dsize);
				SdrRes res = { 0, 0, beg_idx, end_idx, sbuf, word, dstr, "" };
				return res;
			} else {
				cout << "not implement:" << mtype << endl;
			}
			break;
		}
	}
	return nfound;
}
string Sdr::dictm(long beg, long size) {
	char cbuf[size];
	this->dict->seekg(beg);
	this->dict->read(cbuf, size);
	return string(cbuf, size);
}
//
//string Sdr::loadCdx() {
//	string cpath = this->cdxpath();
//	if (!fexist(cpath.c_str())) {
//		return "cdx file not exist";
//	}
//	fstream cfs(cpath.c_str(), ios::in);
//	if (!cfs.is_open()) {
//		cfs.close();
//		return "open cdx file error:" + cpath;
//	}
//	char cbuf[100];
//	cfs.read(cbuf, CDX_HSIZE);
//	int ccount = byte2int(cbuf);
//	int offs_len = byte2int(cbuf + 4);
//	int csize = byte2int(cbuf + 8);
//	while (!cfs.eof()) {
//		cfs.read(cbuf, ccount + 4);
//		int esize = byte2int(cbuf + ccount);
//		cout << cbuf << "," << esize << endl;
//		char ibuf[offs_len];
//		for (int i = 0; i < esize; i++) {
//			cfs.read(ibuf, offs_len);
//		}
//	}
//	cout << csize << endl;
//	return "";
//}
//
string Sdr::createEdx(int ecount, bool oft64) {
	string lres = this->loadDictInfo();
	if (lres.length()) {
		return lres;
	}
	string epath = this->edxpath();
	string ipath = this->idxpath();
	if (fexist(epath.c_str())) {
		return "edx file already exist";
	}
	fstream ifs(ipath.c_str(), ios::in);
	if (!ifs.is_open()) {
		ifs.close();
		return "open .idx file error:" + ipath;
	}
	fstream efs(epath.c_str(), ios::out);
	if (!efs.is_open()) {
		efs.close();
		return "open edx file error:" + epath;
	}
	string sbuf;
	int offs_len = oft64 ? 8 : 4;
	int cbuf_len = ecount + offs_len;
	int rofts = this->lessVersion("3.0.0") ? 4 : 8;
	char cbuf[cbuf_len];
	memset(cbuf, 0, cbuf_len);
	char tbuf[16];
	long offset = 0;
	int esize = 0;
	int ssize = 0;
	long loft = 0;
	//
	efs.seekp(12);
	//read the first line.
	getline(ifs, sbuf, '\0');
	memset(cbuf, 0, cbuf_len);
	ssize = sbuf.length();
	memcpy(cbuf, sbuf.c_str(), ssize < ecount ? ssize : ecount);
	ifs.read(tbuf, rofts + 4);
	while (1) {
		loft = ifs.tellg();
		sbuf.clear();
		getline(ifs, sbuf, '\0');
		ifs.read(tbuf, rofts + 4);
		if (ccmp_c(cbuf, sbuf.c_str(), ecount) == 0 && !ifs.eof()) {
			continue;
		}
		if (oft64) {
			long2byte(offset, cbuf + ecount);
		} else {
			int2byte(offset, cbuf + ecount);
		}
		efs.write(cbuf, cbuf_len);
//		cout << cbuf << endl;
		//
		memset(cbuf, 0, cbuf_len);
		ssize = sbuf.length();
		memcpy(cbuf, sbuf.c_str(), ssize < ecount ? ssize : ecount);
		offset = loft;
		esize++;
		if (ifs.eof()) {
			break;
		}
	}
	int2byte(ecount, tbuf);
	int2byte(offs_len, tbuf + 4);
	int2byte(esize, tbuf + 8);
	efs.seekp(0);
	efs.write(tbuf, 12);
	efs.close();
	ifs.close();
	return "";
}
//string Sdr::createCdx(int ccount, bool oft64) {
//	string lres = this->loadDictInfo();
//	if (lres.length()) {
//		return lres;
//	}
//	string cpath = this->cdxpath();
//	string ipath = this->idxpath();
//	if (fexist(cpath.c_str())) {
//		return "cdx file already exist";
//	}
//	fstream ifs(ipath.c_str(), ios::in);
//	if (!ifs.is_open()) {
//		ifs.close();
//		return "open .idx file error:" + ipath;
//	}
//	fstream cfs(cpath.c_str(), ios::out);
//	if (!cfs.is_open()) {
//		cfs.close();
//		return "open cdx file error:" + cpath;
//	}
//	string sbuf;
//	int offs_len = oft64 ? 8 : 4;
//	int cbuf_len = ccount + offs_len;
//	int rofts = this->lessVersion("3.0.0") ? 4 : 8;
//	char cbuf[cbuf_len];
//	memset(cbuf, 0, cbuf_len);
//	char tbuf[16];
//	long offset = 0;
//	long poffset = 0;
//	long loft = 0;
//	int esize = 0;
//	int ssize = 0;
//	int csize = 0;
//	//
//	cfs.seekp(12);
//	poffset = offset = cfs.tellp();
//	//
//	while (1) {
//		loft = ifs.tellg();
//		sbuf.clear();
//		getline(ifs, sbuf, '\0');
//		ifs.read(tbuf, rofts + 4);
//		esize++;
//		if (ccmp_c(cbuf, sbuf.c_str(), ccount) == 0 && !ifs.eof()) {
//			if (oft64) {
//				long2byte(loft, tbuf);
//			} else {
//				int2byte(loft, tbuf);
//			}
//			cfs.write(tbuf, rofts);
//			continue;
//		}
//		offset = cfs.tellp();
//		if (offset > poffset) {
//			cfs.seekp(poffset + ccount);
//			int2byte(esize, tbuf);
//			cfs.write(tbuf, 4);
//			cfs.seekp(offset);
//			poffset = offset;
//		}
//		memset(cbuf, 0, cbuf_len);
//		ssize = sbuf.length();
//		memcpy(cbuf, sbuf.c_str(), ssize < ccount ? ssize : ccount);
//		int2byte(esize, cbuf + ccount);
//		cfs.write(cbuf, cbuf_len);
//		csize++;
//		//
//		if (oft64) {
//			long2byte(loft, cbuf);
//		} else {
//			int2byte(loft, cbuf);
//		}
//		cfs.write(cbuf, rofts);
//		if (ifs.eof()) {
//			break;
//		}
//	}
//	int2byte(ccount, tbuf);
//	int2byte(offs_len, tbuf + 4);
//	int2byte(csize, tbuf + 8);
//	cfs.seekp(0);
//	cfs.write(tbuf, 12);
//	cfs.close();
//	ifs.close();
//	return "";
//}
} /* namespace sdr */

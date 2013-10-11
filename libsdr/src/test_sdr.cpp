/*
 * test_sdr.cpp
 *
 *  Created on: Aug 30, 2013
 *      Author: cny
 */
#include "Sdr.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
using namespace std;
void testSdrFind() {
	sdr::Sdr ist("/Users/cny/Tmp/stardict-langdao-ec-gb-2.4.2", "langdao-ec-gb", true);
	cout << ist.createEdx(4, true) << endl;
//	cout << ist.createCdx(1, false) << endl;
//	ist.loadCdx();
	cout << ist.loadDict() << endl;
	cout << ist.version() << endl;
	cout << ist.lessVersion("3.0.0") << "  " << ist.lessVersion("2.0.0")
			<< endl;
	for (int i = 0; i < 26; i++) {
		char v = 'z' - i;
//		ist.find("x");
//		v = 'w';
		string word(&v, 1);
		cout << word << endl;
		sdr::SdrRes res = ist.find(word);
		cout << res.matched << endl;
		cout << res.content << endl;
		cout << res.msg << endl;
//		sleep(1);
	}
}
void testConvert() {
	char buf[8];
	unsigned char ubuf[8];
	int vala = 257, valb;
	//
	sdr::int2byte(vala, buf);
	valb = sdr::byte2int(buf);
	assert(vala == valb);
	//
	sdr::int2byte2(vala, ubuf);
	valb = sdr::byte2int2(ubuf);
	assert(vala == valb);
	//
	int lva = 100, lvb;
	//
	sdr::long2byte(lva, buf);
	lvb = sdr::byte2long(buf);
	assert(lva == lvb);
	//
	cout << "test end" << endl;
}
void testfsub() {
	string ss, a, b;
	ss = "sss.sd.oo";
	a = sdr::fsub(ss, '.');
	b = sdr::fsub(ss, '.');
	cout << a << "," << b << "," << ss << endl;
	//
	ss = ".sd.oo";
	a = sdr::fsub(ss, '.');
	b = sdr::fsub(ss, '.');
	cout << a << "," << b << "," << ss << endl;
	//
	ss = ".";
	a = sdr::fsub(ss, '.');
	b = sdr::fsub(ss, '.');
	cout << a << "," << b << "," << ss << endl;
	//
	ss = "b";
	a = sdr::fsub(ss, '.');
	b = sdr::fsub(ss, '.');
	cout << a << "," << b << "," << ss << endl;
	//
	ss = "";
	a = sdr::fsub(ss, '.');
	b = sdr::fsub(ss, '.');
	cout << a << "," << b << "," << ss << endl;
}

void testSdr() {
//	testConvert();
//	testfsub();
	testSdrFind();
}


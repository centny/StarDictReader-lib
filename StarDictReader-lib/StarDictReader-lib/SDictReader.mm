//
//  SDictReader.m
//  StartDictReader-ios
//
//  Created by Cny on 8/31/13.
//  Copyright (c) 2013 Scorpion. All rights reserved.
//

#import "SDictReader.h"
#include "Sdr.h"
using namespace sdr;
using namespace std;
@implementation SDictRes

@end
//
@interface SDictReader ()
@property(nonatomic) Sdr *sdr;
@end

NSString *stoNSString(string s)
{
	if (s.empty()) {
		return nil;
	} else {
		return [NSString stringWithUTF8String:s.c_str()];
	}
}

//
SDictRes *toDictRes(SdrRes sres)
{
	SDictRes *dres = [[SDictRes alloc]init];

	dres.edxBeg		= sres.edxBeg;
	dres.edxLen		= sres.edxLen;
	dres.idxBeg		= sres.idxBeg;
	dres.idxEnd		= sres.idxEnd;
	dres.matched	= stoNSString(sres.matched);
	dres.content	= stoNSString(sres.content);
	dres.msg		= stoNSString(sres.msg);
	//
	return dres;
}

//
@implementation SDictReader
+ (id)sdictReaderWithRPath:(NSString *)rpath{
    NSString* fname=[SDictReader findDictFName:rpath];
    if(fname){
        return [[SDictReader alloc]initWithRPath:rpath fname:fname];
    }else{
        NSLog(@"Erro:not a invalid path for stardict,%@",rpath);
        return nil;
    }
}
- (id)initWithRPath:(NSString *)rpath fname:(NSString *)fname
{
	self = [super init];

	if (self) {
		self.sdr = new Sdr([rpath UTF8String], [fname UTF8String]);
        _rpath=rpath;
        _fname=fname;
        _medx=NO;
	}

	return self;
}
+ (id)sdictReaderWithRPath:(NSString *)rpath medx:(BOOL)medx{
    NSString* fname=[SDictReader findDictFName:rpath];
    if(fname){
        return [[SDictReader alloc]initWithRPath:rpath fname:fname medx:medx];
    }else{
        NSLog(@"Erro:not a invalid path for stardict,%@",rpath);
        return nil;
    }
}
- (id)initWithRPath:(NSString *)rpath fname:(NSString *)fname medx:(BOOL)medx
{
	self = [super init];

	if (self) {
		self.sdr = new Sdr([rpath UTF8String], [fname UTF8String], medx);
        _rpath=rpath;
        _fname=fname;
        _medx=medx;
	}

	return self;
}

- (NSString *)idxpath
{
	return stoNSString(self.sdr->idxpath());
}

- (NSString *)edxpath
{
	return stoNSString(self.sdr->edxpath());
}

- (NSString *)dictpath
{
	return stoNSString(self.sdr->dictpath());
}

- (NSString *)infopath
{
	return stoNSString(self.sdr->infopath());
}

//
// get the value in .ifo by name.
- (NSString *)info:(NSString *)key
{
	return stoNSString(self.sdr->info([key UTF8String]));
}

// get the dict version.
- (NSString *)version
{
	return stoNSString(self.sdr->version());
}

// get the word count in dict.
- (int)wordcount
{
	return self.sdr->wordcount();
}

// get idx file size.
- (long)idxfilesize
{
	return self.sdr->idxfilesize();
}

// get the book name.
- (NSString *)bookname
{
	return stoNSString(self.sdr->bookname());
}

// the .dict format type.
- (NSString *)sametypesequence
{
	return stoNSString(self.sdr->sametypesequence());
}

// check if dict version less tart version.
- (bool)lessVersion:(NSString *)tver
{
	return self.sdr->lessVersion([tver UTF8String]);
}

//
// load dict info,return error message,return empty string when success.
- (NSString *)loadDictInfo
{
	return stoNSString(self.sdr->loadDictInfo());
}

// load dict,return error message,return empty string when success.
- (NSString *)loadDict
{
	return stoNSString(self.sdr->loadDict());
}

// free the dict.
- (void)unloadDict
{
	self.sdr->unloadDict();
}

// find the dict by word.
- (SDictRes *)find:(NSString *)word
{
	SdrRes sres = self.sdr->find([word UTF8String]);

	return toDictRes(sres);
}

// find the dict by .idx offset.
- (SDictRes *)find:(long)beg_idx end:(long)end_idx word:(NSString *)word
{
	SdrRes sres = self.sdr->find(beg_idx, end_idx, [word UTF8String]);

	return toDictRes(sres);
}

// find the m type dict.
- (NSString *)dictm:(long)beg size:(long)size
{
	return stoNSString(self.sdr->dictm(beg, size));
}

//
// create the .edx file by edx size,oft64:if use 64 bit offset.
- (NSString *)createEdx:(int)ecount oft64:(bool)oft64
{
	return stoNSString(self.sdr->createEdx(ecount, oft64));
}

- (NSString *)createEdx:(int)ecount
{
	return stoNSString(self.sdr->createEdx(ecount));
}

- (void)dealloc
{
	delete self.sdr;
}

+ (NSString *)findDictFName:(NSString *)rpath
{
	NSFileManager	*fm		= [NSFileManager defaultManager];
	BOOL			isdir	= false;

	if (![fm fileExistsAtPath:rpath isDirectory:&isdir] || !isdir) {
		return @"";
	}
	NSMutableArray	*nary	= [NSMutableArray array];

	for (NSString *path in [fm contentsOfDirectoryAtPath:rpath error:nil]) {
		if ([path hasSuffix:@".idx"] || [path hasSuffix:@".ifo"] || [path hasSuffix:@".dict"]) {
			[nary addObject:[path stringByDeletingPathExtension]];
		}
	}
    if(nary.count<3){
        return @"";
    }
    if([[nary objectAtIndex:0]isEqualToString:[nary objectAtIndex:1]]&&[[nary objectAtIndex:1]isEqualToString:[nary objectAtIndex:2]]){
        return [nary objectAtIndex:0];
    }else{
        return @"";
    }
}

@end


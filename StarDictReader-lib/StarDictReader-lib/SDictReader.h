//
//  SDictReader.h
//  StartDictReader-ios
//
//  Created by Cny on 8/31/13.
//  Copyright (c) 2013 Scorpion. All rights reserved.
//

#import <UIKit/UIKit.h>
@interface SDictRes : NSObject
@property(nonatomic) long				edxBeg;
@property(nonatomic) long				edxLen;
@property(nonatomic) long				idxBeg;
@property(nonatomic) long				idxEnd;
@property(nonatomic, retain) NSString	*matched;
@property(nonatomic, retain) NSString	*word;
@property(nonatomic, retain) NSString	*content;
@property(nonatomic, retain) NSString	*msg;
@end
//
@interface SDictReader : NSObject
@property(nonatomic, readonly) NSString *fname;
@property(nonatomic, readonly) NSString *rpath;
@property(nonatomic, readonly) BOOL		medx;
//
+ (id)sdictReaderWithRPath:(NSString *)rpath;
- (id)initWithRPath:(NSString *)rpath fname:(NSString *)fname;
+ (id)sdictReaderWithRPath:(NSString *)rpath medx:(BOOL)medx;
- (id)initWithRPath:(NSString *)rpath fname:(NSString *)name medx:(BOOL)medx;
//
- (NSString *)idxpath;
- (NSString *)edxpath;
- (NSString *)dictpath;
- (NSString *)infopath;
//
// get the value in .ifo by name.
- (NSString *)info:(NSString *)key;
// get the dict version.
- (NSString *)version;
// get the word count in dict.
- (int)wordcount;
// get idx file size.
- (long)idxfilesize;
// get the book name.
- (NSString *)bookname;
// the .dict format type.
- (NSString *)sametypesequence;
// check if dict version less tart version.
- (bool)lessVersion:(NSString *)tver;
//
// load dict info,return error message,return empty string when success.
- (NSString *)loadDictInfo;
// load dict,return error message,return empty string when success.
- (NSString *)loadDict;
// free the dict.
- (void)unloadDict;
// find the dict by word.
- (SDictRes *)find:(NSString *)word;
// find the dict by .idx offset.
- (SDictRes *)find:(long)beg_idx end:(long)end_idx word:(NSString *)word;
// find the m type dict.
- (NSString *)dictm:(long)beg size:(long)size;
//
// create the .edx file by edx size,oft64:if use 64 bit offset.
- (NSString *)createEdx:(int)ecount oft64:(bool)oft64;
- (NSString *)createEdx:(int)ecount;

//
+ (NSString *)findDictFName:(NSString *)rpath;
@end


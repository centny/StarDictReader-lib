//
//  SDictReader.m
//  StartDictReader-ios
//
//  Created by Cny on 8/31/13.
//  Copyright (c) 2013 Scorpion. All rights reserved.
//

#import "SDictReader.h"
#include "Sdr.h"
@interface SDictReader ()
@property(nonatomic)sdr::Sdr * sdr;
@end
//
@implementation SDictReader
- (id)initWithRPath:(NSString *)rpath name:(NSString *)name medx:(BOOL)medx
{
	self = [super init];

	if (self) {
        self.sdr=new sdr::Sdr([rpath UTF8String],[name UTF8String],medx);
    }

	return self;
}
-(void)dealloc{
    delete self.sdr;
}
@end


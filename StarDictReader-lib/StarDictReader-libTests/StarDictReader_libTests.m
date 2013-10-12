//
//  StarDictReader_libTests.m
//  StarDictReader-libTests
//
//  Created by Cny on 10/11/13.
//  Copyright (c) 2013 Scorpion. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "SDictReader.h"
@interface StarDictReader_libTests : XCTestCase

@end

@implementation StarDictReader_libTests

- (void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testExample
{
    SDictReader* reader=[[SDictReader alloc]initWithRPath:@"/Users/cny/Tmp/stardict-langdao-ec-gb-2.4.2" name:@"langdao-ec-gb"];
    NSString* msg;
    msg=[reader createEdx:4];
    msg=[reader loadDict];
    if(msg){
        NSLog(@"%@",msg);
        return;
    }
    NSLog(@"%@,%d",[reader version],[reader wordcount]);
    SDictRes* res=[reader find:@"a"];
    NSLog(@"%@,%@",res.content,res.msg);
}

@end

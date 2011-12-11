//
//  gclTests.m
//  gclTests
//
//  Created by Francois Coulombe on 12/4/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//


#include "AssertTest.h"
#include "CurveTest.h"
#include "ExceptionTest.h"
#include "HashTest.h"
#include "Matrix44Test.h"
#include "PathTest.h"
#include "Point2Test.h"
#include "Point3Test.h"
#include "Point4Test.h"
#include "SafeCastTest.h"
#include "StringUtilTest.h"
#include "WarningTest.h"

#import "gclTests.h"

@implementation gclTests

- (void)setUp
{
    [super setUp];
    
    // Set-up code here.
}

- (void)tearDown
{
    // Tear-down code here.
    
    [super tearDown];
}

- (void)testExample
{
    try
    {
        WarningTest::Test();
        ExceptionTest::Test();
        AssertTest::Test();
        SafeCastTest::Test();
        HashTest::Test();
        StringUtilTest::Test();
        PathTest::Test();
        
        Point2Test::Test();
        Point3Test::Test();
        Point4Test::Test();
        
        Matrix44Test::Test();
        
        CurveTest::Test();
    }
    catch (GCLException & e)
    {
        std::stringstream str;
        str << "[FAILED] " << "gcltest" << std::endl;
        str << e.what();
        std::cerr << str.str();
        //STFail(str.c_str());
//        const char *cstr = 
        NSString *nstr;
        nstr= [NSString stringWithCString:str.str().c_str() encoding:NSUTF8StringEncoding];
        STFail(@"%@", nstr);
        
    }
    catch (...)
    {
        STFail(@"[FAILED] gclTest. Something went wrong");
    }
    if (sNumTestFailures)
        STFail(@"[FAILED] gclTest. Something went wrong");
    
}

@end

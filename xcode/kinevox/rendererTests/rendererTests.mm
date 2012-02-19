//
//  rendererTests.m
//  rendererTests
//
//  Created by Francois Coulombe on 12/4/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//


#include "CameraTest.h"
#include "FrameBufferTest.h"
#include "GeomUtilTest.h"
#include "GLRendererTest.h"
#include "MaterialTest.h"
#include "PngLoadingTest.h"
#include "RenderBufferTest.h"
#include "RenderObjectTest.h"
#include "RenderObjectWithMaterialTest.h"
#include "ResourceTest.h"
#include "ShaderTest.h"
#include "TextureTest.h"
#include "TextureAndShaderTest.h"
#include "TgaLoadingTest.h"
#include "VertexBufferTest.h"
#include "VertexTest.h"


#import "rendererTests.h"


@implementation rendererTests

- (void)setUp
{
    [super setUp];
    
    // Set-up code here.
    NSFileManager *filemgr;
    NSString *currentpath;
    
    filemgr = [NSFileManager defaultManager];
    
    currentpath = [filemgr currentDirectoryPath];
    
    NSLog (@"Current directory is %@", currentpath);
    NSString *filePath = [[NSBundle mainBundle] pathForResource:@"data/mushroom" ofType:@"tga"];  
    NSLog (@"Current directory is %@", filePath);
}

- (void)tearDown
{
    // Tear-down code here.
    
    [super tearDown];
}

- (void)testExample
{
    std::cout << "[TEST] renderer_test"  << std::endl;
    
    try
    {
        GLRendererTest::Test();
        CameraTest::Test();
        
        ShaderTest::Test();
        VertexTest::Test();
        VertexBufferTest::Test();
        TgaLoadingTest::Test();
        PngLoadingTest::Test();
        ResourceTest::Test();
        TextureTest::Test();
        TextureAndShaderTest::Test();
        MaterialTest::Test();
        
        RenderObjectTest::Test();
        RenderObjectWithMaterialTest::Test();
        
        RenderBufferTest::Test();
        FrameBufferTest::Test();
        GeomUtilTest::Test();
        
    }
    catch (GCLException & e)
    {
        std::stringstream str;
        str << "[FAILED] renderer_test" << std::endl;
        str << e.what();
        str << std::endl;
        NSString *nstr;
        nstr= [NSString stringWithCString:str.str().c_str() encoding:NSUTF8StringEncoding];
        STFail(@"%@", nstr);
    }
    catch (...)
    {
        std::stringstream str;
        str << "[FAILED] renderer_test"  << std::endl;
        str << "something went wrong" << std::endl;
        NSString *nstr;
        nstr= [NSString stringWithCString:str.str().c_str() encoding:NSUTF8StringEncoding];
        STFail(@"%@", nstr);

    }
    
}

@end

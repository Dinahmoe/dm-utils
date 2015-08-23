//
//  StringUtilities.cpp
//  dmaf-ios
//
//  Created by Alessandro Saccoia on 7/18/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "StringUtilities.hpp"
#include "Log.h"
#include <unistd.h>
#import <Foundation/Foundation.h>  

namespace dinahmoe {
namespace utils {

using namespace std;


  
std::string pathToFile(std::string file) {
  // TODO is this ok? this is a quick and dirty way of using absolute pathsx
  if (file[0] == '/') {
    return file;
  }

  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  
  NSString* nsDirectory = [[NSString stringWithCString:file.c_str() 
                                   encoding:[NSString defaultCStringEncoding]] stringByDeletingLastPathComponent];
                                   
  NSString* nsFilename = [[[NSString stringWithCString:file.c_str() 
                                   encoding:[NSString defaultCStringEncoding]] lastPathComponent] stringByDeletingPathExtension];
  NSString *nsExtension = [[[NSString stringWithCString:file.c_str() 
                                   encoding:[NSString defaultCStringEncoding]] lastPathComponent] pathExtension];                                   
  NSString * path = [[NSBundle mainBundle] pathForResource:  nsFilename ofType: nsExtension inDirectory: nsDirectory];  
  
  if (path == nil) {
    dmaf_log(Log::Error, "File %s not found in main bundle", file.c_str());
    return "";
  }
  
  std::string toReturn([path cStringUsingEncoding:1]);  
  [pool release];
  return toReturn;
}

std::string pathToFile(std::string file, std::string ext) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSString *nsFilename = [NSString stringWithCString:file.c_str() 
                                   encoding:[NSString defaultCStringEncoding]];
  NSString *nsExtension = [NSString stringWithCString:ext.c_str() 
                                   encoding:[NSString defaultCStringEncoding]];
                                   
  NSString * path = [[NSBundle mainBundle] pathForResource:  nsFilename ofType: nsExtension];
  
  if (path == nil) {
    dmaf_log(Log::Error, "File %s not found in main bundle", file.c_str());
    return "";
  }
  
  std::string toReturn([path cStringUsingEncoding:1]);
  [pool release];
  return toReturn;
}

std::string getFullPath(std::string file) {
  // TODO is this ok? this is a quick and dirty way of using absolute pathsx
  if (file[0] != '/') {
    static char buf[512];
    getcwd(buf, 512);
    file = buf + std::string("/") + file;
  }
  return file;
}

std::string getEnclosingDirectory(std::string file) {

  file = getFullPath(file);

  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  
  NSString* nsDirectory = [[NSString stringWithCString:file.c_str() 
                                   encoding:[NSString defaultCStringEncoding]] stringByDeletingLastPathComponent];
  if (nsDirectory == nil) {
    dmaf_log(Log::Error, "Error in finding the parent directory of ", file.c_str());
    return "";
  }
  
  std::string toReturn([nsDirectory cStringUsingEncoding:1]);
  toReturn += "/";
  [pool release];
  return toReturn;
}

}}

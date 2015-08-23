/*
 * Copyright (c) 2015, Dinahmoe. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL PETER THORSON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

//  Created by Alessandro Saccoia on 7/18/12.

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

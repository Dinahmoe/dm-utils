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
#include <algorithm>
#include <cassert>

namespace {

#define PATH_MAX 512
std::string getcwd_string( ) {
   char buff[PATH_MAX];
   getcwd( buff, PATH_MAX );
   std::string cwd( buff );
   return cwd;
}

}

namespace dinahmoe {
namespace utils {

using namespace std;

std::vector<std::string> tokenize(const std::string & str, const std::string & delim, bool toLower) {
  std::vector<std::string> tokens;

  size_t p0 = 0, p1 = std::string::npos;
  while(p0 != std::string::npos)
  {
    p1 = str.find_first_of(delim, p0);
    if(p1 != p0)
    {
      std::string token = str.substr(p0, p1 - p0);
      if (toLower) {
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
      }
      tokens.push_back(token);
    }
    p0 = str.find_first_not_of(delim, p1);
  }
  return tokens;
}

// returns the absolute path from the current working directory
std::string pathToFile(std::string file) {
  // TODO is this ok? this is a quick and dirty way of using absolute pathsx
  if (file[0] == '/') {
    return file;
  }

  return getcwd_string() + file;
}

std::string pathToFile(std::string file, std::string ext) {
  assert(false);
  return "";
}

std::string getEnclosingDirectory(std::string file) {
  // TODO is this ok? this is a quick and dirty way of using absolute pathsx
  if (!isAbsolute(file)) {
    dmaf_log(Log::Error, "The enclosing directory can be found just for absolute path. Pass %s to pathToFile before", file.c_str());
  }

  std::string toReturn = file.substr(0, file.find_last_of('/') + 1);

  return toReturn;
}


}}

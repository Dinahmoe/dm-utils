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

#ifndef dmaf_ios_StringUtilities_hpp
#define dmaf_ios_StringUtilities_hpp

#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <stdio.h>  /* defines FILENAME_MAX */
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <sys/stat.h>


#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
    #ifdef DM_PLATFORM_OSX
    #include <sys/types.h>
    #include <pwd.h>
    #include <uuid/uuid.h>
    #endif
#endif

namespace dinahmoe {
namespace utils {

template <template<typename...> class ContainerT = std::vector>
inline ContainerT<std::string> tokenize(const std::string & str, const std::string & delim, bool toLower = false) {
  ContainerT<std::string> tokens;

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

inline std::string concat(const std::vector<std::string>& tokens_, const std::string & delim_, size_t sIndex_, size_t eIndex_) {
  std::string reconstructedString("");
  for (size_t i = sIndex_; i < eIndex_; ++i) {
    reconstructedString += tokens_[i] + delim_;
  }
  reconstructedString += tokens_[eIndex_];
  return reconstructedString;
}

template <typename OutputType>
OutputType stringConverter(std::string fieldValue_) {
  std::stringstream m_idReader(fieldValue_);
  OutputType toReturn;
  m_idReader >> toReturn;
  if (m_idReader.fail()) {
    throw std::runtime_error("Parsing failed");
  }
  return toReturn;
}

template <typename InputType>
std::string toString(InputType input) {
  std::stringstream out;
  out << input;
  return out.str();
}

std::string getFullPath(std::string file);
std::string pathToFile(std::string file);
std::string pathToFile(std::string file, std::string ext);
std::string getEnclosingDirectory(std::string file);

inline bool isAbsolute(std::string file) {
  return file[0] == '/';
}

inline void ToUpper(const char* bufferIn, char* bufferOut, const int len) {
  for(int i = 0; i < len; ++i) {
    bufferOut[i] = toupper(bufferIn[i]);
  }
}

inline bool IsNumber(const std::string& s) {
  for (unsigned int i = 0; i < s.length(); i++) {
    if (!std::isdigit(s[i])) {
    	return false;
    }
  }
  return true;
}
  
#define xmlIntoPropertyWithDefault(name, actionName, property, type, default) \
  propertyElement = actionNode->FirstChildElement(name); \
  if (!propertyElement) { \
    dmaf_log(Log::Warning, "The " name " element is missing for action " actionName); \
    property = default; \
  } else { \
    retriever = propertyElement->GetText(); \
    if (retriever == NULL) { \
      dmaf_log(Log::Warning, "The " name " element is empty for action " actionName); \
      property = default; \
    } else { \
      property = stringConverter<type>(retriever); \
    } \
  }

// returns the date in format MM/DD/YY hh::mm::ss
inline std::string getCurrentDateAndTime() {
  std::ostringstream oss;
  
  time_t t = time(NULL);
	tm* timePtr = localtime(&t);
  
  oss << timePtr->tm_mon + 1 << "/";
  oss << timePtr->tm_mday << "/";
  oss << ( 1900 + timePtr->tm_year) << " ";
  
  oss << timePtr->tm_hour << ":";
  oss << timePtr->tm_min << ":";
  oss << timePtr->tm_sec;

  return oss.str();
}

// returns the date in format MM/DD/YY hh::mm::ss
inline std::string getCurrentDateAndTime(std::string separator) {
  std::ostringstream oss;
  
  time_t t = time(NULL);
	tm* timePtr = localtime(&t);
  
  oss << std::setfill('0');
  oss << std::setw(4) << (1900 + timePtr->tm_year) << separator;
  oss << std::setw(2) << timePtr->tm_mon << separator;
  oss << std::setw(2) << timePtr->tm_mday << separator;
  
  oss << std::setw(2) << timePtr->tm_hour << separator;
  oss << std::setw(2) << timePtr->tm_min << separator;
  oss << std::setw(2) << timePtr->tm_sec;

  return oss.str();
}

inline std::string GetWorkingDirectory() {
  static char cCurrentPath[FILENAME_MAX];

  if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
    return std::string();
  }

  cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

  return std::string(cCurrentPath);
}

inline std::string GetCurrentUser() {
  #ifdef DM_PLATFORM_OSX
    struct passwd *passwd;
    passwd = getpwuid ( getuid());
    return passwd->pw_name;
  #else
    return "Not implemented";
  #endif
}

// changes the terminal settings, allowing one to get the key code
// of the button that was pressed without waiting for enter to be pressed
/*
  USAGE:
  for (;;) {
        key = getch();
        // terminate loop on ESC (0x1B) or Ctrl-D (0x04) on STDIN 
        if (key == 0x1B || key == 0x04) {
            break;
        }
        else {
            printf("%c\n", key));
        }
    }
*/
inline int getch(void) {
  int c=0;
#ifndef DM_PLATFORM_CYGWIN
  struct termios org_opts, new_opts;
  int res=0;
      //-----  store old settings -----------
  res=tcgetattr(STDIN_FILENO, &org_opts);
  assert(res==0);
      //---- set new terminal parms --------
  memcpy(&new_opts, &org_opts, sizeof(new_opts));
  new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
  c=getchar();
      //------  restore old settings ---------
  res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
  assert(res==0);
#endif
return(c);
}
// throws exception
inline std::string getFileContentAsString(std::string path_) {
  std::ifstream ifs(path_.c_str());
  return std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
}

inline bool fileExists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

inline std::string getFileExtension(const std::string filePath_) {
  std::vector<std::string> tokens = tokenize(filePath_, ".");
  return tokens[tokens.size() - 1];
}

inline void toLower(std::string& str_) {
  std::transform(str_.begin(), str_.end(), str_.begin(), ::tolower);
}

}}

#endif

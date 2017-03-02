/*
 *
 *  Copyright 2016 Bernardo Cuteri, Francesco Ricca.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */
#include "SharedFunctions.h"
#include <string>
#include <iostream>
bool isUnsignedInteger(const std::string & s)
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

std::string escapeDoubleQuotes(const std::string & s) {
    if(s[0]=='"') { 
       return "\\"+s.substr(0, s.size()-1)+"\\\"";
    }
    return s;
}
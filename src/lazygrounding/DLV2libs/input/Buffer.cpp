/*
 *
 *  Copyright 2014 Mario Alviano, Carmine Dodaro, Francesco Ricca and
 *                 Pierfrancesco Veltri.
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

/*
 * Buffer.cpp
 *
 *  Created on: 20/apr/2014
 *      Author: ricca
 */

#include "Buffer.h"
#include <iostream>

using namespace std;
using namespace DLV2;
    
Buffer::Buffer(): 
        lastBlockSize(0), 
        blocks(20)
{
	for (unsigned i = 0; i < 20; ++i)
		blocks[i] = 0;
}

void 
Buffer::addBlock(
    char* b, 
    int size) 
{
    lastBlockSize = size;
	char* bc = new char[size];
	memcpy(bc,b,size*sizeof(char));
	blocks.push_back(bc); 
}
void 
Buffer::lastBlock()
{
  if(blocks.size())
  {
    blocks.back()[lastBlockSize] = '\0';
    blocks.back()[--lastBlockSize] = '\0';
  }
}


void 
Buffer::writeBlock(
    unsigned i, 
    ostream& o)
{
	if(blocks[i])
	{
		o << blocks[i];
		delete [] blocks[i];
		blocks[i] = 0;
	}
}

Buffer::~Buffer() {
	for(unsigned i = 0; i < blocks.size(); ++i)
		if(i)
			delete [] blocks[i];
}

void 
Buffer::flushOn(
    ostream& o )
{
/*if(blocks.size())
{
char* last = blocks.back();
int g=0;
int lastdot= 0;
while(last[g]!='\0')
{
if(last[g] == '.') lastdot=g;
 cerr << g << last[g] << endl; g++;
};

last[lastdot+1]='\0';

}
*/
	for(unsigned i = 0; i < blocks.size(); ++i)
		writeBlock(i,o);
}
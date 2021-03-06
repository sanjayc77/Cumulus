/* 
	Copyright 2010 OpenRTMFP
 
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License received along this program for more
	details (or else see http://www.gnu.org/licenses/).

	This file is a part of Cumulus.
*/

#include "Logs.h"
#include "Poco/File.h"
#include "string.h"

using namespace std;
using namespace Poco;

namespace Cumulus {

Logger* Logs::s_pLogger(NULL);
bool	Logs::s_dump(false);
bool	Logs::s_dumpAll(false);
UInt8	Logs::s_level(Logger::PRIO_INFO); // default log level

Logs::Logs() {
}

Logs::~Logs() {
}

void Logs::EnableDump(bool all) {
	s_dump=true;
	s_dumpAll=all;
}

void Logs::Dump(const UInt8* data,int size,const char* header,bool required) {
	if(!GetLogger() || !s_dump || (!s_dumpAll && !required))
		return;
	char out[PACKETRECV_SIZE*4];
	int len = 0;
	int i = 0;
	int c = 0;
	unsigned char b;
	if(header) {
		out[len++] = '\t';
		c = strlen(header);
		memcpy(out+len,header,c);
		len += c;
		out[len++] = '\n';
	}
	while (i<size) {
		c = 0;
		out[len++] = '\t';
		while ( (c < 16) && (i+c < size) ) {
			b = data[i+c];
			sprintf(out+len,"%X%X ", b/16, b & 0x0f );
			len += 3;
			++c;
		}
		while (c++ < 16) {
			strcpy(out+len,"   ");
			len += 3;
		}
		out[len++] = ' ';
		c = 0;
		while ( (c < 16) && (i+c < size) ) {
			b = data[i+c];
			if (b > 31)
				out[len++] = b;
			else
				out[len++] = '.';
			++c;
		}
		i += 16;
		out[len++] = '\n';
	}
	GetLogger()->dumpHandler(out,len);
}



} // namespace Cumulus

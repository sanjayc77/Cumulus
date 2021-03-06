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

#pragma once

#include "Cumulus.h"
#include "PacketWriter.h"
#include "Poco/BinaryReader.h"

#define PACKETRECV_SIZE		2048

namespace Cumulus {


class PacketReader: public Poco::BinaryReader {
public:
	PacketReader(const Poco::UInt8* buffer,int size);
	PacketReader(PacketReader&);
	virtual ~PacketReader();

	Poco::UInt32 read7BitValue();
	void readRaw(Poco::UInt8* value,int size);
	void readRaw(char* value,int size);
	void readRaw(int size,std::string& value);
	void readString8(std::string& value);
	void readString16(std::string& value);
	Poco::UInt8		read8();
	Poco::UInt16	read16();
	Poco::UInt32	read32();

	int				available();
	Poco::UInt8*	current();
	int				position();

	void			reset(int newPos=0);
	void			shrink(int rest);
	void			next(int size);
private:
	MemoryInputStream _memory;
	
};

inline void PacketReader::readRaw(Poco::UInt8* value,int size) {
	Poco::BinaryReader::readRaw((char*)value,size);
}
inline void PacketReader::readRaw(char* value,int size) {
	Poco::BinaryReader::readRaw(value,size);
}
inline void PacketReader::readRaw(int size,std::string& value) {
	Poco::BinaryReader::readRaw(size,value);
}


inline void PacketReader::readString8(std::string& value) {
	readRaw(read8(),value);
}
inline void PacketReader::readString16(std::string& value) {
	readRaw(read16(),value);
}

inline int PacketReader::available() {
	return _memory.available();
}

inline int PacketReader::position() {
	return _memory.current()-_memory.begin();
}

inline void PacketReader::reset(int newPos) {
	_memory.reset(newPos);
}

inline void PacketReader::next(int size) {
	return _memory.next(size);
}

inline Poco::UInt8* PacketReader::current() {
	return (Poco::UInt8*)_memory.current();
}


} // namespace Cumulus

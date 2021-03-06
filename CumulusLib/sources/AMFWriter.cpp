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

#include "AMFWriter.h"
#include "Logs.h"

using namespace std;
using namespace Poco;
using namespace Poco::Util;

namespace Cumulus {

AMFWriter::AMFWriter(BinaryWriter& writer) : _writer(writer) {

}

AMFWriter::~AMFWriter() {

}

void AMFWriter::writeResponseHeader(const string& key,double callbackHandle) {
	_writer.write8(0x14);
	_writer.write32(0);
	write(key);
	writeNumber(callbackHandle);
	writeNull();
}

void AMFWriter::writeBool(bool value){
	_writer.write8(AMF_BOOLEAN); // marker
	_writer << value;
}

void AMFWriter::write(const string& value) {
	if(value.empty()) {
		_writer.write8(AMF_UNDEFINED);
		return;
	}
	_writer.write8(AMF_STRING); // marker
	_writer.writeString16(value);
}

void AMFWriter::writeNumber(double value){
	_writer.write8(AMF_NUMBER); // marker
	_writer << value;
}



void AMFWriter::writeObject(const AMFObject& amfObject) {
	beginObject();
	AbstractConfiguration::Keys keys;
	amfObject.keys(keys);
	AbstractConfiguration::Keys::const_iterator it;
	for(it=keys.begin();it!=keys.end();++it) {
		string name = *it;
		_writer.writeString16(name);
		int type = amfObject.getInt(name+".type",-1);
		switch(type) {
			case AMF_BOOLEAN:
				writeBool(amfObject.getBool(name));
				break;
			case AMF_STRING:
				write(amfObject.getString(name));
				break;
			case AMF_NUMBER:
				writeNumber(amfObject.getDouble(name));
				break;
			case AMF_UNDEFINED:
				write("");
				break;
			case AMF_NULL:
				writeNull();
				break;
			default:
				ERROR("Unknown AMF '%d' type",type);
		}
	}
	endObject();
}


void AMFWriter::writeObjectProperty(const string& name,double value) {
	_writer.writeString16(name);
	writeNumber(value);
}

void AMFWriter::writeObjectProperty(const string& name,const vector<UInt8>& data) {
	_writer.writeString16(name);
	writeByteArray(data);
}

void AMFWriter::writeObjectProperty(const string& name,const string& value) {
	_writer.writeString16(name);
	write(value);
}

void AMFWriter::writeByteArray(const vector<UInt8>& data) {
	if(data.size()==0) {
		_writer.write8(AMF_UNDEFINED);
		return;
	}
	_writer.write8(AMF_AVMPLUS_OBJECT); // switch in AMF3 format 
	_writer.write8(AMF_LONG_STRING); // bytearray in AMF3 format!
	_writer.write7BitValue((data.size() << 1) | 1);
	_writer.writeRaw(&data[0],data.size());

}

void AMFWriter::endObject() {
	// mark end
	_writer.write16(0); 
	_writer.write8(AMF_END_OBJECT);
}



} // namespace Cumulus

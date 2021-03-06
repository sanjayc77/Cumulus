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
#include "PacketReader.h"
#include "PacketWriter.h"
#include "AESEngine.h"
#include "RTMFP.h"
#include "Flow.h"
#include "FlowNull.h"
#include "Poco/Timestamp.h"
#include "Poco/Net/DatagramSocket.h"

#define SYMETRIC_ENCODING	0x01
#define WITHOUT_ECHO_TIME   0x02

namespace Cumulus {

class Session {
public:

	Session(Poco::UInt32 id,
			Poco::UInt32 farId,
			const Peer& peer,
			const Poco::UInt8* decryptKey,
			const Poco::UInt8* encryptKey,
			Poco::Net::DatagramSocket& socket,
			ServerHandler& serverHandler);

	virtual ~Session();

	virtual void	packetHandler(PacketReader& packet);

	Poco::UInt32		id() const;
	Poco::UInt32		farId() const;
	const Peer& 		peer() const;
	bool				died() const;
	bool				failed() const;
	virtual void		manage();
	void				flush(Poco::UInt8 flags=0);
	PacketWriter&		writeMessage(Poco::UInt8 type,Poco::UInt16 length);
	PacketWriter&		writer();

	void	p2pHandshake(const Poco::Net::SocketAddress& address,const std::string& tag,Session* pSession);
	bool	decode(PacketReader& packet,const Poco::Net::SocketAddress& sender);
	
	void	fail(const std::string& msg);

	bool	_testDecode; // TODO enlever!
protected:
	void			setFailed(const std::string& msg);
	virtual void	fail();
	void			kill();

	ServerHandler&			_serverHandler;

	Poco::UInt32			_farId; // Protected for Middle session
	Poco::Timestamp			_recvTimestamp; // Protected for Middle session
	Poco::UInt16			_timeSent; // Protected for Middle session

private:
	void				keepAlive();

	Flow&				flow(Poco::UInt8 id);
	Flow*				createFlow(const std::string& signature,Poco::UInt8 id);
	
	bool						_failed;
	Poco::UInt8					_timesFailed;
	Poco::UInt8					_timesKeepalive;

	std::map<Poco::UInt8,Flow*> _flows;	
	FlowNull					_flowNull;

	Poco::UInt32				_id;

	Poco::Net::DatagramSocket&	_socket;
	AESEngine					_aesDecrypt;
	AESEngine					_aesEncrypt;

	Poco::UInt8					_buffer[PACKETSEND_SIZE];
	PacketWriter				_writer;

	bool						_died;
	Peer						_peer;

	std::map<std::string,Poco::UInt8>		_p2pHandshakeAttemps;
};

inline void Session::fail(const std::string& msg) {
	setFailed(msg);
	fail();
}

inline bool Session::failed() const {
	return _failed;
}

inline const Peer& Session::peer() const {
	return _peer;
}

inline Poco::UInt32 Session::id() const {
	return _id;
}

inline Poco::UInt32 Session::farId() const {
	return _farId;
}

inline bool Session::died() const {
	return _died;
}

} // namespace Cumulus

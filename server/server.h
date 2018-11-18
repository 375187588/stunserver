/*
   Copyright 2011 John Selbie

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef STUN_SERVER_H
#define	STUN_SERVER_H

#include "stunsocket.h"
#include "stunsocketthread.h"
#include "stunauth.h"
#include "messagehandler.h"

class CStunServerConfig
{
public:
    uint32_t nThreading;  // when set to 0, all sockets on 1 thread. Otherwise, N threads per socket
    
    uint32_t nMaxConnections; // only valid for TCP (on a per-thread basis)

    CSocketAddress addrPP; // address for PP
    CSocketAddress addrPA; // address for PA, ignored if fIsFullMode==false
    CSocketAddress addrAP; // address for AP, ignored if fIsFullMode==false
    CSocketAddress addrAA; // address for AA, ignored if fIsFullMode==false
    
    CSocketAddress addrPrimaryAdvertised;    // public-IP for PP and PA (port is ignored)
    CSocketAddress addrAlternateAdvertised;  // public-IP for AP and AA (port is ignored)
    
    bool fEnableDosProtection; // enable denial of service protection
    bool fReuseAddr; // if true, the socket option SO_REUSEADDR will be set
    bool fIsFullMode; // indicated that we are listening on PA, AP, and AA addresses above
    bool fTCP; // if true, then use TCP instead of UDP
    
    CStunServerConfig();
};


class CStunServer 
{
private:
    std::vector<std::shared_ptr<CStunSocket>> _sockets;
    std::vector<CStunSocketThread*> _threads;

    TransportAddressSet _tsa;
    std::shared_ptr<IStunAuth> _spAuth;

    HRESULT InitializeTSA(const CStunServerConfig& config);
    HRESULT CreateSocket(SocketRole role, const CSocketAddress& addr, bool fReuseAddr);
    HRESULT CreateSockets(const CStunServerConfig& config);

    void PostWakeupMessages();
    
public:
    CStunServer();
    ~CStunServer();

    HRESULT Initialize(const CStunServerConfig& config);
    HRESULT Shutdown();

    HRESULT Start();
    HRESULT Stop();
};

#endif	/* SERVER_H */

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

#ifndef STUNSOCKETTHREAD_H
#define	STUNSOCKETTHREAD_H

#include "stunsocket.h"
#include "ratelimiter.h"


class CStunServer;


class CStunSocketThread
{
    
public:
    CStunSocketThread();
    ~CStunSocketThread();
    
    HRESULT Init(std::vector<std::shared_ptr<CStunSocket>>& arrayOfFourSockets, const TransportAddressSet& tsa, std::shared_ptr<IStunAuth> spAuth, SocketRole rolePrimaryRecv, std::shared_ptr<RateLimiter>& _spRateLimiter);
    HRESULT Start();

    HRESULT SignalForStop();
    HRESULT WaitForStopAndClose();
    
private:
    
    // this is the function that runs in a thread
    void Run();
    
    static void* ThreadFunction(void* pThis);
    
    CStunSocket* WaitForSocketData();
    
    std::vector<std::shared_ptr<CStunSocket>> _arrSendSockets; // 1 socket in basic mode.  4 sockets in full mode
    std::vector<std::shared_ptr<CStunSocket>> _socks; // 1 socket in multi-threaded or basic mode.  4 sockets in single-threaded full mode
    
    bool _fNeedToExit;
    pthread_t _pthread;
    bool _fThreadIsValid;
    
    int _rotation;
    
    TransportAddressSet _tsa;
    
    std::shared_ptr<IStunAuth> _spAuth;
    
    // pre-allocated objects for the thread
    CStunMessageReader _reader;
    CRefCountedBuffer _spBufferReader; // buffer internal to the reader
    CRefCountedBuffer _spBufferIn;     // buffer we receive requests on
    CRefCountedBuffer _spBufferOut;    // buffer we send responses on
    StunMessageIn _msgIn;
    StunMessageOut _msgOut;
    
    std::shared_ptr<RateLimiter> _spLimiter;
    
    HRESULT InitThreadBuffers();
    void UninitThreadBuffers();
    
    HRESULT ProcessRequestAndSendResponse();
    
    void ClearSocketArray();

    void DumpInitParams(std::vector<std::shared_ptr<CStunSocket>>& arrayOfFourSockets, const TransportAddressSet& tsa, SocketRole rolePrimaryRecv);
    
};

#endif	/* STUNSOCKETTHREAD_H */


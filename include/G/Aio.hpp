//
//  Aio.hpp
//  GHTTPd
//
//  Created by 王兴卓 on 16/6/11.
//  Copyright © 2016年 watsserve. All rights reserved.
//

#ifndef Aio_h
#define Aio_h

extern "C" {
    #include <aio.h>
}
#include "G/Object.hpp"


/**
 *  UNIX or BSD
 */
#if !defined (__linux__) && !defined(__linux)

#include "G/ThreadPool.hpp"

struct aioinit
{
    int aio_threads;   // Maximum number of threads
    int aio_num;       // Number of expected simultaneous requests
    int aio_idle_time;
};

#endif


/**
 *  MAC or BSD
 */
#if defined(__APPLE__) || defined (__MACOSX__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__)

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

typedef struct aio_back
{
    ssize_t readyDataLen;
    int error;
} AioBack;

#endif


/**
 *  all system not windows
 */

namespace G {
    class Aio : public Object {
        
    public:

    	// MAC or BSD
#if defined(__APPLE__) || defined (__MACOSX__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__)
        static int kq;
        static struct kevent *eventList;
        static AioBack *abList;
        static aioinit conf;
        static void* listenEvnt(void *);
        static void* eventCallback(void *);
        static ThreadPool threadPool;

        // UNIX
#elif !defined (__linux__) && !defined(__linux)
        static int pfd;

        // linux
#else
        static int epfd;
#endif
        static int aioInit(struct aioinit *);
        static int aioRead(struct aiocb *);
        static int aioWrite(struct aiocb *);
        static ssize_t aioReturn(struct aiocb *);
        static int aioError(const struct aiocb *);
        static int aioCancel(int, struct aiocb *);
    };
}

#endif /* Aio_h */

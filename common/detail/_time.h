/*	$OpenBSD: time.h,v 1.11 2000/10/10 13:36:48 itojun Exp $	*/
/*	$NetBSD: time.h,v 1.18 1996/04/23 10:29:33 mycroft Exp $	*/

/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)time.h	8.2 (Berkeley) 7/10/94
 */

#ifndef _SYS_TIME_H_
#define _SYS_TIME_H_

#ifdef timercmp
#undef timercmp
#endif

#ifdef timeradd
#undef timeradd
#endif

#ifdef timersub
#undef timersub
#endif

#define	timercmp(tvp, uvp, cmp)						\
	(((tvp)->tv_sec == (uvp)->tv_sec) ?				\
	    ((tvp)->tv_usec cmp (uvp)->tv_usec) :			\
	    ((tvp)->tv_sec cmp (uvp)->tv_sec))
#define	timeradd(tvp, uvp, vvp)						\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec + (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec + (uvp)->tv_usec;	\
		if ((vvp)->tv_usec >= 1000000) {			\
			(vvp)->tv_sec++;				\
			(vvp)->tv_usec -= 1000000;			\
		}							\
	} while (0)
#define	timersub(tvp, uvp, vvp)						\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;	\
		if ((vvp)->tv_usec < 0) {				\
			(vvp)->tv_sec--;				\
			(vvp)->tv_usec += 1000000;			\
		}							\
	} while (0)

/* Operations on timevals. */
struct timeval_a {
        long    tv_sec;         /* seconds */
        long    tv_usec;        /* and microseconds */
		bool operator<(const timeval_a& y) const
		{
			return timercmp(this,&y,<);
		}
		bool operator<=(const timeval_a& y) const
		{
			return timercmp(this,&y,<=);
		}
		bool operator==(const timeval_a& y) const
		{
			return timercmp(this,&y,==);
		}
};

#ifdef WIN32
#include <time.h>
#include <sys/timeb.h>
#define localtime_r(a,b) (*b=*(localtime(a)))
int inline gettimeofday_a (struct timeval_a *tp, void *tz)
{
	struct _timeb timebuffer;

	_ftime (&timebuffer);
	tp->tv_sec = timebuffer.time;
	tp->tv_usec = timebuffer.millitm * 1000;
	return 0;
}
#else
#include <sys/time.h>
int inline gettimeofday_a (struct timeval_a *tp, void *tz)
{
    struct timeval tm;
    gettimeofday(&tm,0);
    tp->tv_sec=tm.tv_sec;
    tp->tv_usec=tm.tv_usec;
    return 0;
}
#endif

#endif /* !_SYS_TIME_H_ */


/*
	timestart and timestop
	Sec. 1.7.2

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/

/*[timestart]*/
#include <sys/times.h>
#include <unistd.h>
#include <stdio.h>

#include "t.h"

static struct tms tbuf1;
static clock_t real1;
static long clock_ticks;

void timestart(void)
{
	real1 = times(&tbuf1);
	clock_ticks = sysconf(_SC_CLK_TCK);
	return;
}

void timestop(char *msg)
{
	struct tms tbuf2;
	clock_t real2;

	real2 = times(&tbuf2);
	fprintf(stdout, "%s:\n\t\"Total (user/sys/real)\", %ld, %ld, %ld\n\t\"Child (user/sys)\", %ld, %ld\n", 
		msg,
	  	(long)((tbuf2.tms_utime + tbuf2.tms_cutime) - (tbuf1.tms_utime + tbuf1.tms_cutime)),
	  	(long)((tbuf2.tms_stime + tbuf2.tms_cstime) - (tbuf1.tms_stime + tbuf1.tms_cstime)),
		(long)(real2 - real1),
	  	(long)(tbuf2.tms_cutime - tbuf1.tms_cutime),
	  	(long)(tbuf2.tms_cstime - tbuf1.tms_cstime));
	return;
}
/*[]*/

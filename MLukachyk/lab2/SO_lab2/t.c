//
// Created by Maryna Lukachyk on 2020-03-25.
//

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
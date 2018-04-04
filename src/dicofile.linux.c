#include <stdio.h>
#include <sys/time.h>
#include <fcntl.h>

//#define SHARE_ID 123
#define ACCESS_TIMEOUT 8e06

FILE *dicofopen(char *nom,char *mode)
{
    FILE *flot;
    int desc,flag,fcn_status;

/*
    flag = O_RDWR | O_DSYNC | O_RSYNC | O_SYNC;
	      2     010000     010000    010000
    flag = O_RDWR | O_FSYNC;  // FreeBSD
*/
    flag = O_RDWR | O_DSYNC | O_RSYNC | O_SYNC;
    desc = open(nom,flag);
    if (desc==-1)
        return NULL;
    fcn_status=flock(desc,LOCK_EX | LOCK_NB);
    if (fcn_status == -1)
    {
        double tdiff;
        long tdiffm;
        struct timeval *temps2,*temps1;
        struct timezone *tps2,*tps1;

        temps1 = malloc(sizeof(struct timeval));
        temps2 = malloc(sizeof(struct timeval));
        tps1 = malloc(sizeof(struct timezone));
        tps2 = malloc(sizeof(struct timezone));
        gettimeofday(temps1,tps1);

        do {
            gettimeofday(temps2,tps2);
            tdiff = (double)difftime(temps2->tv_sec,temps1->tv_sec);
            tdiffm = temps2->tv_usec-temps1->tv_usec;
            tdiff = tdiff*1000000 + tdiffm;
            if (tdiff>ACCESS_TIMEOUT)
                return NULL;
            fcn_status=flock(desc,LOCK_EX | LOCK_NB);
        } while (fcn_status == -1);
    }
    flot = fdopen(desc,mode);
    return flot;
}

void dicofclose(FILE *f)
{
//   flock(f->_fileno,LOCK_UN);
    fclose(f);
}




#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>

#include <stdio.h>

#define BUFSIZE 5

int main(int argc, char* argv[])
{
  struct rlimit limit;
  char buf[BUFSIZE] = {'a','a','a','a','a'};
  char* filename;
  int bytes = 0, towrite, smin = 0, smax = 10000, sdef = 100, 
    wrote, fd, s = sdef; 
  switch(argc){
    case 1:
      printf("Filename is required\n");
      return 1;
      break;
    case 2:
      filename = argv[1];
      break;
    case 3:
      s = atoi(argv[1]);
      filename = argv[2];
      break;
    default:
      printf("Too many params\n");
      return 2;
      break;
  }
  if((s < smin) || (s > smax))
     s = sdef;     
  
  /* reading file size limit */
  getrlimit(RLIMIT_FSIZE, &limit);
  printf("RLIMIT_FSIZE: cur=%d, max=%d\n", limit.rlim_cur, limit.rlim_max);
  fflush(stdout);

  printf("Writing %d bytes into %s file...\n", s, filename);
  fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC,0666);
  do{
    towrite = s - bytes;
    towrite = towrite<BUFSIZE?towrite:BUFSIZE;
    wrote = write(fd, buf, towrite);
    bytes += wrote;
    if(wrote == -1)
      break;
  }
  while((wrote > 0) && (bytes < s));
  close(fd);

  return 0;
}
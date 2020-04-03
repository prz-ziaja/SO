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

int isNumeric(char* str)
{
  char* character;
  for(character = str; *character != '\0'; character++)
  {
    if(*character < '0' || *character > '9')
    {
      return -1;
    }
  }
  int multiplier = 1;
  int value = 0;
  character--;
  for(; character != str - 1; character--)
  {
    value = value + (*character - 48) * multiplier;
    multiplier = multiplier * 10;
  }
  return value;
}

int main(int argc, char* argv[])
{
  struct rlimit limit;
  char buf[BUFSIZE] = {'a','a','a','a','a'};
  char* filename;
  int bytes = 0, towrite, smin = 0, smax = 10000, sdef = 100, 
    wrote, fd, s = sdef; 

  if(argc == 2)
  {
    filename = argv[1];
  }
  else if(argc == 3)
  {
    filename = argv[2];
    s = isNumeric(argv[1]);
    if(s == -1)
    {
      printf("Usage: ./save <bytes> [filename]");
    }
  }
  else
  {
    return 1;
  }
  

  if((s < smin) || (s > smax))
     s = sdef;     
  
  /* reading file size limit */
  getrlimit(RLIMIT_FSIZE, &limit);
  printf("RLIMIT_FSIZE: cur=%d, max=%d\n", limit.rlim_cur, limit.rlim_max);
  fflush(stdout);

  printf("Writing %d bytes into %s file...\n", s, filename);
  fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC,
      S_IRWXU | S_IRWXG | S_IRWXO);
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
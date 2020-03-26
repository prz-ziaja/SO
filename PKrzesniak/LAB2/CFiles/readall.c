ssize_t readall(int fd, void *buf, size_t nbyte)
{
 ssize_t nread = 0, n;
 do {
	 if ((n = read(fd, &((char *)buf)[nread], nbyte - nread)) == -1) {
		 if (errno == EINTR)
				continue;
		 else
				exit(EXIT_FAILURE);
	 }
	 if (n == 0)
		 return nread;  
		 nread += n;
	 } while (nread < nbyte);
 
 return nread;
}

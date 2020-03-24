ssize_t readall(int fd, const void *buf, size_t nbyte)
{
	ssize_t nread = 0, n;

	do
	{
		if ((n = read(fd, &((const char *)buf)[nread], nbyte - nread)) == -1) {
    			if (errno == EINTR)
    				continue;
    			else
    				return -1;
    		}
    		nwritten += n;

		if (n == 0)/* koniec pliku */
			return nwritten;		
	}
	while(nread < nbyte);

	return nread;
}
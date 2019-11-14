/*************************************
 * Lab 5 Ex3
 * Name: Jiang Yuxin
 * Student No: A0177868A
 * Lab Group: 11
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"

size_t my_fwrite(const void *ptr, size_t size, size_t nmemb, MY_FILE *stream) {
	size_t wrt = size * nmemb;
	size_t wrtcur = 0;

	if(stream->sz + wrt <= BUFFER_SIZE) {
		strncpy(stream->buffer+stream->sz, ptr, wrt);
		stream->sz+=wrt;
		wrtcur = wrt;
		wrt=0;
	} else {
		strncpy(stream->buffer+stream->sz, ptr, BUFFER_SIZE - stream->sz);
		wrtcur = BUFFER_SIZE - stream->sz;
		wrt -= wrtcur;
		while(wrt>0) {
			if(stream->apd)
				lseek(stream->fd, 0, SEEK_END);
			ssize_t wrtsz = write(stream->fd, stream->buffer, BUFFER_SIZE);
			if(wrtsz==-1) return -1;
			
			stream->sz=0;
			size_t nxtwrt = MIN(BUFFER_SIZE, wrt);
			strncpy(stream->buffer, ptr, nxtwrt);
			wrt-=nxtwrt;
			wrtcur+=nxtwrt;
			stream->sz=nxtwrt;
		}
	}
	stream->mode=1;
	return wrtcur/size;
}

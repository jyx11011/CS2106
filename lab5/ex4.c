/*************************************
 * Lab 5 Ex4
 * Name: Jiang Yuxin
 * Student No: A0177868A
 * Lab Group: 11
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"

int my_fflush(MY_FILE *stream) {
	if(stream->mode==1){
		if(stream->apd)
				lseek(stream->fd, 0, SEEK_END);
		ssize_t wrtsz = write(stream->fd, stream->buffer, stream->sz);
		if(wrtsz==-1) 
			return MY_EOF;
		else {
			stream->mode=0;
			stream->sz=0;
			return 0;
		}
	} else if(stream->mode==-1){
		lseek(stream->fd, -stream->sz, SEEK_CUR);
		stream->sz=0;
		stream->mode=0;
		return 0;
	} else {
		return 0;
	}
}

int my_fseek(MY_FILE *stream, long offset, int whence) {
	my_fflush(stream);
	int offsetFrombegin = lseek(stream->fd, offset, whence);
	return offsetFrombegin;
}
/*************************************
 * Lab 5 Ex2
 * Name: Jiang Yuxin
 * Student No: A0177868A
 * Lab Group: 11
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"

size_t my_fread(void *ptr, size_t size, size_t nmemb, MY_FILE *stream) {
	size_t rd = size*nmemb;
	size_t rdcur = 0;
	if(stream->sz>=rd) {
		strncpy((char*)ptr, stream->buffer+stream->head, rd);
		stream->head=(stream->head+rd)%BUFFER_SIZE;
		stream->sz-=rd;
		rdcur = rd;
	} else {
		strncpy((char*)ptr, stream->buffer+stream->head, stream->sz);
		stream->head=0;
		rd-=stream->sz;
		rdcur = stream->sz;
		stream->sz=0;

		while(rd>0) {
			ssize_t size_read = read(stream->fd, stream->buffer, BUFFER_SIZE);
			if(size_read==-1) return -1;
			if(size_read==0) break;
			stream->sz=size_read;
			stream->head=0;
			size_t nxt_read = MIN(rd, stream->sz);
			strncpy((char*)(ptr+rdcur), stream->buffer+stream->head, nxt_read);

			rdcur+=nxt_read;
			stream->head=(stream->head+nxt_read)%BUFFER_SIZE;
			rd-=nxt_read;
			stream->sz-=nxt_read;
		}

	}
	stream->mode = -1;
	*((char*)(ptr+rdcur))='\0';
	if(size==0) return 0;
	else return rdcur/size;
}

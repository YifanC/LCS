#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "zlib.h"

#define CHECK_ERR(err, msg) { \
    if (err != Z_OK) { \
        fprintf(stderr, "%s error: %d\n", msg, err); \
        exit(1); \
    } \
}

static z_stream c_stream; /* compression stream */
static z_stream d_stream; /* decompression stream */

extern int strcmp(const char *s1, const char *s2);

int gzip_init(void) {

  int err;
  uLong zf;

  static const char* myVersion = ZLIB_VERSION;
  
  if (zlibVersion()[0] != myVersion[0]) {
    fprintf(stderr, "incompatible zlib version\n");
    exit(1);
    
  } else if (strcmp(zlibVersion(), ZLIB_VERSION) != 0) {
    fprintf(stderr, "warning: different zlib version\n");
  }

  zf = zlibCompileFlags();
  printf("zlib version %s = 0x%04x, compile flags = 0x%lx\n",
	 ZLIB_VERSION, ZLIB_VERNUM, zf);

  c_stream.zalloc = (alloc_func)0;
  c_stream.zfree = (free_func)0;
  c_stream.opaque = (voidpf)0;
  
  err = deflateInit(&c_stream, Z_BEST_SPEED);
  CHECK_ERR(err, "deflateInit");

  return err;
}

long gzip_data(int nb, char* p, int no, char* out) {

  int err, nc;

  c_stream.next_out = (Byte*) out;
  c_stream.avail_out = (uInt) no;
  
  c_stream.next_in = (Byte*) p;
  c_stream.avail_in = (uInt) nb;

  err = deflate(&c_stream, Z_FINISH);
  if (err != Z_STREAM_END) {
    fprintf(stderr, "deflate should report Z_STREAM_END\n");
    exit(1);
  }

  nc = c_stream.total_out;
  
  if (c_stream.avail_in != 0) {
    fprintf(stderr, "deflate not greedy\n");
    exit(1);
  }
  err = deflateReset(&c_stream);
  CHECK_ERR(err, "deflate");

  return nc;
}

int gzip_end(void) {

  int err;

  err = deflate(&c_stream, Z_FINISH);
  if (err != Z_STREAM_END) {
    fprintf(stderr, "deflate should report Z_STREAM_END\n");
    exit(1);
  }
  err = deflateEnd(&c_stream);
  CHECK_ERR(err, "deflateEnd");
  
  return err;
}

int gunzip_init(void) {

  int err;
  uLong zf;

  static const char* myVersion = ZLIB_VERSION;
  
  if (zlibVersion()[0] != myVersion[0]) {
    fprintf(stderr, "incompatible zlib version\n");
    exit(1);
    
  } else if (strcmp(zlibVersion(), ZLIB_VERSION) != 0) {
    fprintf(stderr, "warning: different zlib version\n");
  }

  zf = zlibCompileFlags();
  printf("zlib version %s = 0x%04x, compile flags = 0x%lx\n",
	 ZLIB_VERSION, ZLIB_VERNUM, zf);

  d_stream.zalloc = (alloc_func)0;
  d_stream.zfree = (free_func)0;
  d_stream.opaque = (voidpf)0;
  
  err = inflateInit(&d_stream);
  CHECK_ERR(err, "inflateInit");

  return err;
}

long gunzip_data(int nb, char* p, int no, char* out) {

  int err, nout;

  d_stream.next_in  = (Byte*) p;
  d_stream.avail_in = (uInt) nb;

  d_stream.next_out = (Byte*) out;
  d_stream.avail_out = (uInt) no;

  err = inflate(&d_stream, Z_SYNC_FLUSH);
  if (err == Z_STREAM_END) {
    printf("stream end found\n"); //  break;}
  } else {
    CHECK_ERR(err, "large inflate");
  }

  nout =  d_stream.total_out;

  err = inflateReset(&d_stream);
  CHECK_ERR(err, "inflate");

  return nout;

}

int gunzip_end(void) {

  int err;

  err = inflateEnd(&d_stream);
  CHECK_ERR(err, "inflateEnd");

  return err;
}


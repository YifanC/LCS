
int  gzip_init(void);
long gzip_data(int nr, char* buffer, int no, char* out);
int  gzip_end(void);
int  gunzip_init(void);
long gunzip_data(int nr, char* buffer, int no, char* out);
int  gunzip_end(void);


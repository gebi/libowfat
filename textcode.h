#ifndef TEXTCODE_H
#define TEXTCODE_H

/* These take len bytes from src and write them in encoded form to
 * dest (if dest != NULL), returning the number of bytes written. */
unsigned int fmt_uuencoded(char* dest,const char* src,unsigned int len);
unsigned int fmt_base64(char* dest,const char* src,unsigned int len);
unsigned int fmt_quotedprintable(char* dest,const char* src,unsigned int len);
unsigned int fmt_urlencoded(char* dest,const char* src,unsigned int len);
unsigned int fmt_yenc(char* dest,const char* src,unsigned int len);

/* These read one line from src, decoded it, and write the result to
 * dest.  The number of decoded bytes is written to destlen.  dest
 * should be able to hold destlen bytes as a rule of thumb. */
unsigned int scan_uuencoded(const char *src,char *dest,unsigned int *destlen);
unsigned int scan_base64(const char *src,char *dest,unsigned int *destlen);
unsigned int scan_quotedprintable(const char *src,char *dest,unsigned int *destlen);
unsigned int scan_urlencoded(const char *src,char *dest,unsigned int *destlen);
unsigned int scan_yenc(const char *src,char *dest,unsigned int *destlen);

extern const char base64[64];

#endif

#ifndef TEXTCODE_H
#define TEXTCODE_H

/* These take len bytes from src and write them in encoded form to
 * dest (if dest != NULL), returning the number of bytes written. */
unsigned int fmt_uuencoded(char* dest,const char* src,unsigned int len);
unsigned int fmt_base64(char* dest,const char* src,unsigned int len);
unsigned int fmt_quotedprintable(char* dest,const char* src,unsigned int len);
unsigned int fmt_urlencoded(char* dest,const char* src,unsigned int len);
unsigned int fmt_yenc(char* dest,const char* src,unsigned int len);
unsigned int fmt_hexdump(char* dest,const char* src,unsigned int len);
/* this changes '<' to '&lt;' and '&' to '&amp;' */
unsigned int fmt_html(char* dest,const char* src,unsigned int len);

/* These read one line from src, decoded it, and write the result to
 * dest.  The number of decoded bytes is written to destlen.  dest
 * should be able to hold destlen bytes as a rule of thumb. */
unsigned int scan_uuencoded(const char *src,char *dest,unsigned int *destlen);
unsigned int scan_base64(const char *src,char *dest,unsigned int *destlen);
unsigned int scan_quotedprintable(const char *src,char *dest,unsigned int *destlen);
unsigned int scan_urlencoded(const char *src,char *dest,unsigned int *destlen);
unsigned int scan_yenc(const char *src,char *dest,unsigned int *destlen);
unsigned int scan_hexdump(const char *src,char *dest,unsigned int *destlen);
unsigned int scan_html(const char *src,char *dest,unsigned int *destlen);

#ifdef STRALLOC_H
/* These take len bytes from src and write them in encoded form to sa.
 * As the stralloc_* functions do, 1
 * is returned. If they run out of memory, sa contains the bytes already 
 * written and 0 is returned.  */
int fmt_quotedprintable_sa(stralloc *sa,const char* src,unsigned int len);
int fmt_base64_sa(stralloc *sa,const char* src,unsigned int len);
int fmt_uuencoded_sa(stralloc *sa,const char* src,unsigned int len);
int fmt_urlencoded_sa(stralloc *sa,const char* src,unsigned int len);
int fmt_yenc_sa(stralloc *sa,const char* src,unsigned int len);
int fmt_hexdump_sa(stralloc *sa,const char* src,unsigned int len);

/* These read one line from src, decoded it, and write the result to
 * sa. As the stralloc_* functions do, 1
 * is returned. If they run out of memory, sa contains the bytes already 
 * written and 0 is returned.  */
int scan_base64_sa(const char *src,stralloc* sa);
int scan_quotedprintable_sa(const char *src,stralloc* sa);
int scan_uuencoded_sa(const char *src,stralloc *sa);
int scan_urlencoded_sa(const char *src,stralloc *sa);
int scan_yenc_sa(const char *src,stralloc *sa);
int scan_hexdump_sa(const char *src,stralloc *sa);
#endif

extern const char base64[64];

#endif

#ifndef TEXTCODE_H
#define TEXTCODE_H

unsigned int fmt_uuencoded(char* dest,const char* src,unsigned int len);
unsigned int fmt_base64(char* dest,const char* src,unsigned int len);
unsigned int fmt_quotedprintable(char* dest,const char* src,unsigned int len);
unsigned int fmt_yenc(char* dest,const char* src,unsigned int len);

unsigned int scan_uuencoded(const char *src,char *dest,unsigned int *destlen);
unsigned int scan_base64(const char *src,char *dest,unsigned int *destlen);
unsigned int scan_quotedprintable(const char *src,char *dest,unsigned int *destlen);
unsigned int scan_yenc(const char *src,char *dest,unsigned int *destlen);

#endif

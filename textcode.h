#ifndef TEXTCODE_H
#define TEXTCODE_H

/* These take len bytes from src and write them in encoded form to
 * dest (if dest != NULL), returning the number of bytes written. */
unsigned long fmt_uuencoded(char* dest,const char* src,unsigned long len);
unsigned long fmt_base64(char* dest,const char* src,unsigned long len);
unsigned long fmt_quotedprintable(char* dest,const char* src,unsigned long len);
unsigned long fmt_quotedprintable2(char* dest,const char* src,unsigned long len,const char* escapeme);
unsigned long fmt_urlencoded(char* dest,const char* src,unsigned long len);
unsigned long fmt_urlencoded2(char* dest,const char* src,unsigned long len,const char* escapeme);
unsigned long fmt_yenc(char* dest,const char* src,unsigned long len);
unsigned long fmt_hexdump(char* dest,const char* src,unsigned long len);
/* this changes '<' to '&lt;' and '&' to '&amp;' */
unsigned long fmt_html(char* dest,const char* src,unsigned long len);
/* change '\' to "\\", '\n' to "\n", ^A to "\x01" etc */
unsigned long fmt_cescape(char* dest,const char* src,unsigned long len);
unsigned long fmt_cescape2(char* dest,const char* src,unsigned long len,const char* escapeme);
/* fold awk whitespace to '_'; this is great for writing fields with
 * white spaces to a log file and still allow awk to do log analysis */
unsigned long fmt_foldwhitespace(char* dest,const char* src,unsigned long len);

/* These read one line from src, decoded it, and write the result to
 * dest.  The number of decoded bytes is written to destlen.  dest
 * should be able to hold strlen(src) bytes as a rule of thumb. */
unsigned long scan_uuencoded(const char *src,char *dest,unsigned long *destlen);
unsigned long scan_base64(const char *src,char *dest,unsigned long *destlen);
unsigned long scan_quotedprintable(const char *src,char *dest,unsigned long *destlen);
unsigned long scan_urlencoded(const char *src,char *dest,unsigned long *destlen);
unsigned long scan_urlencoded2(const char *src,char *dest,unsigned long *destlen);
unsigned long scan_yenc(const char *src,char *dest,unsigned long *destlen);
unsigned long scan_hexdump(const char *src,char *dest,unsigned long *destlen);
unsigned long scan_html(const char *src,char *dest,unsigned long *destlen);
unsigned long scan_cescape(const char *src,char *dest,unsigned long *destlen);

#ifdef STRALLOC_H
/* WARNING: these functions _append_ to the stralloc, not overwrite! */
/* stralloc wrappers; return 1 on success, 0 on failure */
/* arg 1 is one of the fmt_* functions from above */
int fmt_to_sa(unsigned long (*func)(char*,const char*,unsigned long),
	      stralloc* sa,const char* src,unsigned long len);

int fmt_to_sa2(unsigned long (*func)(char*,const char*,unsigned long,const char*),
	      stralloc* sa,const char* src,unsigned long len,const char* escapeme);

/* arg 1 is one of the scan_* functions from above */
/* return number of bytes scanned */
unsigned long scan_to_sa(unsigned long (*func)(const char*,char*,unsigned long*),
			 const char* src,stralloc* sa);

#define fmt_uuencoded_sa(sa,src,len) fmt_to_sa(fmt_uuencoded,sa,src,len)
#define fmt_base64_sa(sa,src,len) fmt_to_sa(fmt_base64,sa,src,len)
#define fmt_quotedprintable_sa(sa,src,len) fmt_to_sa(fmt_quotedprintable,sa,src,len)
#define fmt_urlencoded_sa(sa,src,len) fmt_to_sa(fmt_urlencoded,sa,src,len)
#define fmt_yenc_sa(sa,src,len) fmt_to_sa(fmt_yenc,sa,src,len)
#define fmt_hexdump_sa(sa,src,len) fmt_to_sa(fmt_hexdump,sa,src,len)
#define fmt_html_sa(sa,src,len) fmt_to_sa(fmt_html,sa,src,len)
#define fmt_cescape_sa(sa,src,len) fmt_to_sa(fmt_cescape,sa,src,len)

#define fmt_quotedprintable2_sa(sa,src,len,escapeme) fmt_to_sa2(fmt_quotedprintable2,sa,src,len,escapeme)
#define fmt_urlencoded2_sa(sa,src,len,escapeme) fmt_to_sa2(fmt_urlencoded2,sa,src,len,escapeme)
#define fmt_cescape2_sa(sa,src,len,escapeme) fmt_to_sa2(fmt_cescape2,sa,src,len,escapeme)

#define scan_uuencoded_sa(src,sa) scan_to_sa(scan_uuencoded,src,sa)
#define scan_base64_sa(src,sa) scan_to_sa(scan_base64,src,sa)
#define scan_quotedprintable_sa(src,sa) scan_to_sa(scan_quotedprintable,src,sa)
#define scan_urlencoded_sa(src,sa) scan_to_sa(scan_urlencoded,src,sa)
#define scan_yenc_sa(src,sa) scan_to_sa(scan_yenc,src,sa)
#define scan_hexdump_sa(src,sa) scan_to_sa(scan_hexdump,src,sa)
#define scan_html_sa(src,sa) scan_to_sa(scan_html,src,sa)
#define scan_cescape_sa(src,sa) scan_to_sa(scan_cescape,src,sa)
#endif

#ifdef ARRAY_H
void fmt_to_array(unsigned long (*func)(char*,const char*,unsigned long),
		  array* a,const char* src,unsigned long len);

void fmt_tofrom_array(unsigned long (*func)(char*,const char*,unsigned long),
		      array* dest,array* src);

void fmt_to_array2(unsigned long (*func)(char*,const char*,unsigned long,const char*),
		  array* a,const char* src,unsigned long len,const char* escapeme);

void fmt_tofrom_array2(unsigned long (*func)(char*,const char*,unsigned long,const char*),
		      array* dest,array* src,const char* escapeme);

unsigned long scan_to_array(unsigned long (*func)(const char*,char*,unsigned long*),
			    const char* src,array* dest);

unsigned long scan_tofrom_array(unsigned long (*func)(const char*,char*,unsigned long*),
			        array* src,array* dest);
#endif

extern const char base64[64];

#endif

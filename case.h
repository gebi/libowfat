#ifndef CASE_H
#define CASE_H

/* turn upper case letters to lower case letters, ASCIIZ */
extern void case_lowers(char *s);
/* turn upper case letters to lower case letters, binary */
extern void case_lowerb(void *buf,unsigned long len);

/* like str_diff, ignoring case */
extern int case_diffs(const char *,const char *);
/* like byte_diff, ignoring case */
extern int case_diffb(const void *,unsigned long,const void *);

/* like str_start, ignoring case */
extern int case_starts(const char *,const char *);

#define case_equals(s,t) (!case_diffs((s),(t)))
#define case_equalb(s,n,t) (!case_diffb((s),(n),(t)))

#endif

#ifndef CASE_H
#define CASE_H

/* turn upper case letters to lower case letters, ASCIIZ */
extern void case_lowers(char *s);
/* turn upper case letters to lower case letters, binary */
extern void case_lowerb(char *buf,unsigned int len);

/* like str_diff, ignoring case */
extern int case_diffs(const char *,const char *);
/* like byte_diff, ignoring case */
extern int case_diffb(const char *,unsigned int,const char *);

/* like str_start, ignoring case */
extern int case_starts(const char *,const char *);
/* alias for case_diffb? */
extern int case_startb(const char *,unsigned int,const char *);

#define case_equals(s,t) (!case_diffs((s),(t)))

#endif

#include "fmt.h"
#include "textcode.h"
#include "scan.h"

size_t scan_jsonescape(const char *src,char *dest,size_t *destlen) {
  register const unsigned char* s=(const unsigned char*) src;
  size_t written=0,i;
  char c;
  unsigned int prev,cur,todo;
  prev=cur=(unsigned int)-1;
  for (i=0; s[i]; ++i) {
    if (s[i]=='"') {
      if (prev!=(unsigned int)-1) goto abort;
      goto done;
    }
    if ((c=s[i])=='\\') {
      switch (s[i+1]) {
      case '\\':
	// c='\\';	// c already is backslash
	break;
      case '"': c='"'; break;
      case 'n': c='\n'; break;
      case 'r': c='\r'; break;
      case 'b': c='\b'; break;
      case 'f': c='\f'; break;
      case 't': c='\t'; break;
      case 'u':
	{
	  size_t j;
	  for (cur=j=0; j<4; ++j) {
	    char x=scan_fromhex(s[i+2+j]);
	    if (x<0) goto abort;	// not hex -> invalid input
	    cur=(cur<<4) | x;
	  }
	  if (cur>=0xd800 && cur<=0xdbff) {
	    // utf-16 surrogate pair; needs to be followed by another
	    // surrogate. We need to read both and convert to UTF-8
	    if (prev!=(unsigned int)-1) goto abort;	// two lead surrogates
	    prev=cur;
	    i+=5;	// we want i to go up by 6, 1 is done by the for loop
	    continue;	// write nothing!
	  } else if (cur>=0xdc00 && cur<=0xdfff) {
	    if (prev==(unsigned int)-1) goto abort;	// no lead surrogate
	    todo=(cur&0x3ff) | ((prev&0x3ff) << 10) | 0x10000;
	  } else
	    todo=cur;
	  written+=fmt_utf8(dest?dest+written:dest,todo);
	  i+=5;
	  prev=-1;
	  continue;
	}
      default:
	c=s[i+1];
	break;
      }
      ++i;
    }
    if (prev!=(unsigned int)-1) goto abort;
    /* We expect utf-8 incoming. Make sure it's valid. */
    if (!scan_utf8(s+i,4,NULL)) goto abort;
    if (dest) dest[written]=c;
    ++written;
  }
done:
  *destlen=written;
  return i;
abort:
  if (prev!=(unsigned int)-1) i-=6; // if we abort and there still was an open surrogate pair, cancel it
  *destlen=written;
  return i;
}

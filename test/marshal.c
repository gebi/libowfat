#include <fmt.h>
#include <scan.h>
#include <textcode.h>
#include <byte.h>
#include <assert.h>

char buf[100];

void zap() { size_t i; for (i=0; i<sizeof(buf); ++i) buf[i]='_'; }

int main() {
  uint32_t x;

  // check utf8 encoding
  zap(); assert(fmt_utf8(NULL,12345) == 3);
  zap(); assert(fmt_utf8(buf,12345) == 3 && byte_equal(buf,4,"\xe3\x80\xb9_"));
  zap(); assert(fmt_utf8(buf,0xa9) == 2 && byte_equal(buf,3,"\xc2\xa9_"));
  zap(); assert(fmt_utf8(buf,0)==1 && byte_equal(buf,2,"\x00_"));
  zap(); assert(fmt_utf8(buf,0x80000000)==0 && buf[0]=='_');

  // check utf8 decoding
  assert(scan_utf8("",1,&x)==1 && x==0);	// one byte
  assert(scan_utf8("\xf0",1,&x)==0);		// incomplete sequence
  assert(scan_utf8("\xc2\xa9",2,&x)==2 && x==0xa9);
  assert(scan_utf8("\xe3\x80\xb9",3,&x)==3 && x==12345);
  assert(scan_utf8("\xc0\xaf",2,&x)==0);	// '/' in non-minimal encoding
  assert(scan_utf8("\xc2\x00",2,&x)==0);	// second byte does not start with 10 bits

  // check ascii encoding of numbers
  assert(fmt_long(NULL,12345)==5);
  zap(); assert(fmt_long(buf,12345)==5 && byte_equal(buf,6,"12345_"));
  assert(fmt_long(NULL,-12345)==6);
  zap(); assert(fmt_long(buf,-12345)==6 && byte_equal(buf,7,"-12345_"));
  assert(fmt_ulong(NULL,123456)==6);
  zap(); assert(fmt_ulong(buf,123456)==6 && byte_equal(buf,7,"123456_"));
  assert(fmt_xlong(NULL,0x12345)==5);
  zap(); assert(fmt_xlong(buf,0x12345)==5 && byte_equal(buf,6,"12345_"));
  assert(fmt_8long(NULL,012345)==5);
  zap(); assert(fmt_8long(buf,012345)==5 && byte_equal(buf,6,"12345_"));
  assert(fmt_longlong(NULL,12345678900ll)==11);
  zap(); assert(fmt_longlong(buf,12345678900ll)==11 && byte_equal(buf,12,"12345678900_"));
  assert(fmt_longlong(NULL,-12345678900ll)==12);
  zap(); assert(fmt_longlong(buf,-12345678900ll)==12 && byte_equal(buf,13,"-12345678900_"));
  assert(fmt_xlonglong(NULL,12345678900ll)==9);
  zap(); assert(fmt_xlonglong(buf,12345678900ll)==9 && byte_equal(buf,10,"2dfdc1c34_"));
  assert(fmt_ulonglong(NULL,12345678900ull)==11);
  zap(); assert(fmt_ulonglong(buf,12345678900ull)==11 && byte_equal(buf,12,"12345678900_"));

  assert(fmt_ulong0(NULL,12345,7)==7);
  zap(); assert(fmt_ulong0(buf,12345,7)==7 && byte_equal(buf,8,"0012345_"));
  assert(fmt_plusminus(NULL,15)==1);
  zap(); assert(fmt_plusminus(buf,15)==1 && byte_equal(buf,2,"+_"));
  assert(fmt_plusminus(NULL,-15)==1);
  zap(); assert(fmt_plusminus(buf,-15)==1 && byte_equal(buf,2,"-_"));
  assert(fmt_plusminus(NULL,0)==0);
  zap(); assert(fmt_plusminus(buf,0)==0 && buf[0]=='_');
  assert(fmt_minus(NULL,15)==0);
  zap(); assert(fmt_minus(buf,15)==0 && buf[0]=='_');
  assert(fmt_minus(NULL,-15)==1);
  zap(); assert(fmt_minus(buf,-15)==1 && byte_equal(buf,2,"-_"));
  assert(fmt_minus(NULL,0)==0);
  zap(); assert(fmt_minus(buf,0)==0 && buf[0]=='_');

  assert(fmt_str(NULL,"blah")==4);
  zap(); assert(fmt_str(buf,"blah")==4 && byte_equal(buf,5,"blah_"));
  zap(); assert(fmt_strn(buf,"blah",3)==3 && byte_equal(buf,4,"bla_"));
  zap(); assert(fmt_strn(buf,"blah",5)==4 && byte_equal(buf,5,"blah_"));

  assert(fmt_pad(NULL,"blah",3,5,7)==5);
  zap(); assert(fmt_pad(buf,"blah",3,5,7)==5 && byte_equal(buf,6,"  bla_"));
  zap(); assert(fmt_pad(buf,"blah",3,5,5)==5 && byte_equal(buf,6,"  bla_"));
  zap(); assert(fmt_pad(buf,"blah",4,5,5)==5 && byte_equal(buf,6," blah_"));
  zap(); assert(fmt_pad(buf,"blah",4,3,5)==4 && byte_equal(buf,5,"blah_"));
  zap(); assert(fmt_pad(buf,"blah",4,3,3)==3 && byte_equal(buf,4,"bla_"));

  assert(fmt_fill(NULL,3,5,7)==5);
  zap(); fmt_str(buf,"bla"); assert(fmt_fill(buf,3,5,7)==5 && byte_equal(buf,6,"bla  _"));
  zap(); fmt_str(buf,"bla"); assert(fmt_fill(buf,3,5,5)==5 && byte_equal(buf,6,"bla  _"));
  zap(); fmt_str(buf,"blah"); assert(fmt_fill(buf,4,5,5)==5 && byte_equal(buf,6,"blah _"));
  zap(); fmt_str(buf,"blah"); assert(fmt_fill(buf,4,3,5)==4 && byte_equal(buf,5,"blah_"));
  zap(); fmt_strn(buf,"blah",3); assert(fmt_fill(buf,4,3,3)==3 && byte_equal(buf,4,"bla_"));

  assert(fmt_human(NULL,12345)==5);
  zap(); assert(fmt_human(buf,12345)==5 && byte_equal(buf,6,"12.3k_"));
  zap(); assert(fmt_human(buf,1024)==4 && byte_equal(buf,5,"1.0k_"));
  zap(); assert(fmt_human(buf,1000)==4 && byte_equal(buf,5,"1.0k_"));
  assert(fmt_humank(NULL,12345)==5);
  zap(); assert(fmt_humank(buf,12345)==5 && byte_equal(buf,6,"12.1k_"));
  zap(); assert(fmt_humank(buf,1024)==4 && byte_equal(buf,5,"1.0k_"));
  zap(); assert(fmt_humank(buf,1000)==4 && byte_equal(buf,5,"1.0k_"));

  zap(); assert(fmt_httpdate(buf,1393353927)==29 && byte_equal(buf,30,"Tue, 25 Feb 2014 18:45:27 GMT_"));

  assert(fmt_asn1derlength(NULL,0)==1);
  zap(); assert(fmt_asn1derlength(buf,0)==1 && byte_equal(buf,2,"\x00_"));
  assert(fmt_asn1derlength(NULL,0xc2)==2);
  zap(); assert(fmt_asn1derlength(buf,0xc2)==2 && byte_equal(buf,3,"\x81\xc2_"));
  assert(fmt_asn1derlength(NULL,0x1234)==3);
  zap(); assert(fmt_asn1derlength(buf,0x1234)==3 && byte_equal(buf,3,"\x82\x12\x34_"));

  assert(fmt_asn1dertag(NULL,0)==1);
  zap(); assert(fmt_asn1dertag(buf,0)==1 && byte_equal(buf,2,"\x00_"));
  assert(fmt_asn1dertag(NULL,0xc2)==2);
  zap(); assert(fmt_asn1dertag(buf,0xc2)==2 && byte_equal(buf,3,"\x81\x42_"));

  zap(); assert(fmt_strm(buf,"hell","o, worl","d!\n")==14 && byte_equal(buf,15,"hello, world!\n_"));
}

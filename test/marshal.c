#include <fmt.h>
#include <scan.h>
#include <textcode.h>
#include <byte.h>
#include <assert.h>

char buf[100];

void zap() { size_t i; for (i=0; i<sizeof(buf); ++i) buf[i]='_'; }

int main() {
  uint32_t x;

  unsigned long long ull;
  unsigned long ul;
  unsigned int ui;
  unsigned short us;
  unsigned char uc;
  signed long long ll;
  signed long l;
  signed int i;
  signed short s;
  signed char c;

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

  assert(fmt_escapecharxml(NULL,0xc2)==6);
  zap(); assert(fmt_escapecharxml(buf,0xc2)==6 && byte_equal(buf,7,"&#194;_"));
  assert(fmt_escapecharxml(NULL,0)==4);
  zap(); assert(fmt_escapecharxml(buf,0)==4 && byte_equal(buf,5,"&#0;_"));

  assert(fmt_escapecharjson(NULL,'\\')==2);	// "\\"
  zap(); assert(fmt_escapecharjson(buf,'\\')==2 && byte_equal(buf,3,"\\\\_"));
  assert(fmt_escapecharjson(NULL,0xc2)==6);	// "\u00c2"
  zap(); assert(fmt_escapecharjson(buf,0xc2)==6 && byte_equal(buf,7,"\\u00c2_"));
  assert(fmt_escapecharjson(NULL,0x1d11e)==12);	// "\ud834\xdd1e"
  zap(); assert(fmt_escapecharjson(buf,0x1d11e)==12 && byte_equal(buf,13,"\\ud834\\udd1e_"));

  assert(fmt_escapecharquotedprintable(NULL,'=')==3);	// =3d
  zap(); assert(fmt_escapecharquotedprintable(buf,'=')==3 && byte_equal(buf,4,"=3d_"));
  assert(fmt_escapecharquotedprintable(NULL,0xf6)==3);	// =f6
  zap(); assert(fmt_escapecharquotedprintable(buf,0xf6)==3 && byte_equal(buf,4,"=f6_"));
  assert(fmt_escapecharquotedprintable(NULL,0x100)==0);

  assert(fmt_escapecharquotedprintableutf8(NULL,'=')==3);	// =3d
  zap(); assert(fmt_escapecharquotedprintableutf8(buf,'=')==3 && byte_equal(buf,4,"=3d_"));
  assert(fmt_escapecharquotedprintableutf8(NULL,0xf6)==6);	// =c3=b6
  zap(); assert(fmt_escapecharquotedprintableutf8(buf,0xf6)==6 && byte_equal(buf,7,"=c3=b6_"));

  assert(scan_ulong("23",&ul)==2 && ul==23);
  assert(scan_ulong("46halbe",&ul)==2 && ul==46);
  if (sizeof(ul)==4) {
    assert(scan_ulong("4294967295",&ul)==10 && ul==0xffffffff);
    assert(scan_ulong("4294967296",&ul)==9 && ul==429496729);
    assert(scan_xlong("ffffffff",&ul)==8 && ul==0xffffffff);
    assert(scan_xlong("ffffffff0",&ul)==8 && ul==0xffffffff);
    assert(scan_8long("37777777777",&ul)==11 && ul==0xffffffff);
    assert(scan_8long("377777777771",&ul)==11 && ul==0xffffffff);

    assert(scan_long("2147483647",&l)==10 && l==0x7fffffff);
    assert(scan_long("02147483647",&l)==11 && l==0x7fffffff);
    assert(scan_long("021474836470",&l)==11 && l==0x7fffffff);
    assert(scan_long("+2147483647",&l)==11 && l==0x7fffffff);
    assert(scan_long("+2147483648",&l)==10 && l==214748364);
    assert(scan_long("-2147483647",&l)==11 && l==-2147483647);
    assert(scan_long("-2147483648",&l)==11 && l==-2147483648);
    assert(scan_long("-2147483649",&l)==10 && l==-214748364);

  } else {
    assert(scan_ulong("18446744073709551615",&ul)==20 && ul==0xffffffffffffffffull);
    assert(scan_ulong("18446744073709551616",&ul)==19 && ul==1844674407370955161ull);
    assert(scan_xlong("ffffffffffffffff",&ul)==16 && ul==0xffffffffffffffffull);
    assert(scan_xlong("ffffffffffffffff0",&ul)==16 && ul==0xffffffffffffffffull);
    assert(scan_8long("1777777777777777777777",&ul)==22 && ul==0xffffffffffffffffull);
    assert(scan_8long("17777777777777777777770",&ul)==22 && ul==0xffffffffffffffffull);

    assert(scan_long("9223372036854775807",&l)==19 && l==0x7fffffffffffffffll);
    assert(scan_long("09223372036854775807",&l)==20 && l==0x7fffffffffffffffll);
    assert(scan_long("092233720368547758070",&l)==20 && l==0x7fffffffffffffffll);
    assert(scan_long("+9223372036854775807",&l)==20 && l==0x7fffffffffffffffll);
    assert(scan_long("+9223372036854775808",&l)==19 && l==922337203685477580ll);
    assert(scan_long("-9223372036854775807",&l)==20 && l==-9223372036854775807ll);
    x=scan_long("-9223372036854775808",&l);
    assert(scan_long("-9223372036854775808",&l)==20 && l==0x8000000000000000);
    assert(scan_long("-9223372036854775809",&l)==19 && l==-922337203685477580ll);

  }
  assert(scan_uint("4294967295",&ui)==10 && ui==0xffffffff);
  assert(scan_uint("4294967296",&ui)==9 && ui==429496729);
  assert(scan_ushort("65535",&us)==5 && us==0xffff);
  assert(scan_ushort("65536",&us)==4 && us==6553);

  assert(scan_xint("0ffffffff",&ui)==9 && ui==0xffffffff);
  assert(scan_xint("ffffffff0",&ui)==8 && ui==0xffffffff);
  assert(scan_xshort("0ffff",&us)==5 && us==0xffff);
  assert(scan_xshort("ffff1",&us)==4 && us==0xffff);

  assert(scan_8int("037777777777",&ui)==12 && ui==0xffffffff);
  assert(scan_8int("377777777771",&ui)==11 && ui==0xffffffff);
  assert(scan_8short("00177777",&us)==8 && us==0xffff);
  assert(scan_8short("1777771",&us)==6 && us==0xffff);

  assert(scan_int("2147483647",&i)==10 && i==0x7fffffff);
  assert(scan_int("02147483647",&i)==11 && i==0x7fffffff);
  assert(scan_int("021474836470",&i)==11 && i==0x7fffffff);
  assert(scan_int("+2147483647",&i)==11 && i==0x7fffffff);
  assert(scan_int("+2147483648",&i)==10 && i==214748364);
  assert(scan_int("-2147483647",&i)==11 && i==-2147483647);
  assert(scan_int("-2147483648",&i)==11 && i==-2147483648);
  assert(scan_int("-2147483649",&i)==10 && i==-214748364);

  assert(scan_short("32767",&s)==5 && s==32767);
  assert(scan_short("32768",&s)==4 && s==3276);
  assert(scan_short("+032767",&s)==7 && s==32767);
  assert(scan_short("-32768",&s)==6 && s==-32768);
  assert(scan_short("-032769",&s)==6 && s==-3276);
}

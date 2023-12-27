CC ?= /usr/bin/cc
CFLAGS += -Wall -march=native -mtune=native -O3 -fomit-frame-pointer -Wno-unknown-pragmas
LDFLAGS += -lcrypto -ldl

SOURCES = pke.c poly.c reduce.c coding.c randombytes.c cbd.c\
  cpucycles.c speed.c pack.c kem.c fips202.c inverse.c 

HEADERS = params.h pke.h poly.h reduce.h coding.h randombytes.h cbd.h \
  cpucycles.h speed.h pack.h kem.h fips202.h inverse.h symmetric_crypto.h 
  
SOURCES_N653 = $(SOURCES) poly_mul_n653q/radix_ntt_n653.c 
HEADERS_N653 = $(HEADERS) poly_mul_n653q/radix_ntt_n653.h 

SOURCES_N761 = $(SOURCES) poly_mul_n761q/radix_ntt_n761.c 
HEADERS_N761 = $(HEADERS) poly_mul_n761q/radix_ntt_n761.h

SOURCES_N1277 = $(SOURCES) poly_mul_n1277q/radix_ntt_n1277.c
HEADERS_N1277 = $(HEADERS) poly_mul_n1277q/radix_ntt_n1277.h

all: test_kem653 test_speed653 \
	test_kem761 test_speed761 \
	test_kem1277 test_speed1277 \

test_kem653: test_kem.c $(SOURCES_N653) $(HEADERS_N653)
	$(CC) $(CFLAGS) -DFPTRU_N=653 $< $(SOURCES_N653) -o $@ $(LDFLAGS)

test_speed653: test_speed.c $(SOURCES_N653) $(HEADERS_N653)
	$(CC) $(CFLAGS) -DFPTRU_N=653 $< $(SOURCES_N653) -o $@ $(LDFLAGS)

test_kem761: test_kem.c $(SOURCES_N761) $(HEADERS_N761)
	$(CC) $(CFLAGS) -DFPTRU_N=761 $< $(SOURCES_N761) -o $@ $(LDFLAGS)

test_speed761: test_speed.c $(SOURCES_N761) $(HEADERS_N761)
	$(CC) $(CFLAGS) -DFPTRU_N=761 $< $(SOURCES_N761) -o $@ $(LDFLAGS)

test_kem1277: test_kem.c $(SOURCES_N1277) $(HEADERS_N1277)
	$(CC) $(CFLAGS) -DFPTRU_N=1277 $< $(SOURCES_N1277) -o $@ $(LDFLAGS)

test_speed1277: test_speed.c $(SOURCES_N1277) $(HEADERS_N1277)
	$(CC) $(CFLAGS) -DFPTRU_N=1277 $< $(SOURCES_N1277) -o $@ $(LDFLAGS)

clean:
	rm -f *~ *~
	rm -f test_kem653
	rm -f test_speed653	
	rm -f test_kem761 
	rm -f test_speed761	
	rm -f test_kem1277 
	rm -f test_speed1277

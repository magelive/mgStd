lib_LTLIBRARIES=libmgStd.la 
libmgStd_la_CFLAGS=$(AM_FLGS) -fPIC -shared -D_LARGEFILE64_SOURCE=1 -std=c99
libmgStd_la_LIBADD=
libmgStd_la_SOURCES=mglist.c mgMurmurhash.c mgtime.c mgmd5.c mgsha1.c mgbase64.c mgsha256.c mgsha384.c mgsha512.c
libmgStd_HEADERS=mglist.h mgMurmurhash.h mgtime.h mgmd5.h mgsha1.h mgbase64.h mgsha256.h mgsha384.h mgsha512.h
libmgStddir=@includedir@/mg

#bin_PROGRAMS=mglist_test
noinst_PROGRAMS=mglist_test mgMurmurhash_test mgmd5_test mghlist_test mgsha1_test mgbase64_test mgsha256_test mgsha384_test mgsha512_test


mglist_test_DEPENDENCIES=./libmgStd.la
mglist_test_LDADD=./libmgStd.la
mglist_test_SOURCES=mglist_test.c
mglist_test_LDFLAGS=

mghlist_test_DEPENDENCIES=./libmgStd.la
mghlist_test_LDADD=./libmgStd.la
mghlist_test_SOURCES=mghlist_test.c
mghlist_test_LDFLAGS=

mgMurmurhash_test_DEPENDENCIES=./libmgStd.la
mgMurmurhash_test_LDADD=./libmgStd.la
mgMurmurhash_test_SOURCES=mgMurmurhash_test.c
mgMurmurhash_test_LDFLAGS=

mgmd5_test_DEPENDENCIES=./libmgStd.la
mgmd5_test_LDADD=./libmgStd.la
mgmd5_test_SOURCES=mgmd5_test.c
mgmd5_test_LDFLAGS=

mgsha1_test_DEPENDENCIES=./libmgStd.la
mgsha1_test_LDADD=./libmgStd.la
mgsha1_test_SOURCES=mgsha1_test.c
mgsha1_test_LDFLAGS=

mgsha256_test_DEPENDENCIES=./libmgStd.la
mgsha256_test_LDADD=./libmgStd.la
mgsha256_test_SOURCES=mgsha256_test.c
mgsha256_test_LDFLAGS=

mgsha384_test_DEPENDENCIES=./libmgStd.la
mgsha384_test_LDADD=./libmgStd.la
mgsha384_test_SOURCES=mgsha384_test.c
mgsha384_test_LDFLAGS=

mgsha512_test_DEPENDENCIES=./libmgStd.la
mgsha512_test_LDADD=./libmgStd.la
mgsha512_test_SOURCES=mgsha512_test.c
mgsha512_test_LDFLAGS=

mgbase64_test_DEPENDENCIES=./libmgStd.la
mgbase64_test_LDADD=./libmgStd.la
mgbase64_test_SOURCES=mgbase64_test.c
mgbase64_test_LDFLAGS=

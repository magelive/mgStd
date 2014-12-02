lib_LTLIBRARIES=libmgStd.la 
libmgStd_la_CFLAGS=$(AM_FLGS) -fPIC -shared -D_LARGEFILE64_SOURCE=1 -std=c99
libmgStd_la_LIBADD=
libmgStd_la_SOURCES=mglist.c mgMurmurhash.c mgtime.c
libmgStd_HEADERS=mglist.h mgMurmurhash.h mgtime.h
libmgStddir=@includedir@/mg

#bin_PROGRAMS=mglist_test
noinst_PROGRAMS=mglist_test mgMurmurhash_test


mglist_test_DEPENDENCIES=./libmgStd.la
mglist_test_LDADD=./libmgStd.la
mglist_test_SOURCES=mglist_test.c
mglist_test_LDFLAGS=

mgMurmurhash_test_DEPENDENCIES=./libmgStd.la
mgMurmurhash_test_LDADD=./libmgStd.la
mgMurmurhash_test_SOURCES=mgMurmurhash_test.c
mgMurmurhash_test_LDFLAGS=

#mgsyslogtest_DEPENDENCIES=./libmgLog.la ./libmgsyslog.la
#mgsyslogtest_LDADD=./libmgLog.la ./libmgsyslog.la
#mgsyslogtest_SOURCES=mgsyslogtest.c
#mgsyslogtest_LDFLAGS=

#mgsyslog_server_LDADD=
#mgsyslog_server_SOURCES=mgsyslog_server.c
#mgsyslog_server_LDFLAGS=

/*
 * md5.h        Structures and prototypes for md5.
 *
 * Version:     $Id$
 * License:		LGPL, but largely derived from a public domain source.
 *
 */

#ifndef _MG_MD5_H
#define _MG_MD5_H

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#include <string.h>

#ifdef WITH_OPENSSL_MD5
#include <openssl/md5.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WITH_OPENSSL_MD5
/*  The below was retrieved from
 *  http://www.openbsd.org/cgi-bin/cvsweb/~checkout~/src/sys/crypto/md5.h?rev=1.1
 *  With the following changes: uint64_t => unsigned int[2]
 *  Commented out #include <sys/cdefs.h>
 *  Commented out the __BEGIN and __END _DECLS, and the __attributes.
 */

/*
 * This code implements the MD5 message-digest algorithm.
 * The algorithm is due to Ron Rivest.  This code was
 * written by Colin Plumb in 1993, no copyright is claimed.
 * This code is in the public domain; do with it what you wish.
 *
 * Equivalent code is available from RSA Data Security, Inc.
 * This code has been tested against that, and is equivalent,
 * except that you don't need to include two pages of legalese
 * with every copy.
 */

#define	MD5_BLOCK_LENGTH		64
#define	MD5_DIGEST_LENGTH		16

typedef struct MG_MD5Context {
	unsigned int state[4];			/* state */
	unsigned int count[2];			/* number of bits, mod 2^64 */
	unsigned char buffer[MD5_BLOCK_LENGTH];	/* input buffer */
} MG_MD5_CTX;

/* include <sys/cdefs.h> */

void mg_md5_calc(unsigned char *output, const unsigned char *input,
                    unsigned int inlen);

void mg_md5_file(unsigned char *output, char *filename);
/* __BEGIN_DECLS */
void	 mg_MD5Init(MG_MD5_CTX *);
void	 mg_MD5Update(MG_MD5_CTX *, const unsigned char *, size_t)
/*		__attribute__((__bounded__(__string__,2,3)))*/;
void	 mg_MD5Final(unsigned char [MD5_DIGEST_LENGTH], MG_MD5_CTX *)
/*		__attribute__((__bounded__(__minbytes__,1,MD5_DIGEST_LENGTH)))*/;
void	 mg_MD5Transform(unsigned int [4], const unsigned char [MD5_BLOCK_LENGTH])
/*		__attribute__((__bounded__(__minbytes__,1,4)))*/
/*		__attribute__((__bounded__(__minbytes__,2,MD5_BLOCK_LENGTH)))*/;
/* __END_DECLS */

#else  /* WITH_OPENSSL_HASH */

#define MG_MD5_CTX	MD5_CTX
#define mg_MD5Init	MD5_Init
#define mg_MD5Update	MD5_Update
#define mg_MD5Final	MD5_Final
#define mg_MD5Transform MD5_Transform
#endif

#ifdef __cplusplus
}
#endif

#endif /* _MG_MD5_H */

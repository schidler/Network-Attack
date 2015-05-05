/*
 * util.c:
 * Various utility functions.
 *
 * Copyright (c) 2003 Chris Lightfoot. All rights reserved.
 * Email: chris@ex-parrot.com; WWW: http://www.ex-parrot.com/~chris/
 *
 */

static const char rcsid[] = "$Id: util.c,v 1.1 2003/08/25 12:24:08 chris Exp $";

#include <stdlib.h>
#include <string.h>

#include "driftnet.h"

/* xmalloc COUNT
 * Malloc, and abort if malloc fails. */
void *xmalloc(size_t n) {
    void *v;
    v = malloc(n);
    if (!v) abort();
    return v;
}

/* xcalloc NITEMS COUNT
 * As above. */
void *xcalloc(size_t n, size_t m) {
    void *v;
    v = calloc(n, m);
    if (!v) abort();
    return v;
}

/* xrealloc PTR COUNT
 * As above. */
void *xrealloc(void *w, size_t n) {
    void *v;
    v = realloc(w, n);
    if (n != 0 && !v) abort();
    return v;
}

/* xfree PTR
 * Free, ignoring a passed NULL value. */
void xfree(void *v) {
    if (v) free(v);
}

/* xstrdup:
 * Strdup, aborting on failure. */
char *xstrdup(const char *s) {
    char *t;
    t = xmalloc(strlen(s) + 1);
    strcpy(t, s);
    return t;
}

/* memstr:
 * Locate needle, of length n_len, in haystack, of length h_len, returning NULL.
 * Uses the Boyer-Moore search algorithm. Cf.
 *  http://www-igm.univ-mlv.fr/~lecroq/string/node14.html */
unsigned char *memstr(const unsigned char *haystack, const size_t hlen,
                             const unsigned char *needle, const size_t nlen) {
    int skip[256], k;

    if (nlen == 0) return (unsigned char*)haystack;

    /* Set up the finite state machine we use. */
    for (k = 0; k < 256; ++k) skip[k] = nlen;
    for (k = 0; k < nlen - 1; ++k) skip[needle[k]] = nlen - k - 1;

    /* Do the search. */
    for (k = nlen - 1; k < hlen; k += skip[haystack[k]]) {
        int i, j;
        for (j = nlen - 1, i = k; j >= 0 && haystack[i] == needle[j]; j--) i--;
        if (j == -1) return (unsigned char*)(haystack + i + 1);
    }

    return NULL;
}


char *gethttp_header(const char *tcpdata, int len)
{
	char *delimi = NULL;
	delimi = memstr(tcpdata, len, (unsigned char *)"\r\n\r\n", 4);
	if(NULL == delimi)
		return NULL;
	char *http_header = NULL;
	int http_header_len = delimi - tcpdata;
	printf("http_header_length:%d\n", http_header_len);
	http_header = xmalloc(http_header_len + 1);
	http_header[http_header_len] = 0;
	memcpy(http_header, tcpdata, http_header_len);
	return http_header;
}

int ishttp(const char *tcpdata, int len)
{
	if(memstr(tcpdata, len, (unsigned char *)"\r\n\r\n", 4))
		return 1;
	return 0;
}


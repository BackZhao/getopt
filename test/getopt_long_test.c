/* -*- indent-tabs-mode: nil -*-
 *
 * ya_getopt  - Yet another getopt
 * https://github.com/kubo/ya_getopt
 *
 * Copyright 2015 Kubo Takehiro <kubo@jiubao.org>
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice, this list of
 *       conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright notice, this list
 *       of conditions and the following disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied, of the authors.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/resource.h>

#ifndef __linux
#error This program was made for Linux to check compatibility with GNU C Library getopt and ya_getopt.
#endif

#ifdef USE_YA_GETOPT
#include "ya_getopt.h"
#else
#include <getopt.h>
#endif

static int bar_flag;

static struct option longopts[] = {
    {"foo", no_argument, NULL, 1},
    {"bar", required_argument, &bar_flag, 2},
    {"baz", optional_argument, NULL, 3},
    {"qux", 4 /* invalid value */, NULL, 4},
    {NULL,},
};

static void print_opts(int opt, int argc, char **argv, int longindex)
{
    int i;

    if (isprint(opt)) {
        fprintf(stderr, "retval = '%c', ", opt);
    } else {
        fprintf(stderr, "retval = %3d, ", opt);
    }
    fprintf(stderr, "optind = %d, ", optind);
    fprintf(stderr, "opterr = %d, ", opterr);
    if (isprint(optopt)) {
        fprintf(stderr, "optopt = '%c', ", optopt);
    } else {
        fprintf(stderr, "optopt = %3d, ", optopt);
    }
    if (optarg == NULL) {
        fprintf(stderr, "optarg = (null), ");
    } else {
        fprintf(stderr, "optarg = \"%s\", ", optarg);
    }
    fprintf(stderr, "longindex = %d, bar_flag = %d\n", longindex, bar_flag);
    for (i = 0; i < argc; i++) {
        fprintf(stderr, "'%s'%c", argv[i], (i + 1 == argc) ? '\n' : ' ');
    }
}

int main(int argc, char **argv)
{
    int opt = -1;
    const char *optstring = getenv("OPTSTRING");
    int longindex = -1;
    struct rlimit rlim = {1, 1};

    setrlimit(RLIMIT_CPU, &rlim); /* to prevent infinite loop */

    if (optstring == NULL) {
        optstring = "";
    }

    if (getenv("OPTERR") != NULL) {
        opterr = atoi(getenv("OPTERR"));
    }

    argv[0] = "getopt_test_long";
    print_opts(opt, argc, argv, longindex);
    while ((opt = getopt_long(argc, argv, optstring, longopts, &longindex)) != -1) {
        print_opts(opt, argc, argv, longindex);
    }
    print_opts(opt, argc, argv, longindex);
    return 0;
}

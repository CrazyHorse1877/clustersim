/*
 * Copyright (c) 1991, 1992, 1993 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the name of
 * Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Silicon Graphics.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF
 * ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT SHALL SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#ifndef _OPENGL_TOP_HPP_
#define _OPENGL_TOP_HPP_


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>
//#include "opengl_3d.hpp"

#define CI_RED COLOR_RED
#define CI_ANTI_ALIAS_GREEN 16
#define CI_ANTI_ALIAS_YELLOW 32
#define CI_ANTI_ALIAS_RED 48

static void Init(void);
static void Reshape(int width, int height);
static void Key2(int key, int x, int y);
static void Key(unsigned char key, int x, int y);
static void Draw(void);
static GLenum Args(int argc, char **argv);
int opengl_top(int argc, char **argv);


#endif

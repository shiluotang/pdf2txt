ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -Wall -I/usr/local/opt/mupdf/include
AM_CFLAGS 	=
AM_CXXFLAGS =
AM_LDFLAGS 	= -L/usr/local/opt/mupdf/lib/ -lmupdf -lmupdf-third

bin_PROGRAMS = pdf2txt
pdf2txt_SOURCES = src/pdf2txt.c
pdf2txt_LDFLAGS = $(AM_LDFLAGS) -static

TESTS = $(check_PROGRAMS)

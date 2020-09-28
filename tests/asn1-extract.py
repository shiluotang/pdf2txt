#!/usr/bin/env python
# -*- coding: utf-8 -*-


import io
import sys
import logging
import codecs


def extract_asn1(infile, outfile):
    asn1_mode = False
    paging_mode = False
    for line in infile:
        line = line.rstrip()
        if line.find('TAGS ::=') > 0 and not asn1_mode:
            asn1_mode = True
        elif line == 'END' and asn1_mode:
            outfile.write(line)
            outfile.write("\n")
            asn1_mode = False
        elif line.find('OMA-TS-ULP-V') == 0 and not paging_mode:
            paging_mode = True
        elif line.find('[OMA-Template-Spec-') == 0 and paging_mode:
            paging_mode = False
            line = None

        if asn1_mode and not paging_mode and line is not None:
            outfile.write(line)
            outfile.write("\n")
    pass


def usage(prog):
    print("%s infile [outfile]" % (prog))
    print("Options")
    print("\tinfile")
    print("\toutfile")
    sys.exit(1)
    pass


if __name__ == "__main__":
    if len(sys.argv) >= 3:
        infile = io.open(sys.argv[1], "r")
        outfile = codecs.getwriter('utf8')(io.open(sys.argv[2], "w"))
    elif len(sys.argv) == 2:
        infile = io.open(sys.argv[1], "r")
        outfile = codecs.getwriter('utf8')(sys.stdout)
    else:
        usage(sys.argv[0])
    with infile, outfile:
            extract_asn1(infile, outfile)

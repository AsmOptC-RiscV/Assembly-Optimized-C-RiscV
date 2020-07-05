#!/usr/bin/env python3

import os
import glob
import sys
from utils.tools import read, save, bcolors


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("usage: ./configure.py <scheme> <compiler> <platform>")
        print("compilers: GCC, GCC_B, Clang, clang-10")
        print("platform:  sifive, murax, riscvOPVsim, spike")
        exit(0)

    scheme_name = sys.argv[1]
    compiler = sys.argv[2]
    platform = sys.argv[3]

    if not os.path.exists(scheme_name + '/base.h'):
        print(bcolors.BOLD + bcolors.RED + scheme_name +
              "/base.h not found" + bcolors.DARKGRAY)
        print(bcolors.NORMAL + bcolors.LIGHTGRAY + "available schemes: " +
              ", ".join([x[:-7] for x in glob.glob("*/base.h")]))
        exit(1)

    compilers = ["gcc", "gcc_b", "clang", "clang-10", "clang-9"]
    if compiler.lower() not in compilers:
        print(bcolors.BOLD + bcolors.RED + "Compiler " +
              compiler + " not found." + bcolors.DARKGRAY)
        print(bcolors.NORMAL + bcolors.LIGHTGRAY +
              "available compilers: " + ", ".join(compilers) + bcolors.DARKGRAY)
        exit(1)

    configs = [x for x in glob.glob(
        "*.mk") if x not in [x + ".mk" for x in ["clang", "gcc", "gcc_b", "Make", "config", "clang-10"]]]
    if platform+'.mk' not in configs:
        print(bcolors.BOLD + bcolors.RED + platform +
              ".mk not found." + bcolors.DARKGRAY)
        print(bcolors.NORMAL + bcolors.LIGHTGRAY + "available platforms: " +
              ", ".join([x[:-3] for x in configs]) + bcolors.DARKGRAY)
        exit(1)

    main = read('Make.mk')
    main = main.format(scheme=scheme_name,
                       compiler=compiler.lower().replace('-9',''), platform=platform)
    print(bcolors.PURPLE + "Generating Makefile for " +
          scheme_name + "." + bcolors.DARKGRAY)
    save('Makefile', main)

    main = read('scheme.tpl.h')
    main = main.format(scheme=scheme_name)
    print(bcolors.PURPLE + "Generating scheme.h for " +
          scheme_name + "." + bcolors.DARKGRAY)
    save('scheme.h', main)

    exit(0)

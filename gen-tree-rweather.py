#!/usr/bin/python3

import os
import glob
import sys
from utils.tools import read, save, bcolors
from os import listdir

if __name__ == "__main__":
    action = 'install';
    if len(sys.argv) > 1:
        action = sys.argv[1]

    ciphers = [f for f in os.listdir('rweather-repo/src/individual') if not f == 'Makefile']

    if action == 'install':
        for cipher in ciphers:
            if not os.path.isdir('rweather/' + cipher) :
                os.mkdir('rweather/' + cipher)
            src = '../../rweather-repo/src/individual/' + cipher
            dst = 'rweather/' + cipher + '/src'
            try:
                os.symlink(src, dst)
                print('set up symbolic link to ' + dst)
            except:
                print('link to ' + src + ' already exists!')

    if action == 'remove' or action == 'uninstall':
        for cipher in ciphers:
            if os.path.isdir('rweather/' + cipher) :
                if os.path.exists('rweather/' + cipher + '/src') :
                    os.unlink('rweather/' + cipher + '/src')
                    print('removed symbolic link rweather/' + cipher + '/src')
                else:
                    print('rweather/' + cipher + '/src not found')


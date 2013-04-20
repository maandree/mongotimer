#!/usr/bin/env python
# 
# Copyright © 2013  Mattias Andrée  (maandree@member.fsf.org)
# 
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.  [GNU All Permissive License]

import os
import time
import sys
from subprocess import Popen, PIPE

def awesome(list):
    return [elem.replace('X', ' ').replace('[', '\033[07m ').replace(']', '\033[00m ') for elem in list]


MONGO_0 = ['    [XXXX]      ',
           '  [XX]  [XX]    ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '[XX]  []  [XX]  ',
           '[XX]  []  [XX]  ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '  [XX]  [XX]    ',
           '    [XXXX]      ']

MONGO_1 = ['      [XX]      ',
           '    [XXXX]      ',
           '  [XXXXXX]      ',
           '      [XX]      ',
           '      [XX]      ',
           '      [XX]      ',
           '      [XX]      ',
           '      [XX]      ',
           '      [XX]      ',
           '      [XX]      ',
           '      [XX]      ',
           '  [XXXXXXXXXX]  ']

MONGO_2 = ['  [XXXXXXXX]    ',
           '[XX]      [XX]  ',
           '          [XX]  ',
           '          [XX]  ',
           '          [XX]  ',
           '        [XX]    ',
           '      [XX]      ',
           '    [XX]        ',
           '  [XX]          ',
           '[XX]            ',
           '[XX]      [XX]  ',
           '[XXXXXXXXXXXX]  ']

MONGO_3 = ['  [XXXXXXXX]    ',
           '[XX]      [XX]  ',
           '          [XX]  ',
           '          [XX]  ',
           '          [XX]  ',
           '    [XXXXXX]    ',
           '          [XX]  ',
           '          [XX]  ',
           '          [XX]  ',
           '          [XX]  ',
           '[XX]      [XX]  ',
           '  [XXXXXXXX]    ']

MONGO_4 = ['        [XX]    ',
           '      [XXXX]    ',
           '    [XXXXXX]    ',
           '  [XX]  [XX]    ',
           '[XX]    [XX]    ',
           '[XXXXXXXXXXXX]  ',
           '        [XX]    ',
           '        [XX]    ',
           '        [XX]    ',
           '        [XX]    ',
           '        [XX]    ',
           '      [XXXXXX]  ']

MONGO_5 = ['[XXXXXXXXXXXX]  ',
           '[XX]            ',
           '[XX]            ',
           '[XX]            ',
           '[XX]            ',
           '[XXXXXXXXXX]    ',
           '          [XX]  ',
           '          [XX]  ',
           '          [XX]  ',
           '          [XX]  ',
           '[XX]      [XX]  ',
           '  [XXXXXXXX]    ']

MONGO_6 = ['      [XXXX]    ',
           '    [XX]        ',
           '  [XX]          ',
           '[XX]            ',
           '[XX]            ',
           '[XXXXXXXXXX]    ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '  [XXXXXXXX]    ']

MONGO_7 = ['[XXXXXXXXXXXX]  ',
           '[XX]      [XX]  ',
           '          [XX]  ',
           '          [XX]  ',
           '        [XX]    ',
           '      [XX]      ',
           '    [XX]        ',
           '    [XX]        ',
           '    [XX]        ',
           '    [XX]        ',
           '    [XX]        ',
           '    [XX]        ']

MONGO_8 = ['  [XXXXXXXX]    ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '  [XXXXXXXX]    ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '  [XXXXXXXX]    ']

MONGO_9 = ['  [XXXXXXXX]    ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '[XX]      [XX]  ',
           '  [XXXXXXXXXX]  ',
           '          [XX]  ',
           '          [XX]  ',
           '          [XX]  ',
           '        [XX]    ',
           '      [XX]      ',
           '  [XXXX]        ']

MONGO__ = ['                ',
           '                ',
           '      [XX]      ',
           '      [XX]      ',
           '                ',
           '                ',
           '                ',
           '                ',
           '      [XX]      ',
           '      [XX]      ',
           '                ',
           '                ']


DX = len(MONGO_0[0])
DC = len(MONGO__[0])


MONGO_0 = awesome(MONGO_0)
MONGO_1 = awesome(MONGO_1)
MONGO_2 = awesome(MONGO_2)
MONGO_3 = awesome(MONGO_3)
MONGO_4 = awesome(MONGO_4)
MONGO_5 = awesome(MONGO_5)
MONGO_6 = awesome(MONGO_6)
MONGO_7 = awesome(MONGO_7)
MONGO_8 = awesome(MONGO_8)
MONGO_9 = awesome(MONGO_9)
MONGO__ = awesome(MONGO__)


def print_digit(digit, x, y):
    d = MONGO__
    if   digit == 0:  d = MONGO_0
    elif digit == 1:  d = MONGO_1
    elif digit == 2:  d = MONGO_2
    elif digit == 3:  d = MONGO_3
    elif digit == 4:  d = MONGO_4
    elif digit == 5:  d = MONGO_5
    elif digit == 6:  d = MONGO_6
    elif digit == 7:  d = MONGO_7
    elif digit == 8:  d = MONGO_8
    elif digit == 9:  d = MONGO_9
    x += 1
    for i in range(len(d)):
        y += 1
        print("\033[%i;%iH%s" % (y, x, d[i]))

def print_number(number, x, y):
    print_digit(number // 10, x, y)
    print_digit(number % 10, x + DX, y)

def print_time(h, m, s, x, y):
    print_number(h, x, y)
    x += DX << 1
    print_digit(-1, x, y)
    x += DC
    print_number(m, x, y)
    if s is not None:
        x += DX << 1
        print_digit(-1, x, y)
        x += DC
        print_number(s, x, y)

def get_print_width(has_seconds):
    return DC + 4 * DX + ((DC + 2 * DX) if has_seconds else 0)

def get_print_height():
    return len(MONGO_0)

def get_print_offset(has_seconds):
    (h, w) = get_dimensions()
    return ((w - get_print_width(has_seconds)) // 2, (h - get_print_height()) // 2)

def fits(has_seconds):
    (h, w) = get_dimensions()
    return (get_print_width(has_seconds) <= w) and (get_print_height() <= h)

def get_dimensions():
    rc = Popen(['stty', 'size'], stdout = PIPE).communicate()[0]
    rc = rc.decode('utf-8')
    rc = rc.replace('\n', '').split(' ')
    return [int(x) for x in rc]


if __name__ == '__main__':
    val = None
    if fits(True):
        val = True
    elif fits(False):
        val = False
    else:
        print('Screen is too small')
        exit(1)
    (x, y) = get_print_offset(val)
    last = (-1, -1, -1)
    while True:
        (h, m, s) = [int(x) for x in time.strftime('%H %M %S', time.localtime()).split(' ')]
        if not val:
            s = None
        if h != last[0] or m != last[1] or s != last[2]:
            print('\033[H\033[2J', end='')
            print_time(h, m, s, x, y)
            print('\033[1;1H', end='')
            sys.stdout.flush()
            last = (h, m, s)
        time.sleep(0.1)


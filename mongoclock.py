def awesome(e):
    return e.replace('X', ' ').replace('[', '\033[07m ').replace(']', '\033[00m ')


MONGO_0 = [awesome(e) for e in
          ['    [XXXX]      ',
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
           '    [XXXX]      ']]

MONGO_1 = [awesome(e) for e in
          ['      [XX]      ',
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
           '  [XXXXXXXXXX]  ']]

MONGO_2 = [awesome(e) for e in
          ['  [XXXXXXXX]    ',
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
           '[XXXXXXXXXXXX]  ']]

MONGO_3 = [awesome(e) for e in
          ['  [XXXXXXXX]    ',
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
           '  [XXXXXXXX]    ']]

MONGO_4 = [awesome(e) for e in
          ['        [XX]    ',
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
           '      [XXXXXX]  ']]

MONGO_5 = [awesome(e) for e in
          ['[XXXXXXXXXXXX]  ',
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
           '  [XXXXXXXX]    ']]

MONGO_6 = [awesome(e) for e in
          ['      [XXXX]    ',
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
           '  [XXXXXXXX]    ']]

MONGO_7 = [awesome(e) for e in
          ['[XXXXXXXXXXXX]  ',
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
           '    [XX]        ']]

MONGO_8 = [awesome(e) for e in
          ['  [XXXXXXXX]    ',
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
           '  [XXXXXXXX]    ']]

MONGO_9 = [awesome(e) for e in
          ['  [XXXXXXXX]    ',
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
           '  [XXXX]        ']]

MONGO__ = [awesome(e) for e in
          ['                ',
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
           '                ']]


DX = len(MONGO_0[0])


class mongoclock:
    def __init__(self):
        pass
    
    
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
            print("\033[%i;%iH%s", y, x, d[i])
    
    def print_number(number, x, y):
        print_digit(number // 10, x, y)
        print_digit(number % 10, x + DX, y)
    
    def print_time(h, m, s, x, y):
        print_number(h, x, y)
        x += DX << 1
        print_digit(-1, x, y)
        x += DX
        print_number(m, x, y)
        if s is not None:
            x += DX << 1
            print_digit(-1, x, y)
            x += DX
            print_number(s, x, y)
    
    def get_print_width(has_seconds):
        return 5 * DX + (3 * DX if has_seconds else 0)


if __name__ == '__main__':
    mongoclock()


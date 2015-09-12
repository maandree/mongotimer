/**
 * Copyright © 2013, 2015  Mattias Andrée  (maandree@member.fsf.org)
 * 
 * Copying and distribution of this file, with or without modification,
 * are permitted in any medium without royalty provided the copyright
 * notice and this notice are preserved.  This file is offered as-is,
 * without any warranty.  [GNU All Permissive License]
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <stddef.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/timerfd.h>

$<
d() {
  echo '"'"$(echo "$1" | sed -e 's/X/ /g' -e 's/\[/\\033\[7m /g' -e 's/\]/\\033\[m /g')"'"'
}
$>

#define DX  16
#define DY  12
static const char* mongo_0[] = {$(d '    [XXXX]      '),
				$(d '  [XX]  [XX]    '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]  []  [XX]  '),
				$(d '[XX]  []  [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '  [XX]  [XX]    '),
				$(d '    [XXXX]      ')};

static const char* mongo_1[] = {$(d '      [XX]      '),
				$(d '    [XXXX]      '),
				$(d '  [XXXXXX]      '),
				$(d '      [XX]      '),
				$(d '      [XX]      '),
				$(d '      [XX]      '),
				$(d '      [XX]      '),
				$(d '      [XX]      '),
				$(d '      [XX]      '),
				$(d '      [XX]      '),
				$(d '      [XX]      '),
				$(d '  [XXXXXXXXXX]  ')};

static const char* mongo_2[] = {$(d '  [XXXXXXXX]    '),
				$(d '[XX]      [XX]  '),
				$(d '          [XX]  '),
				$(d '          [XX]  '),
				$(d '          [XX]  '),
				$(d '        [XX]    '),
				$(d '      [XX]      '),
				$(d '    [XX]        '),
				$(d '  [XX]          '),
				$(d '[XX]            '),
				$(d '[XX]      [XX]  '),
				$(d '[XXXXXXXXXXXX]  ')};

static const char* mongo_3[] = {$(d '  [XXXXXXXX]    '),
				$(d '[XX]      [XX]  '),
				$(d '          [XX]  '),
				$(d '          [XX]  '),
				$(d '          [XX]  '),
				$(d '    [XXXXXX]    '),
				$(d '          [XX]  '),
				$(d '          [XX]  '),
				$(d '          [XX]  '),
				$(d '          [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '  [XXXXXXXX]    ')};

static const char* mongo_4[] = {$(d '        [XX]    '),
				$(d '      [XXXX]    '),
				$(d '    [XXXXXX]    '),
				$(d '  [XX]  [XX]    '),
				$(d '[XX]    [XX]    '),
				$(d '[XXXXXXXXXXXX]  '),
				$(d '        [XX]    '),
				$(d '        [XX]    '),
				$(d '        [XX]    '),
				$(d '        [XX]    '),
				$(d '        [XX]    '),
				$(d '      [XXXXXX]  ')};

static const char* mongo_5[] = {$(d '[XXXXXXXXXXXX]  '),
				$(d '[XX]            '),
				$(d '[XX]            '),
				$(d '[XX]            '),
				$(d '[XX]            '),
				$(d '[XXXXXXXXXX]    '),
				$(d '          [XX]  '),
				$(d '          [XX]  '),
				$(d '          [XX]  '),
				$(d '          [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '  [XXXXXXXX]    ')};

static const char* mongo_6[] = {$(d '      [XXXX]    '),
				$(d '    [XX]        '),
				$(d '  [XX]          '),
				$(d '[XX]            '),
				$(d '[XX]            '),
				$(d '[XXXXXXXXXX]    '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '  [XXXXXXXX]    ')};

static const char* mongo_7[] = {$(d '[XXXXXXXXXXXX]  '),
				$(d '[XX]      [XX]  '),
				$(d '          [XX]  '),
				$(d '          [XX]  '),
				$(d '        [XX]    '),
				$(d '      [XX]      '),
				$(d '    [XX]        '),
				$(d '    [XX]        '),
				$(d '    [XX]        '),
				$(d '    [XX]        '),
				$(d '    [XX]        '),
				$(d '    [XX]        ')};

static const char* mongo_8[] = {$(d '  [XXXXXXXX]    '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '  [XXXXXXXX]    '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '  [XXXXXXXX]    ')};

static const char* mongo_9[] = {$(d '  [XXXXXXXX]    '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '[XX]      [XX]  '),
				$(d '  [XXXXXXXXXX]  '),
				$(d '          [XX]  '),
				$(d '          [XX]  '),
				$(d '          [XX]  '),
				$(d '        [XX]    '),
				$(d '      [XX]      '),
				$(d '  [XXXX]        ')};

#define DC  16
static const char* mongo_c[] = {$(d '                '),
				$(d '                '),
				$(d '      [XX]      '),
				$(d '      [XX]      '),
				$(d '                '),
				$(d '                '),
				$(d '                '),
				$(d '                '),
				$(d '      [XX]      '),
				$(d '      [XX]      '),
				$(d '                '),
				$(d '                ')};

static const char** mongo_ds[] = {
  mongo_0, mongo_1, mongo_2, mongo_3, mongo_4,
  mongo_5, mongo_6, mongo_7, mongo_8, mongo_9};


static volatile sig_atomic_t caught_sigterm = 0;
static volatile sig_atomic_t caught_sigwinch = 1;


#define t(...)  do if ((__VA_ARGS__) < 0) goto fail; while (0)


static void sigterm(int signo)
{
  caught_sigterm = 1;
  (void) signo;
}

static void sigwinch(int signo)
{
  caught_sigwinch = 1;
  (void) signo;
}


static void print_time(const char*** str, size_t y, size_t x)
{
  size_t r, c;
  
  fprintf(stdout, "\033[%zu;1H\033[1J", y + 1);
  
  for (r = 0; r < DY; r++)
    {
      fprintf(stdout, "\033[%zu;%zuH\033[1K", y + r + 1, x + 1);
      for (c = 0; str[c] != NULL; c++)
	fprintf(stdout, "%s", str[c][r]);
      fprintf(stdout, "\033[0K");
    }
  
  fprintf(stdout, "\033[0J");
  fflush(stdout);
}


int main(int argc, char* argv[])
{
  time_t now_;
  struct tm* now;
  const char** digits[9];
  size_t x = 0, y = 0;
  struct winsize winsize;
  int small = 0, fd;
  struct itimerspec itimerspec;
  char _buf[8];
  
  fprintf(stdout, "\033[?1049h\033[?25l");
  
  t (clock_gettime(CLOCK_REALTIME, &(itimerspec.it_value)));
  itimerspec.it_interval.tv_sec = 1;
  itimerspec.it_interval.tv_nsec = 0;
  itimerspec.it_value.tv_sec += 1;
  itimerspec.it_value.tv_nsec = 0;
  t (fd = timerfd_create(CLOCK_REALTIME, 0));
  t (timerfd_settime(fd, TFD_TIMER_ABSTIME, &itimerspec, NULL));
  
  signal(SIGTERM, sigterm);
  signal(SIGQUIT, sigterm);
  signal(SIGINT, sigterm);
  signal(SIGWINCH, sigwinch);
  siginterrupt(SIGTERM, 1);
  siginterrupt(SIGQUIT, 1);
  siginterrupt(SIGINT, 1);
  siginterrupt(SIGWINCH, 1);
  
  while (!caught_sigterm)
    {
      if (caught_sigwinch)
	{
	  caught_sigwinch = 0;
	  t (TEMP_FAILURE_RETRY(ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize)));
	  y = winsize.ws_row;
	  x = winsize.ws_col;
	  if      (y < DY)               small = 2;
	  else if (x < 4 * DX + DC)      small = 2;
	  else if (x < 6 * DX + 2 * DC)  small = 1;
	  else                           small = 0;
	  y -= DY;
	  x -= 4 * DX + DC;
	  if (!small)
	    x -= 2 * DX + DC;
	  y /= 2;
	  x /= 2;
	}
      
      if (small == 2)
	{
	  fprintf(stdout, "\033[H\033[2J%s\n", "Screen is too small");
	  fflush(stdout);
	  pause();
	  continue;
	}
      
      now_ = time(NULL);
      if (now_ == -1)  goto fail;
      now = localtime(&now_);
      if (now == NULL)  goto fail;
      
      digits[0] = mongo_ds[now->tm_hour / 10];
      digits[1] = mongo_ds[now->tm_hour % 10];
      digits[2] = mongo_c;
      digits[3] = mongo_ds[now->tm_min / 10];
      digits[4] = mongo_ds[now->tm_min % 10];
      digits[5] = small ? NULL : mongo_c;
      digits[6] = mongo_ds[now->tm_sec / 10];
      digits[7] = mongo_ds[now->tm_sec % 10];
      digits[8] = NULL;
      
      print_time(digits, y, x);
      
      if (read(fd, _buf, 8) < 0)
	if (errno != EINTR)
	  goto fail;
    }
  
  fprintf(stdout, "\033[?25h\n\033[?1049l");
  fflush(stdout);
  TEMP_FAILURE_RETRY(close(fd));
  return 0;
  
 fail:
  perror(*argv);
  fprintf(stdout, "\033[?25h\n\033[?1049l");
  fflush(stdout);
  TEMP_FAILURE_RETRY(close(fd));
  return 1;
  (void) argc;
}


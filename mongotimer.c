/* See LICENSE file for copyright and license details. */
#include <sys/ioctl.h>
#include <sys/timerfd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "arg.h"

#define DX 16
#define DY 12
#define DC 16
#define DM 16

#include "mongo_0.h"
#include "mongo_1.h"
#include "mongo_2.h"
#include "mongo_3.h"
#include "mongo_4.h"
#include "mongo_5.h"
#include "mongo_6.h"
#include "mongo_7.h"
#include "mongo_8.h"
#include "mongo_9.h"
#include "mongo_c.h"
#include "mongo_m.h"

static const char **mongo_ds[] = {
	mongo_0, mongo_1, mongo_2, mongo_3, mongo_4,
	mongo_5, mongo_6, mongo_7, mongo_8, mongo_9
};

static volatile sig_atomic_t caught_sigterm = 0;
static volatile sig_atomic_t caught_sigwinch = 1;
static volatile sig_atomic_t caught_sigio = 0;

char *argv0;

static void
usage(void)
{
	fprintf(stderr, "usage: %s [[-e] [[hh]:mm:]ss]\n", argv0);
	exit(1);
}

static void
sigterm(int signo)
{
	caught_sigterm = 1;
	(void) signo;
}

static void
sigwinch(int signo)
{
	caught_sigwinch = 1;
	(void) signo;
}

static void
sigio(int signo)
{
	caught_sigio = 1;
	(void) signo;
}

static void
print_time(const char ***str, size_t y, size_t x)
{
	size_t r, c;

	fprintf(stdout, "\033[%zu;1H\033[1J", y + 1);

	for (r = 0; r < DY; r++) {
		fprintf(stdout, "\033[%zu;%zuH\033[1K", y + r + 1, x + 1);
		for (c = 0; str[c]; c++)
			fprintf(stdout, "%s", str[c][r]);
		fprintf(stdout, "\033[0K");
	}

	fprintf(stdout, "\033[0J");
	fflush(stdout);
}

static int
display_stopwatch(int timerfd)
{
	uint64_t overrun, total_overrun = 0;
	struct winsize winsize;
	size_t x = 0, y = 0, width, i;
	const char **digits[23];
	uint64_t h, m, s, h_div = 1, th;
	size_t h_digits = 0;
	int small = 0, paused = 0;
	struct itimerspec old_time, zero_time;
	ssize_t r;
	char c;

	memset(&zero_time, 0, sizeof(zero_time));

	while (!caught_sigterm) {
		if (caught_sigwinch) {
			if (ioctl(STDOUT_FILENO, (unsigned long)TIOCGWINSZ, &winsize) < 0) {
				if (errno == EINTR)
					continue;
				goto fail;
			}
			caught_sigwinch = 0;
		recenter:
			y = winsize.ws_row;
			x = winsize.ws_col;
			width = 4 * DX + DC;
			if (h_digits) {
				width += DC;
				width += h_digits * DX;
			}
			if      (y < DY)    small = 1;
			else if (x < width) small = 1;
			else                small = 0;
			y -= DY;
			x -= width;
			y /= 2;
			x /= 2;
		}
		if (caught_sigio) {
			caught_sigio = 0;
			for (;;) {
				r = read(STDIN_FILENO, &c, 1);
				if (r <= 0) {
					if (!r)
						goto out;
					if (errno == EAGAIN)
						break;
					if (errno == EINTR)
						continue;
					goto fail;
				}
				if (c == ' ' || c == '\n') {
					paused ^= 1;
					if (paused) {
						if (timerfd_settime(timerfd, 0, &zero_time, &old_time))
							goto fail;
					} else {
						if (timerfd_settime(timerfd, 0, &old_time, NULL))
							goto fail;
					}
				}
			}
		}

		if (small) {
			fprintf(stdout, "\033[H\033[2J%s\n", "Screen is too small");
			fflush(stdout);
			pause();
			continue;
		}

		s = total_overrun % 60;
		m = total_overrun / 60 % 60;
		h = total_overrun / 60 / 60;
		if (h / h_div) {
			h_div *= 10;
			h_digits++;
			goto recenter;
		}

		i = 0;
		if (h_digits) {
			for (th = h; th; th /= 10)
				digits[h_digits - ++i] = mongo_ds[th % 10];
			digits[i++] = mongo_c;
		}
		digits[i++] = mongo_ds[m / 10];
		digits[i++] = mongo_ds[m % 10];
		digits[i++] = mongo_c;
		digits[i++] = mongo_ds[s / 10];
		digits[i++] = mongo_ds[s % 10];
		digits[i++] = NULL;

		if (paused) {
			fprintf(stdout, "\033[33m\n");
			print_time(digits, y, x);
			fprintf(stdout, "\033[39m\n");
			pause();
			continue;
		} else {
			print_time(digits, y, x);
		}

		if (read(timerfd, &overrun, sizeof(overrun)) < 0) {
			if (errno != EINTR)
				goto fail;
		} else {
			total_overrun += overrun;
		}
	}

out:
	return 0;

fail:
	return -1;
}

static int
display_timer(int timerfd, int64_t time, int exit_on_zero)
{
	uint64_t overrun, abstime;
	struct winsize winsize;
	size_t x = 0, y = 0, width;
	const char **digits[24];
	uint64_t h, m, s, th, h_div;
	size_t h_digits, i;
	int small = 0, paused = 0;
	struct itimerspec old_time, zero_time;
	ssize_t r;
	char c;

	memset(&zero_time, 0, sizeof(zero_time));

	while (!caught_sigterm) {
		if (caught_sigwinch) {
			if (ioctl(STDOUT_FILENO, (unsigned long)TIOCGWINSZ, &winsize) < 0) {
				if (errno == EINTR)
					continue;
				goto fail;
			}
			caught_sigwinch = 0;
		}
		if (caught_sigio) {
			caught_sigio = 0;
			for (;;) {
				r = read(STDIN_FILENO, &c, 1);
				if (r <= 0) {
					if (!r)
						goto out;
					if (errno == EAGAIN)
						break;
					if (errno == EINTR)
						continue;
					goto fail;
				}
				if (c == ' ' || c == '\n') {
					paused ^= 1;
					if (paused) {
						if (timerfd_settime(timerfd, 0, &zero_time, &old_time))
							goto fail;
					} else {
						if (timerfd_settime(timerfd, 0, &old_time, NULL))
							goto fail;
					}
				}
			}
		}

		abstime = time < 0 ? (uint64_t)-time : (uint64_t)time;
		s = abstime % 60;
		m = abstime / 60 % 60;
		h = abstime / 60 / 60;

		y = winsize.ws_row;
		x = winsize.ws_col;
		for (h_div = 1, h_digits = 0; h / h_div; h_div *= 10, h_digits += 1);
		width = 4 * DX + DC;
		if (time < 0)
			width += DM;
		if (h_digits)
			width += h_digits * DX + DC;
		if      (y < DY)    small = 1;
		else if (x < width) small = 1;
		else                small = 0;
		y -= DY;
		x -= width;
		y /= 2;
		x /= 2;

		if (small) {
			fprintf(stdout, "\033[H\033[2J%s\n", "Screen is too small");
			fflush(stdout);
			pause();
			continue;
		}

		i = 0;
		if (time < 0)
			digits[i++] = mongo_m;
		if (h_digits) {
			for (th = h; th; th /= 10)
				digits[h_digits - ++i] = mongo_ds[th % 10];
			digits[i++] = mongo_c;
		}
		digits[i++] = mongo_ds[m / 10];
		digits[i++] = mongo_ds[m % 10];
		digits[i++] = mongo_c;
		digits[i++] = mongo_ds[s / 10];
		digits[i++] = mongo_ds[s % 10];
		digits[i++] = NULL;

		if (paused) {
			fprintf(stdout, "\033[33m\n");
			print_time(digits, y, x);
			fprintf(stdout, "\033[39m\n");
			pause();
			continue;
		} else if (time >= 0) {
			print_time(digits, y, x);
		} else {
			fprintf(stdout, "\033[31m\n");
			print_time(digits, y, x);
			fprintf(stdout, "\033[39m\n");
		}

		if (read(timerfd, &overrun, sizeof(overrun)) < 0) {
			if (errno != EINTR)
				goto fail;
		} else {
			time -= (int64_t)overrun;
			if (time <= 0 && exit_on_zero)
				break;
		}
	}

out:
	if (caught_sigterm && exit_on_zero)
		return -1;

	return 0;

fail:
	return -1;
}

int
main(int argc, char *argv[])
{
	int timerfd = -1, old_flags = -1;
	int exit_on_zero = 0, old_sig = 0;
	struct itimerspec itimerspec;
	struct sigaction sigact;
	int64_t time = 0, t = 0, owner_set = 0;
	size_t colons = 0;
	char *s;
	struct f_owner_ex old_owner, new_owner;
	struct termios stty, saved_stty;

	ARGBEGIN {
	case 'e':
		exit_on_zero = 1;
		break;
	default:
		usage();
	} ARGEND;

	if (argc > 1 || (exit_on_zero && !argc))
		usage();

	fprintf(stdout, "\033[?1049h\033[?25l");

	itimerspec.it_interval.tv_sec = 1;
	itimerspec.it_interval.tv_nsec = 0;
	itimerspec.it_value.tv_sec = 1;
	itimerspec.it_value.tv_nsec = 0;
	timerfd = timerfd_create(CLOCK_BOOTTIME, 0);
	if (timerfd < 0)
		goto fail;
	if (timerfd_settime(timerfd, 0, &itimerspec, NULL))
		goto fail;

	memset(&sigact, 0, sizeof(sigact));

	sigact.sa_handler = sigterm;
	sigaction(SIGTERM, &sigact, NULL);
	sigaction(SIGQUIT, &sigact, NULL);
	sigaction(SIGINT, &sigact, NULL);

	sigact.sa_handler = sigwinch;
	sigaction(SIGWINCH, &sigact, NULL);

	sigact.sa_handler = sigio;
	sigaction(SIGIO, &sigact, NULL);
	sigaction(SIGURG, &sigact, NULL);

	if (fcntl(STDIN_FILENO, F_GETOWN_EX, &old_owner))
		goto fail;
	memset(&new_owner, 0, sizeof(new_owner));
	new_owner.type = F_OWNER_PID;
	new_owner.pid = getpid();
	if (fcntl(STDIN_FILENO, F_SETOWN_EX, &new_owner))
		goto fail;
	owner_set = 1;
	old_flags = fcntl(STDIN_FILENO, F_GETFL);
	fcntl(STDIN_FILENO, F_SETFL, old_flags | FASYNC | O_NONBLOCK);
	fcntl(STDIN_FILENO, F_GETSIG, &old_sig);
	if (old_sig)
		fcntl(STDIN_FILENO, F_SETSIG, 0);

	tcgetattr(STDIN_FILENO, &stty);
	saved_stty = stty;
        stty.c_lflag &= (tcflag_t)~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &stty);

	if (argc) {
		for (s = argv[0]; *s; s++) {
			if ('0' <= *s && *s <= '9') {
				t = t * 10 + (*s & 15);
			} else if (*s == ':' && colons++ < 2) {
				if (s == argv[0] || s[-1] == ':')
					goto fail_usage;
				time = (time + t) * 60;
				t = 0;
			} else {
				goto fail_usage;
			}
		}
		if (s == argv[0] || s[-1] == ':')
			goto fail_usage;
		time += t;
		if (display_timer(timerfd, time, exit_on_zero))
			goto fail;
	} else {
		if (display_stopwatch(timerfd))
			goto fail;
	}

	fprintf(stdout, "\033[?25h\n\033[?1049l");
	fflush(stdout);
	fcntl(STDIN_FILENO, F_SETOWN_EX, &old_owner);
	fcntl(STDIN_FILENO, F_SETFL, old_flags);
	fcntl(STDIN_FILENO, F_SETSIG, old_sig);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &saved_stty);
	close(timerfd);
	return 0;

fail:
	perror(argv0 ? argv0 : "mongotimer");
	fprintf(stdout, "\033[?25h\n\033[?1049l");
	fflush(stdout);
	if (owner_set)
		fcntl(STDIN_FILENO, F_SETOWN_EX, &old_owner);
	if (old_flags != -1)
		fcntl(STDIN_FILENO, F_SETFL, old_flags);
	if (old_sig) {
		fcntl(STDIN_FILENO, F_SETSIG, old_sig);
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &saved_stty);
	}
	if (timerfd >= 0)
		close(timerfd);
	return 1;

fail_usage:
	fprintf(stdout, "\033[?25h\n\033[?1049l");
	fflush(stdout);
	if (owner_set)
		fcntl(STDIN_FILENO, F_SETOWN_EX, &old_owner);
	if (old_flags != -1)
		fcntl(STDIN_FILENO, F_SETFL, old_flags);
	if (old_sig) {
		fcntl(STDIN_FILENO, F_SETSIG, old_sig);
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &saved_stty);
	}
	usage();
}

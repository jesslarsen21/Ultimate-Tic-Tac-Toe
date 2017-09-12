#include "signals.h"
#include <signal.h>

static struct sigaction old_sighup_handler_action;
static struct sigaction old_sigint_handler_action;
static struct sigaction old_sigquit_handler_action;
static struct sigaction old_sigill_handler_action;
static struct sigaction old_sigtrap_handler_action;
static struct sigaction old_sigabrt_handler_action;
static struct sigaction old_sigbus_handler_action;
static struct sigaction old_sigfpe_handler_action;
static struct sigaction old_sigsegv_handler_action;
static struct sigaction old_sigpipe_handler_action;
static struct sigaction old_sigterm_handler_action;

int registerSignalHandlers(void (*my_handler)(int))
{
	/* configure instructions for new signal handler */
	struct sigaction my_handler_action;
	my_handler_action.sa_handler = my_handler;
	sigemptyset(&my_handler_action.sa_mask);
	my_handler_action.sa_flags = 0;
	

	/* */
	sigaction(SIGHUP, &my_handler_action, &old_sighup_handler_action);
	sigaction(SIGINT, &my_handler_action, &old_sigint_handler_action);
	sigaction(SIGQUIT, &my_handler_action, &old_sigquit_handler_action);
	sigaction(SIGILL, &my_handler_action, &old_sigill_handler_action);
	sigaction(SIGTRAP, &my_handler_action, &old_sigtrap_handler_action);
	sigaction(SIGABRT, &my_handler_action, &old_sigabrt_handler_action);
	sigaction(SIGBUS, &my_handler_action, &old_sigbus_handler_action);
	sigaction(SIGFPE, &my_handler_action, &old_sigfpe_handler_action);
	sigaction(SIGSEGV, &my_handler_action, &old_sigsegv_handler_action);
	sigaction(SIGPIPE, &my_handler_action, &old_sigpipe_handler_action);
	sigaction(SIGTERM, &my_handler_action, &old_sigterm_handler_action);
	return 0;
}

/*  XMMS2 - X Music Multiplexer System
 *  Copyright (C) 2003-2007 XMMS2 Team
 *
 *  PLUGINS ARE NOT CONSIDERED TO BE DERIVED WORK !!!
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#include "commands.h"

#include "cli_infos.h"
#include "command_trie.h"
#include "command_utils.h"
#include "callbacks.h"


/* Setup commands */

#define CLI_SIMPLE_SETUP(setupcmd, name, cmd, needconn, usage, desc) \
	void \
	setupcmd (command_action_t *action) \
	{ command_action_fill (action, name, cmd, needconn, NULL, usage, desc); }

CLI_SIMPLE_SETUP(cli_play_setup, "play", cli_play, TRUE, NULL, "Start playback.")
CLI_SIMPLE_SETUP(cli_pause_setup, "pause", cli_pause, TRUE, NULL, "Pause playback.")
CLI_SIMPLE_SETUP(cli_seek_setup, "seek", cli_seek, TRUE, "<time|offset>",
                 "Seek to a relative or absolute position.")
CLI_SIMPLE_SETUP(cli_prev_setup, "prev", cli_prev, TRUE, NULL, "Jump to previous song.")
CLI_SIMPLE_SETUP(cli_next_setup, "next", cli_next, TRUE, NULL, "Jump to next song.")
CLI_SIMPLE_SETUP(cli_info_setup, "info", cli_info, TRUE, "<pattern>",
                 "Display all the properties for all media matching the pattern.")
CLI_SIMPLE_SETUP(cli_quit_setup, "quit", cli_quit, FALSE, NULL, "Terminate the server.")
CLI_SIMPLE_SETUP(cli_exit_setup, "exit", cli_exit, FALSE, NULL, "Exit the shell-like interface.")
CLI_SIMPLE_SETUP(cli_help_setup, "help", cli_help, FALSE, "[command]",
                 "List all commands, or help on one command.")

void
cli_stop_setup (command_action_t *action)
{
	const argument_t flags[] = {
		{ "tracks", 'n', 0, G_OPTION_ARG_INT, NULL, _("Number of tracks after which to stop playback."), "tracks" },
		{ "time",   't', 0, G_OPTION_ARG_INT, NULL, _("Duration after which to stop playback."), "time" },
		{ NULL }
	};
	command_action_fill (action, "stop", &cli_stop, TRUE, flags,
	                     "[-n <tracks> | -t <time>]",
	                     "Stop playback.");
}

void
cli_status_setup (command_action_t *action)
{
	const argument_t flags[] = {
		{ "refresh", 'r', 0, G_OPTION_ARG_INT, NULL, _("Delay between each refresh of the status. If 0, the status is only printed once (default)."), "time" },
		{ "format",  'f', 0, G_OPTION_ARG_STRING, NULL, _("Format string used to display status."), "format" },
		{ NULL }
	};
	command_action_fill (action, "status", &cli_status, TRUE, flags,
	                     "[-r <time>] [-f <format>]",
	                     "Display playback status, either continuously or once.");
}


/* Define commands */

gboolean cli_play (cli_infos_t *infos, command_context_t *ctx)
{
	xmmsc_result_t *res;
	res = xmmsc_playback_start (infos->conn);
	xmmsc_result_notifier_set (res, cb_done, infos);
	return TRUE;
}

gboolean cli_pause (cli_infos_t *infos, command_context_t *ctx)
{
	xmmsc_result_t *res;
	res = xmmsc_playback_pause (infos->conn);
	xmmsc_result_notifier_set (res, cb_done, infos);
	return TRUE;
}

gboolean cli_stop (cli_infos_t *infos, command_context_t *ctx)
{
	xmmsc_result_t *res;
	gint n;

	/* FIXME: Support those flags */
	if (command_flag_int_get (ctx, "tracks", &n) && n != 0) {
		g_printf (_("--tracks flag not supported yet!\n"));
	}
	if (command_flag_int_get (ctx, "time", &n) && n != 0) {
		g_printf (_("--time flag not supported yet!\n"));
	}

	res = xmmsc_playback_stop (infos->conn);
	xmmsc_result_notifier_set (res, cb_done, infos);

	return TRUE;
}

gboolean cli_seek (cli_infos_t *infos, command_context_t *ctx)
{
	xmmsc_result_t *res;
	command_arg_time_t t;

	if (command_arg_time_get (ctx, 0, &t)) {
		if (t.type == COMMAND_ARG_TIME_OFFSET) {
			res = xmmsc_playback_seek_ms_rel (infos->conn, t.value.offset * 1000);
		} else {
			res = xmmsc_playback_seek_ms (infos->conn, t.value.pos * 1000);
		}

		xmmsc_result_notifier_set (res, cb_done, infos);
	} else {
		g_printf (_("Error: failed to parse the time argument!\n"));
		cli_infos_loop_resume (infos);
	}

	return TRUE;
}

gboolean cli_status (cli_infos_t *infos, command_context_t *ctx)
{
	command_argument_t *arg;
	gchar *f;
	gint r;

	if (command_flag_int_get (ctx, "refresh", &r)) {
		g_printf ("refresh=%d\n", r);
	}

	if (command_flag_string_get (ctx, "format", &f)) {
		g_printf ("format='%s'\n", f);
	}

	cli_infos_loop_resume (infos);

	return TRUE;
}

gboolean cli_prev (cli_infos_t *infos, command_context_t *ctx)
{
	xmmsc_result_t *res;
	gint n;
	gint offset = 1;

	if (command_arg_int_get (ctx, 0, &n)) {
		offset = n;
	}

	res = xmmsc_playlist_set_next_rel (infos->conn, - offset);
	xmmsc_result_notifier_set (res, cb_tickle, infos);

	return TRUE;
}

gboolean cli_next (cli_infos_t *infos, command_context_t *ctx)
{
	xmmsc_result_t *res;
	gint n;
	gint offset = 1;

	if (command_arg_int_get (ctx, 0, &n)) {
		offset = n;
	}

	res = xmmsc_playlist_set_next_rel (infos->conn, offset);
	xmmsc_result_notifier_set (res, cb_tickle, infos);

	return TRUE;
}

gboolean cli_info (cli_infos_t *infos, command_context_t *ctx)
{
	gchar *pattern = NULL;
	xmmsc_coll_t *query;
	xmmsc_result_t *res;

	command_arg_longstring_get (ctx, 0, &pattern);
	if (!pattern) {
		g_printf (_("Error: you must provide a pattern!\n"));
		cli_infos_loop_resume (infos);
	} else if (!xmmsc_coll_parse (pattern, &query)) {
		g_printf (_("Error: failed to parse the pattern!\n"));
		cli_infos_loop_resume (infos);
	} else {
		res = xmmsc_coll_query_ids (infos->conn, query, NULL, 0, 0);
		xmmsc_result_notifier_set (res, cb_list_print_info, infos);
		xmmsc_coll_unref (query);
	}

	if (pattern) {
		g_free (pattern);
	}

	return TRUE;
}


gboolean cli_quit (cli_infos_t *infos, command_context_t *ctx)
{
	/* FIXME: Actually we need a connection. We just don't want to
	 * start it for nothing. */
	xmmsc_quit (infos->conn);

	cli_infos_loop_resume (infos);

	return TRUE;
}

gboolean cli_exit (cli_infos_t *infos, command_context_t *ctx)
{
	cli_infos_loop_stop (infos);

	return TRUE;
}


static void
help_short_command (gpointer elem, gpointer udata)
{
	gchar *cmdname = (gchar *)elem;
	g_printf ("   %s\n", cmdname);
}

static void
help_all_commands (cli_infos_t *infos)
{
	g_printf (_("usage: nyxmms2 <command> [args]\n\n"));
	g_printf (_("Available commands:\n"));
	g_list_foreach (infos->cmdnames, help_short_command, NULL);
	g_printf (_("\nType 'help <command>' for detailed help about a command.\n"));
}

static void
help_command (cli_infos_t *infos, gchar *cmd)
{
	command_action_t *action;
	gint i;

	action = command_trie_find (infos->commands, cmd);
	if (action) {
		g_printf (_("usage: %s"), action->name);
		if (action->usage) {
			g_printf (" %s", action->usage);
		}
		g_printf ("\n\n  %s\n\n", action->description);
		if (action->argdefs && action->argdefs[0].long_name) {
			g_printf (_("Valid options:\n"));
			for (i = 0; action->argdefs[i].long_name; ++i) {
				if (action->argdefs[i].short_name) {
					g_printf ("  -%c, ", action->argdefs[i].short_name);
				} else {
					g_printf ("      ");
				}
				g_printf ("--%s", action->argdefs[i].long_name);
				g_printf ("  %s\n", action->argdefs[i].description);
				/* FIXME: align, show arg_description, etc */
			}
		}
	} else {
		g_printf (_("Unknown command: '%s'\n"), cmd);
		g_printf (_("Type 'help' for the list of commands.\n"));
	}
}

gboolean cli_help (cli_infos_t *infos, command_context_t *ctx)
{
	gint i;

	/* No argument, display the list of commands */
	if (command_arg_count (ctx) == 0) {
		help_all_commands (infos);
	} else if (command_arg_count (ctx) == 1) {
		gchar *cmd;

		if (command_arg_string_get (ctx, 0, &cmd)) {
			help_command (infos, cmd);
		} else {
			g_printf (_("Error while parsing the argument!\n"));
		}
	} else {
		g_printf (_("To get help for a command, type 'help <command>'.\n"));
	}

	cli_infos_loop_resume (infos);

	return TRUE;
}
/*
 * main.c
 *
 * wacom-plus - A simple QOTD daemon.
 * Copyright (c) 2015-2016 Ammon Smith
 *
 * wacom-plus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * wacom-plus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with wacom-plus.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <gtk/gtk.h>

#include "core.h"
#include "win_primary.h"

int main(int argc, char *argv[])
{
	GtkApplication *app;
	int ret;

	app = gtk_application_new("org.ammongit.wacomplus",
				   G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", win_primary_activate, NULL);
	ret = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return ret;
}
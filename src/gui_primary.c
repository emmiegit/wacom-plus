/*
 * gui_primary.c
 *
 * wacom-plus - Linux GUI configuration for Wacom tablets
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

#include <stdlib.h>

#include "gui_primary.h"

static void submenu_append(const char *label, GtkWidget *menu, GCallback cbf)
{
	GtkWidget *menu_item;

	menu_item = gtk_menu_item_new_with_label(label);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
	gtk_widget_show(menu_item);
}

static void menu_append(const char *label, GtkWidget *menu, GtkWidget *menu_bar)
{
	GtkWidget *menu_item;

	menu_item = gtk_menu_item_new_with_label(label);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_item);
	gtk_widget_show(menu_item);
}

static GtkWidget *make_menu(void)
{
	GtkWidget *menu, *menu_bar;

	/* Menu Bar */
	menu_bar = gtk_menu_bar_new();
	gtk_widget_set_hexpand(menu_bar, TRUE);

	/* File */
	menu = gtk_menu_new();
	submenu_append("New...", menu, NULL);
	submenu_append("Open...", menu, NULL);
	menu_append("File", menu, menu_bar);

	return menu;
}

void gui__primary_activate(GtkApplication *app, gpointer arg)
{
	GtkWidget *win, *grid;
	GtkWidget *listbox, *ref_btn, *conf_btn;
	GtkWidget *menu;

	UNUSED(arg);

	/* Menu */
	menu = make_menu();

	/* Window */
	win = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(win), "Wacom Plus");
	gtk_window_set_default_size(GTK_WINDOW(win), 700, 850);
	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);

	/* Listbox */
	listbox = gtk_list_box_new();

	/* Refresh button */
	ref_btn = gtk_button_new_with_label("Refresh");
	g_signal_connect(ref_btn, "clicked", gui_primary_refresh, NULL);

	/* Configure button */
	conf_btn = gtk_button_new_with_label("Configure");
	g_signal_connect(conf_btn, "clicked", gui_primary_configure, NULL);

	/* Grid placement */
	grid = gtk_grid_new();
	gtk_container_set_border_width(GTK_CONTAINER(grid), 5);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
	gtk_grid_set_row_homogeneous(GTK_GRID(grid), FALSE);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_attach(GTK_GRID(grid),
			listbox,
			/* col */ 0,
			/* row */ 0,
			/* width */ 8,
			/* height */ 25);
	gtk_grid_attach(GTK_GRID(grid),
			ref_btn,
			/* col */ 0,
			/* row */ 25,
			/* width */ 4,
			/* height */ 2);
	gtk_grid_attach(GTK_GRID(grid),
			conf_btn,
			/* col */ 4,
			/* row */ 25,
			/* width */ 4,
			/* height */ 2);

	/* Finish */
	gtk_container_add(GTK_CONTAINER(win), grid);
	gtk_widget_show_all(win);
}

void gui__primary_refresh(GtkApplication *app, gpointer arg)
{
}

void gui__primary_configure(GtkApplication *app, gpointer arg)
{
}

#if 0
void gui__primary_about(GtkApplication *app, gpointer arg)
{
	GtkWidget *win;

	/* Window */
	win = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(win), "About Wacom Plus");
	gtk_window_set_default_size(GTK_WINDOW(win), 350, 500);

	/* Finish */
	gtk_widget_show_all(win);
}
#endif
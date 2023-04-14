#include "Menu.h"
#include "Import.h"
#include "Export.h"
#include "Staff.h"
#include "Info.h"

static gboolean update_time(gpointer data);

GtkWidget* create_manager_homepage_grid(Account accounts) {

	GtkWidget* grid;
	GtkWidget* sub_stack;
	GtkWidget* stackSidebar;

	//------ Create grid and stackSidebar
	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

	sub_stack = gtk_stack_new();
	stackSidebar = gtk_stack_sidebar_new();
	gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(stackSidebar), GTK_STACK(sub_stack));

	//------
	GtkWidget* time_label = gtk_label_new(NULL);
	update_time(time_label);
	g_timeout_add(1000, update_time, time_label);
	gtk_label_set_xalign(GTK_LABEL(time_label), 0.0);

	gtk_grid_attach(GTK_GRID(grid), time_label, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), stackSidebar, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), sub_stack, 1, 1, 1, 1);

	//------
	GtkWidget* Menu = create_menu_grid();
	GtkWidget* Import_menu = create_import_box();
	GtkWidget* Export_menu = create_export_box();
	GtkWidget* button3 = gtk_button_new_from_icon_name("camera-web", GTK_ICON_SIZE_MENU);
	GtkWidget* Staff = create_staff_box();
	GtkWidget* Info = create_info_box(accounts);

	gtk_stack_add_titled(GTK_STACK(sub_stack), Menu, "menu", "Menu");
	gtk_stack_add_titled(GTK_STACK(sub_stack), Import_menu, "import", "Import");
	gtk_stack_add_titled(GTK_STACK(sub_stack), Export_menu, "export", "Export");
	gtk_stack_add_titled(GTK_STACK(sub_stack), button3, "revenue", "Revenue");
	gtk_stack_add_titled(GTK_STACK(sub_stack), Staff, "staff", "Staff");
	gtk_stack_add_titled(GTK_STACK(sub_stack), Info, "info", "Info");

	return grid;
}

GtkWidget* create_staff_homepage_grid(Account accounts) {

	GtkWidget* grid;
	GtkWidget* sub_stack;
	GtkWidget* stackSidebar;

	//------ Create grid and stackSidebar
	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

	sub_stack = gtk_stack_new();
	stackSidebar = gtk_stack_sidebar_new();
	gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(stackSidebar), GTK_STACK(sub_stack));

	//------
	GtkWidget* time_label = gtk_label_new(NULL);
	update_time(time_label);
	g_timeout_add(1000, update_time, time_label);
	gtk_label_set_xalign(GTK_LABEL(time_label), 0.0);

	gtk_grid_attach(GTK_GRID(grid), time_label, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), stackSidebar, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), sub_stack, 1, 1, 1, 1);

	//------
	GtkWidget* Menu = create_menu_grid();
	GtkWidget* Import_menu = create_import_box();
	GtkWidget* Export_menu = create_export_box();
	GtkWidget* Info = create_info_box(accounts);

	gtk_stack_add_titled(GTK_STACK(sub_stack), Menu, "menu", "Menu");
	gtk_stack_add_titled(GTK_STACK(sub_stack), Import_menu, "import", "Import");
	gtk_stack_add_titled(GTK_STACK(sub_stack), Export_menu, "export", "Export");
	gtk_stack_add_titled(GTK_STACK(sub_stack), Info, "info", "Info");

	return grid;
}

static gboolean update_time(gpointer data) {
	GtkLabel* label = (GtkLabel*)data;
	time_t now = time(NULL);
	struct tm* now_tm = localtime(&now);
	char buffer[64];
	strftime(buffer, sizeof(buffer), "%d/%m/%Y %I:%M:%S %p", now_tm);
	gtk_label_set_text(label, buffer);
	return TRUE;
}

static void login_clicked_succeed(GtkButton* button, gpointer data) {

	GtkWidget* id_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "id_entry"));
	GtkWidget* username_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "username_entry"));
	GtkWidget* password_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "password_entry"));

	const gchar* id = gtk_entry_get_text(GTK_ENTRY(id_entry));
	const gchar* username = gtk_entry_get_text(GTK_ENTRY(username_entry));
	const gchar* password = gtk_entry_get_text(GTK_ENTRY(password_entry));

	Account accounts[MAX_USER];
	gint count = 0;
	gboolean found = FALSE;
	loadAccountDataFromFile(accounts, &count);

	for (gint i = 0; i < count; i++) {
		if (g_strcmp0(id, accounts[i].id) == 0 && g_strcmp0(username, accounts[i].username) == 0 && g_strcmp0(password, accounts[i].password) == 0) {
			get_account = accounts[i];
			found = TRUE;
			break;
		}
	}

	GtkWidget* old_window = GTK_WIDGET(data);
	GtkWidget* new_window;
	GtkWidget* manager_homepage_grid;
	GtkWidget* staff_homepage_grid;

	new_window = createWindow(-1, -1, "Convenience Store Management");
	g_signal_connect(new_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	if (found) {
		if (g_strcmp0(get_account.position, "Manager") == 0) {

			manager_homepage_grid = create_manager_homepage_grid(get_account);
			show_new_window(new_window, old_window, manager_homepage_grid);
		}
		else {
			staff_homepage_grid = create_staff_homepage_grid(get_account);
			show_new_window(new_window, old_window, staff_homepage_grid);
		}
	}
	else {
		GtkWidget* dialog = gtk_dialog_new_with_buttons("Login Failure",
			NULL,
			GTK_DIALOG_MODAL,
			"_Cancel",
			GTK_RESPONSE_CANCEL,
			NULL);
		gtk_container_set_border_width(GTK_CONTAINER(dialog), 20);
		gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
		gtk_window_set_default_size(GTK_WINDOW(dialog), -1, -1);

		//------
		GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
		GtkWidget* label = gtk_label_new("Authentication failed. Please try again.");
		g_object_set(label, "margin", 10, NULL);

		gtk_container_add(GTK_CONTAINER(content_area), label);

		gtk_widget_show_all(dialog);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		
	}

}
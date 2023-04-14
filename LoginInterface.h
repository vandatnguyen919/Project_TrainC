#include "AccountData.h"

static void main_clicked(GtkButton* button, GtkStack* stack);
static void login_clicked(GtkButton* button, GtkStack* stack);
static void login_clicked_succeed(GtkButton* button, gpointer data);

Account get_account;

GtkWidget* create_main_box(GtkWidget* stack, const gchar* name) {

	GtkWidget* box;
	GtkWidget* label;
	GtkWidget* login_button;
	GtkWidget* quit_button;

	//------
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	label = gtk_label_new(name);
	login_button = gtk_button_new_with_label("Login");
	quit_button = gtk_button_new_with_label("Quit");

	PangoAttrList* attrlist = pango_attr_list_new();
	PangoAttribute* size_attr = pango_attr_size_new(20 * PANGO_SCALE);
	pango_attr_list_insert(attrlist, size_attr);
	gtk_label_set_attributes(GTK_LABEL(label), attrlist);
	pango_attr_list_unref(attrlist);
	//------
	gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
	gtk_box_pack_start(GTK_BOX(box), login_button, 0, 0, 0);
	gtk_box_pack_start(GTK_BOX(box), quit_button, 0, 0, 0);

	//------
	g_signal_connect(login_button, "clicked", G_CALLBACK(login_clicked), stack);
	g_signal_connect(quit_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);

	return box;
}


GtkWidget* create_login_grid(GtkWidget* stack, GtkWidget* window) {

	GtkWidget* grid;
	GtkWidget* id_label;
	GtkWidget* id_entry;
	GtkWidget* username_label;
	GtkWidget* username_entry;
	GtkWidget* password_label;
	GtkWidget* password_entry;
	GtkWidget* login_button;
	GtkWidget* main_button;

	//------
	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

	//------
	id_label = gtk_label_new("ID:");
	username_label = gtk_label_new("Username:");
	password_label = gtk_label_new("Password:");

	//------
	id_entry = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(id_entry), 8);
	username_entry = gtk_entry_new();
	password_entry = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

	//------
	login_button = gtk_button_new_with_label("Login");
	main_button = gtk_button_new_with_label("Go to Main");

	//------
	gtk_grid_attach(GTK_GRID(grid), id_label,		0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), username_label, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), password_label, 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), id_entry,		1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), username_entry, 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), main_button,	0, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), login_button,	1, 3, 1, 1);

	//------
	g_object_set_data(G_OBJECT(login_button), "id_entry", id_entry);
	g_object_set_data(G_OBJECT(login_button), "username_entry", username_entry);
	g_object_set_data(G_OBJECT(login_button), "password_entry", password_entry);

	g_signal_connect(main_button, "clicked", G_CALLBACK(main_clicked), stack);
	g_signal_connect(login_button, "clicked", G_CALLBACK(login_clicked_succeed), window);

	//------
	return grid;
}

static void main_clicked(GtkButton* button, GtkStack* stack) {

	g_return_if_fail(GTK_IS_BUTTON(button));
	g_return_if_fail(GTK_IS_STACK(stack));

	gtk_stack_set_visible_child_full(GTK_STACK(stack), "Main", GTK_STACK_TRANSITION_TYPE_SLIDE_DOWN);
}

static void login_clicked(GtkButton* button, GtkStack* stack) {

	g_return_if_fail(GTK_IS_BUTTON(button));
	g_return_if_fail(GTK_IS_STACK(stack));

	gtk_stack_set_visible_child_full(GTK_STACK(stack), "Login", GTK_STACK_TRANSITION_TYPE_SLIDE_UP);
}
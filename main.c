#include <gtk/gtk.h>
#include "createWindow.h"
#include "LoginInterface.h"
#include "Homepage.h"

//gcc `pkg-config --cflags gtk+-3.0` -o app main.c `pkg-config --libs gtk+-3.0`

int main(int argc, char** argv) {

	GtkWidget* window;
	GtkWidget* stack_box;
	GtkWidget* stack;

	GtkWidget* main_box;
	GtkWidget* login_grid;

	gtk_init(&argc, &argv);

	window = createWindow(-1, 500, "Convenience Store Management");
	
	//------
	stack_box = create_stack_box(&stack);
	gtk_container_add(GTK_CONTAINER(window), stack_box);

	//------
	main_box   = create_main_box(stack, "Convenience Store Management");
	login_grid = create_login_grid(stack, window);

	//------
	gtk_stack_add_named(GTK_STACK(stack), main_box, "Main");
	gtk_stack_add_named(GTK_STACK(stack), login_grid, "Login");

	gtk_stack_set_transition_duration(GTK_STACK(stack), 1000);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}
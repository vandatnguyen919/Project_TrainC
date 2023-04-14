
GtkWidget* createWindow(const gint width, const gint height, const gchar* name) {

	GtkWidget* Window;
	Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	//------
	gtk_window_set_title(GTK_WINDOW(Window), name);
	gtk_window_set_default_size(GTK_WINDOW(Window), width, height);
	gtk_window_set_position(GTK_WINDOW(Window), GTK_WIN_POS_CENTER);
	//gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(Window), 50);

	//------
	return Window;
}

GtkWidget* create_stack_box(GtkWidget** stack) {

	GtkWidget* box;

	//------ Create the Box
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

	//------ Create a Stack
	*stack = gtk_stack_new();
	gtk_widget_set_halign(*stack, GTK_ALIGN_CENTER);
	gtk_box_set_center_widget(GTK_BOX(box), *stack);

	//------
	return box;
}

static void show_new_window(GtkWidget* new_window, GtkWidget* old_window, GtkWidget* homepage) {

	GtkWidget* stack;
	GtkWidget* stack_box;
	stack_box = create_stack_box(&stack);
	gtk_stack_add_named(GTK_STACK(stack), homepage, "Homepage");
	gtk_container_add(GTK_CONTAINER(new_window), stack_box);
	gtk_widget_destroy(old_window);
	gtk_widget_show_all(new_window);
}
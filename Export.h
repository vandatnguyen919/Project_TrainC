
GtkWidget* create_export_history_table_box();

enum {
	E_COLUMN1, E_COLUMN2, E_COLUMN3, E_COLUMN4, E_COLUMN5, E_COLUMN6, E_COLUMN7,
	E_N_COLUMNS
};

GtkWidget* create_export_box() {

	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

	GtkWidget* stack = gtk_stack_new();
	GtkWidget* stackSwitcher = gtk_stack_switcher_new();
	gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(stackSwitcher), GTK_STACK(stack));

	GtkWidget* export_history_table_box = create_export_history_table_box();

	gtk_grid_attach(GTK_GRID(grid), stackSwitcher, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), stack, 0, 1, 1, 1);

	gtk_stack_add_titled(GTK_STACK(stack), export_history_table_box, "history table", "History");

	gtk_box_pack_start(GTK_BOX(box), grid, 1, 1, 0);
	
	return box;
}

GtkWidget* create_export_history_table_box() {

	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	GtkWidget* scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
		GTK_POLICY_AUTOMATIC,
		GTK_POLICY_AUTOMATIC);
	GtkListStore* model;
	GtkWidget* view;
	GtkTreeViewColumn* column;
	GtkCellRenderer* renderer;

	model = gtk_list_store_new(E_N_COLUMNS,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING
	);

	gchar no[MAX_GOOD];

	Export exports[1024]; gint count = 0;
	loadExportDataFromFile(exports, &count);
	for (gint i = 0; i < count; i++) {
		sprintf(no, "%d", i + 1);
		gtk_list_store_insert_with_values(model, NULL, -1,
			E_COLUMN1, no,
			E_COLUMN2, exports[i].id,
			E_COLUMN3, exports[i].quantity,
			E_COLUMN4, exports[i].price,
			E_COLUMN5, exports[i].totalPrice,
			E_COLUMN6, exports[i].totalQuantity,
			E_COLUMN7, exports[i].time,
			-1);
	}

	//------
	view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(model));
	g_object_unref(model);

	renderer = gtk_cell_renderer_text_new();

	column = gtk_tree_view_column_new_with_attributes("No",
		renderer,
		"text", E_COLUMN1,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("ID",
		renderer,
		"text", E_COLUMN2,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Export Quantity",
		renderer,
		"text", E_COLUMN3,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Price",
		renderer,
		"text", E_COLUMN4,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Total Price",
		renderer,
		"text", E_COLUMN5,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Total Quantity",
		renderer,
		"text", E_COLUMN6,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Export Time",
		renderer,
		"text", E_COLUMN7,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	//------
	GtkWidget* label = gtk_label_new("Export History:");
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);

	gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
	gtk_container_add(GTK_CONTAINER(scrolled_window), view);
	gtk_box_pack_start(GTK_BOX(box), scrolled_window, 1, 1, 0);
	gtk_widget_set_size_request(box, 600, 365);

	return box;
}
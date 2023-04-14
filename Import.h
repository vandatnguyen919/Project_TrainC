
GtkWidget* create_product_table_box();
GtkWidget* create_import_history_table_box();
GtkWidget* create_add_product_box();
GtkWidget* create_update_product_box();

static void add_image_clicked(GtkButton* button, gpointer data);
static void add_product_clicked(GtkButton* button, gpointer data);
static void search_product_clicked(GtkButton* button, gpointer data);

gchar imagePATH[1024] = "\0";

enum {
	P_COLUMN1, P_COLUMN2, P_COLUMN3, P_COLUMN4, P_COLUMN5, P_COLUMN6, P_COLUMN7,
	P_N_COLUMNS
};

GtkWidget* create_import_box() {

	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

	GtkWidget* grid2 = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid2), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid2), 10);

	GtkWidget* stack = gtk_stack_new();
	GtkWidget* stackSwitcher = gtk_stack_switcher_new();
	gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(stackSwitcher), GTK_STACK(stack));

	GtkWidget* stack2 = gtk_stack_new();
	GtkWidget* stackSwitcher2 = gtk_stack_switcher_new();
	gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(stackSwitcher2), GTK_STACK(stack2));

	//------
	GtkWidget* add_product_box = create_add_product_box();
	GtkWidget* update_product_box = create_update_product_box();
	GtkWidget* product_table_box = create_product_table_box();
	GtkWidget* import_history_table_box = create_import_history_table_box();

	//------ Product stackSwitcher
	gtk_grid_attach(GTK_GRID(grid), stackSwitcher, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), stack, 0, 1, 1, 1);

	gtk_stack_add_titled(GTK_STACK(stack), add_product_box, "add product", "Add");
	gtk_stack_add_titled(GTK_STACK(stack), update_product_box, "update product", "Update");

	//------ Table stackSwitcher
	gtk_grid_attach(GTK_GRID(grid2), stackSwitcher2, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid2), stack2, 0, 1, 1, 1);

	gtk_stack_add_titled(GTK_STACK(stack2), product_table_box, "product table", "Products");
	gtk_stack_add_titled(GTK_STACK(stack2), import_history_table_box, "history table", "History");

	//------
	gtk_box_pack_start(GTK_BOX(box), grid, 1, 1, 0);
	gtk_box_pack_start(GTK_BOX(box), grid2, 1, 1, 0);

	return box;
}

GtkWidget* create_product_table_box() {

	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	GtkWidget* scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
		GTK_POLICY_AUTOMATIC,
		GTK_POLICY_AUTOMATIC);
	GtkListStore* model;
	GtkWidget* view;
	GtkTreeViewColumn* column;
	GtkCellRenderer* renderer;

	model = gtk_list_store_new(P_N_COLUMNS,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING
	);

	Product products[100]; gint count = 0;
	loadProductDataFromFile(products, &count);
	for (gint i = 0; i < count; i++) {
		gtk_list_store_insert_with_values(model, NULL, -1,
			P_COLUMN1, products[i].no,
			P_COLUMN2, products[i].id,
			P_COLUMN3, products[i].type,
			P_COLUMN4, products[i].name,
			P_COLUMN5, products[i].quantity,
			P_COLUMN6, products[i].ImPrice,
			P_COLUMN7, products[i].ExPrice,
			-1);
	}

	//------
	view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(model));
	g_object_unref(model);

	renderer = gtk_cell_renderer_text_new();

	column = gtk_tree_view_column_new_with_attributes("No",
		renderer,
		"text", P_COLUMN1,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("ID",
		renderer,
		"text", P_COLUMN2,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Type",
		renderer,
		"text", P_COLUMN3,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Name",
		renderer,
		"text", P_COLUMN4,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Quantity",
		renderer,
		"text", P_COLUMN5,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Import Price/p",
		renderer,
		"text", P_COLUMN6,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Export Price/p",
		renderer,
		"text", P_COLUMN7,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	//------
	gchar LABEL[31] = "Your Product(s): ";
	gchar NUM_OF_PRODUCT[16];
	sprintf(NUM_OF_PRODUCT, "%d", count);
	strcat(LABEL, NUM_OF_PRODUCT);

	GtkWidget* label = gtk_label_new(LABEL);
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);

	gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
	gtk_container_add(GTK_CONTAINER(scrolled_window), view);
	gtk_box_pack_start(GTK_BOX(box), scrolled_window, 1, 1, 0);
	gtk_widget_set_size_request(box, 600, 369);

	return box;
}

GtkWidget* create_import_history_table_box() {

	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	GtkWidget* scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
		GTK_POLICY_AUTOMATIC,
		GTK_POLICY_AUTOMATIC);
	GtkListStore* model;
	GtkWidget* view;
	GtkTreeViewColumn* column;
	GtkCellRenderer* renderer;

	model = gtk_list_store_new(P_N_COLUMNS,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING
	);

	gchar no[MAX_GOOD];

	Import imports[1024]; gint count = 0;
	loadImportDataFromFile(imports, &count);
	for (gint i = 0; i < count; i++) {
		sprintf(no, "%d", i + 1);
		gtk_list_store_insert_with_values(model, NULL, -1,
			P_COLUMN1, no,
			P_COLUMN2, imports[i].id,
			P_COLUMN3, imports[i].quantity,
			P_COLUMN4, imports[i].price,
			P_COLUMN5, imports[i].totalPrice,
			P_COLUMN6, imports[i].totalQuantity,
			P_COLUMN7, imports[i].time,
			-1);
	}

	//------
	view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(model));
	g_object_unref(model);

	renderer = gtk_cell_renderer_text_new();

	column = gtk_tree_view_column_new_with_attributes("No",
		renderer,
		"text", P_COLUMN1,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("ID",
		renderer,
		"text", P_COLUMN2,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Import Quantity",
		renderer,
		"text", P_COLUMN3,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Price",
		renderer,
		"text", P_COLUMN4,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Total Price",
		renderer,
		"text", P_COLUMN5,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Total Quantity",
		renderer,
		"text", P_COLUMN6,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Import Time",
		renderer,
		"text", P_COLUMN7,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	//------
	GtkWidget* label = gtk_label_new("Import History:");
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);

	gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
	gtk_container_add(GTK_CONTAINER(scrolled_window), view);
	gtk_box_pack_start(GTK_BOX(box), scrolled_window, 1, 1, 0);
	return box;
}

GtkWidget* create_add_product_box() {

	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

	GtkWidget* label = gtk_label_new("New Product:");
	gtk_widget_set_size_request(label, 100, -1);
	GtkWidget* id_label = gtk_label_new("ID:");
	GtkWidget* type_label = gtk_label_new("Type:");
	GtkWidget* name_label = gtk_label_new("Name:");
	GtkWidget* qty_label = gtk_label_new("Quantity:");
	GtkWidget* iprice_label = gtk_label_new("Import Price/p:");
	GtkWidget* eprice_label = gtk_label_new("Export Price/p:");
	GtkWidget* image_label = gtk_label_new("Image source:");

	GtkWidget* id_entry = gtk_entry_new();
	GtkWidget* type_entry = gtk_entry_new();
	GtkWidget* name_entry = gtk_entry_new();
	GtkWidget* qty_entry = gtk_entry_new();
	GtkWidget* iprice_entry = gtk_entry_new();
	GtkWidget* eprice_entry = gtk_entry_new();
	GtkWidget* image_button = gtk_button_new_with_label("Select an image from file");
	GtkWidget* add_product_button = gtk_button_new_with_label("Add");

	gtk_entry_set_max_length(GTK_ENTRY(id_entry), 8);

	gtk_label_set_xalign(GTK_LABEL(label), 0);
	gtk_label_set_xalign(GTK_LABEL(id_label), 0);
	gtk_label_set_xalign(GTK_LABEL(type_label), 0);
	gtk_label_set_xalign(GTK_LABEL(name_label), 0);
	gtk_label_set_xalign(GTK_LABEL(qty_label), 0);
	gtk_label_set_xalign(GTK_LABEL(iprice_label), 0);
	gtk_label_set_xalign(GTK_LABEL(eprice_label), 0);
	gtk_label_set_xalign(GTK_LABEL(image_label), 0);

	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), id_label, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), type_label, 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), name_label, 0, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), qty_label, 0, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), iprice_label, 0, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), eprice_label, 0, 6, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), image_label, 0, 7, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), type_entry, 1, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), qty_entry, 1, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), iprice_entry, 1, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), eprice_entry, 1, 6, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), image_button, 1, 7, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), add_product_button, 1, 8, 1, 1);

	gtk_box_pack_start(GTK_BOX(box), grid, 0, 0, 0);

	g_object_set_data(G_OBJECT(add_product_button), "id_entry", id_entry);
	g_object_set_data(G_OBJECT(add_product_button), "type_entry", type_entry);
	g_object_set_data(G_OBJECT(add_product_button), "name_entry", name_entry);
	g_object_set_data(G_OBJECT(add_product_button), "qty_entry", qty_entry);
	g_object_set_data(G_OBJECT(add_product_button), "iprice_entry", iprice_entry);
	g_object_set_data(G_OBJECT(add_product_button), "eprice_entry", eprice_entry);

	g_signal_connect_swapped(qty_entry, "key_press_event", G_CALLBACK(zero_to_nine_keys_callback), qty_entry);
	g_signal_connect_swapped(iprice_entry, "key_press_event", G_CALLBACK(zero_to_nine_keys_callback), iprice_entry);
	g_signal_connect_swapped(eprice_entry, "key_press_event", G_CALLBACK(zero_to_nine_keys_callback), eprice_entry);

	g_signal_connect(image_button, "clicked", G_CALLBACK(add_image_clicked), NULL);
	g_signal_connect(add_product_button, "clicked", G_CALLBACK(add_product_clicked), NULL);

	return box;
}

GtkWidget* create_update_product_box() {

	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

	GtkWidget* label = gtk_label_new("Product:");
	gtk_widget_set_size_request(label, 100, -1);
	GtkWidget* id_label = gtk_label_new("ID:");

	GtkWidget* id_entry = gtk_entry_new();
	GtkWidget* search_product_button = gtk_button_new_with_label("Search");
	gtk_entry_set_max_length(GTK_ENTRY(id_entry), 8);

	gtk_label_set_xalign(GTK_LABEL(label), 0);
	gtk_label_set_xalign(GTK_LABEL(id_label), 0);

	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), id_label, 0, 1, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), search_product_button, 1, 2, 1, 1);

	gtk_box_pack_start(GTK_BOX(box), grid, 1, 1, 0);

	//------
	g_object_set_data(G_OBJECT(search_product_button), "id_entry", id_entry);

	g_signal_connect(search_product_button, "clicked", G_CALLBACK(search_product_clicked), NULL);

	return box;
}

static void add_image_clicked(GtkButton* button, gpointer data) {

	GtkWidget* dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new("Open File",
		NULL,
		action,
		("_Cancel"),
		GTK_RESPONSE_CANCEL,
		("_Open"),
		GTK_RESPONSE_ACCEPT,
		NULL);

	res = gtk_dialog_run(GTK_DIALOG(dialog));
	if (res == GTK_RESPONSE_ACCEPT) {
		char* filename;
		GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
		filename = gtk_file_chooser_get_filename(chooser);
		// do something with the selected file's location
		g_print("Selected file: %s\n", filename);
		strcpy(imagePATH, filename);
		g_free(filename);
	}

	gtk_widget_destroy(dialog);
}

static void add_product_clicked(GtkButton* button, gpointer data) {

	GtkWidget* id_entry =	 GTK_WIDGET(g_object_get_data(G_OBJECT(button), "id_entry"));
	GtkWidget* type_entry =  GTK_WIDGET(g_object_get_data(G_OBJECT(button), "type_entry"));
	GtkWidget* name_entry =  GTK_WIDGET(g_object_get_data(G_OBJECT(button), "name_entry"));
	GtkWidget* qty_entry =   GTK_WIDGET(g_object_get_data(G_OBJECT(button), "qty_entry"));
	GtkWidget* iprice_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "iprice_entry"));
	GtkWidget* eprice_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "eprice_entry"));

	const gchar* id =    gtk_entry_get_text(GTK_ENTRY(id_entry));
	const gchar* type =  gtk_entry_get_text(GTK_ENTRY(type_entry));
	const gchar* name =  gtk_entry_get_text(GTK_ENTRY(name_entry));
	const gchar* qty =   gtk_entry_get_text(GTK_ENTRY(qty_entry));
	const gchar* ImPrice = gtk_entry_get_text(GTK_ENTRY(iprice_entry));
	const gchar* ExPrice = gtk_entry_get_text(GTK_ENTRY(eprice_entry));
	const gchar* image = imagePATH;

	GtkWidget* dialog = gtk_dialog_new_with_buttons("Add a product",
		NULL,
		GTK_DIALOG_MODAL,
		"_Cancel",
		GTK_RESPONSE_CANCEL,
		NULL);
	GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	gtk_container_set_border_width(GTK_CONTAINER(dialog), 20);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_default_size(GTK_WINDOW(dialog), -1, -1);

	GtkWidget* label = gtk_label_new("A new product is added.");
	GtkWidget* label2 = gtk_label_new("This ID has been used.");
	GtkWidget* label3 = gtk_label_new("Please! Fill all of the information.");

	g_object_set(label, "margin", 10, NULL);
	g_object_set(label2, "margin", 10, NULL);
	g_object_set(label3, "margin", 10, NULL);

	//------
	Product products[MAX_GOOD];
	gint count = 0;
	loadProductDataFromFile(products, &count);

	gint response = 1;
	for (gint i = 0; i < count; i++) {
		if (g_strcmp0(products[i].id, id) == 0) {
			response = 2;
			goto RESPONSE;
		}
	}
	if (id[0] == '\0' || type[0] == '\0' || name[0] == '\0' || qty[0] == '\0' || ImPrice[0] == '\0' || ExPrice[0] == '\0' || image[0] == '\0') {
	
		response = 3;
	}
	RESPONSE:
	if (response == 2) {
		gtk_box_pack_start(GTK_BOX(content_area), label2, 0, 0, 0);
		gtk_widget_show_all(dialog);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return;
	}
	else if (response == 3) {
		gtk_box_pack_start(GTK_BOX(content_area), label3, 0, 0, 0);
		gtk_widget_show_all(dialog);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return;
	}

	//------ Add a product

	gchar no[MAX_GOOD];
	sprintf(no, "%d", count + 1);

	strcpy(products[count].no, no);
	strcpy(products[count].id, id);
	strcpy(products[count].type, type);
	strcpy(products[count].name, name);
	strcpy(products[count].quantity, qty);
	strcpy(products[count].ImPrice, ImPrice);
	strcpy(products[count].ExPrice, ExPrice);
	strcpy(products[count].productImage, image);

	storeProductDataToImportFile(products[count], atoi(products[count].quantity));
	count++;
	storeProductDataToFile(products, &count);
	
	//------
	gtk_box_pack_start(GTK_BOX(content_area), label, 0, 0, 0);
	gtk_widget_show_all(dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

static void search_product_clicked(GtkButton* button, gpointer data) {

	GtkWidget* ID_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "id_entry"));

	const gchar* id = gtk_entry_get_text(GTK_ENTRY(ID_entry));

	GtkWidget* dialog = gtk_dialog_new_with_buttons("Update a product",
		NULL,
		GTK_DIALOG_MODAL,
		"Update",
		GTK_RESPONSE_OK,
		NULL);
	gtk_container_set_border_width(GTK_CONTAINER(dialog), 20);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_default_size(GTK_WINDOW(dialog), -1, -1);

	GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
	g_object_set(grid, "margin-bottom", 10, NULL);

	gtk_box_pack_start(GTK_BOX(content_area), grid, 0, 0, 0);
	
	//------
	Product products[MAX_GOOD]; gint count = 0;
	Product get_product; gboolean found = FALSE;
	loadProductDataFromFile(products, &count);
	for (gint i = 0; i < count; i++) {
		if (g_strcmp0(products[i].id, id) == 0) {
			get_product = products[i];
			found = TRUE;
			break;
		}
	}
	if (!found) return;

	//------
	GtkWidget* id_label = gtk_label_new("ID:");
	GtkWidget* name_label = gtk_label_new("Name:");
	GtkWidget* type_label = gtk_label_new("Type:");
	GtkWidget* qty_label = gtk_label_new("Quantity:");
	GtkWidget* imQty_label = gtk_label_new("Import Quantity:");
	GtkWidget* ImPrice_label = gtk_label_new("Import Price / p:");
	GtkWidget* ExPrice_label = gtk_label_new("Export Price / p:");
	GtkWidget* image_label = gtk_label_new("Image source:");

	GtkEntryBuffer* id_buffer = gtk_entry_buffer_new(get_product.id, -1);
	GtkEntryBuffer* name_buffer = gtk_entry_buffer_new(get_product.name, -1);
	GtkEntryBuffer* type_buffer = gtk_entry_buffer_new(get_product.type, -1);
	GtkEntryBuffer* qty_buffer = gtk_entry_buffer_new(get_product.quantity, -1);
	GtkEntryBuffer* imQty_buffer = gtk_entry_buffer_new("0", -1);
	GtkEntryBuffer* ImPrice_buffer = gtk_entry_buffer_new(get_product.ImPrice, -1);
	GtkEntryBuffer* ExPrice_buffer = gtk_entry_buffer_new(get_product.ExPrice, -1);
	GtkEntryBuffer* image_buffer = gtk_entry_buffer_new(get_product.productImage, -1);
	
	GtkWidget* id_entry = gtk_entry_new_with_buffer(id_buffer);
	GtkWidget* name_entry = gtk_entry_new_with_buffer(name_buffer);
	GtkWidget* type_entry = gtk_entry_new_with_buffer(type_buffer);
	GtkWidget* qty_entry = gtk_entry_new_with_buffer(qty_buffer);
	GtkWidget* imQty_entry = gtk_entry_new_with_buffer(imQty_buffer);
	GtkWidget* ImPrice_entry = gtk_entry_new_with_buffer(ImPrice_buffer);
	GtkWidget* ExPrice_entry = gtk_entry_new_with_buffer(ExPrice_buffer);
	GtkWidget* image_entry = gtk_entry_new_with_buffer(image_buffer);

	gtk_label_set_xalign(GTK_LABEL(id_label), 0);
	gtk_label_set_xalign(GTK_LABEL(name_label), 0);
	gtk_label_set_xalign(GTK_LABEL(type_label), 0);
	gtk_label_set_xalign(GTK_LABEL(qty_label), 0);
	gtk_label_set_xalign(GTK_LABEL(imQty_label), 0);
	gtk_label_set_xalign(GTK_LABEL(ImPrice_label), 0);
	gtk_label_set_xalign(GTK_LABEL(ExPrice_label), 0);
	gtk_label_set_xalign(GTK_LABEL(image_label), 0);

	gtk_grid_attach(GTK_GRID(grid), id_label, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), name_label, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), type_label, 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), qty_label, 0, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), imQty_label, 0, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), ImPrice_label, 0, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), ExPrice_label, 0, 6, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), image_label, 0, 7, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), type_entry, 1, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), qty_entry, 1, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), imQty_entry, 1, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), ImPrice_entry, 1, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), ExPrice_entry, 1, 6, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), image_entry, 1, 7, 1, 1);

	gtk_widget_set_sensitive(GTK_WIDGET(id_entry), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(qty_entry), FALSE);

	g_signal_connect_swapped(imQty_entry, "key_press_event", G_CALLBACK(zero_to_nine_keys_callback), imQty_entry);
	g_signal_connect_swapped(ImPrice_entry, "key_press_event", G_CALLBACK(zero_to_nine_keys_callback), ImPrice_entry);
	g_signal_connect_swapped(ExPrice_entry, "key_press_event", G_CALLBACK(zero_to_nine_keys_callback), ExPrice_entry);

	const gchar* name;
	const gchar* type;
	const gchar* imQuantity;
	const gchar* ImPrice;
	const gchar* ExPrice;
	const gchar* image;

	gtk_widget_show_all(dialog);

	gint response = gtk_dialog_run(GTK_DIALOG(dialog));
	switch (response) {
	case GTK_RESPONSE_OK:
		name = gtk_entry_get_text(GTK_ENTRY(name_entry));
		type = gtk_entry_get_text(GTK_ENTRY(type_entry));
		imQuantity = gtk_entry_get_text(GTK_ENTRY(imQty_entry));
		ImPrice = gtk_entry_get_text(GTK_ENTRY(ImPrice_entry));
		ExPrice = gtk_entry_get_text(GTK_ENTRY(ExPrice_entry));
		image = gtk_entry_get_text(GTK_ENTRY(image_entry));

		gint index;
		if (g_strcmp0(get_product.name, name) != 0 || g_strcmp0(get_product.type, type) != 0 || (g_strcmp0("0", imQuantity) != 0 && atoi(imQuantity) > 0) ||
			g_strcmp0(get_product.ImPrice, ImPrice) != 0 || g_strcmp0(get_product.ExPrice, ExPrice) != 0 || g_strcmp0(get_product.productImage, image) != 0) {
			
			for (gint i = 0; i < count; i++) {
				if (g_strcmp0(products[i].id, id) == 0) {

					gchar new_quantity[21];
					gint quantity = atoi(products[i].quantity) + atoi(imQuantity);
					sprintf(new_quantity, "%d", quantity);

					strcpy(products[i].name, name);
					strcpy(products[i].type, type);
					strcpy(products[i].quantity, new_quantity);
					strcpy(products[i].ImPrice, ImPrice);
					strcpy(products[i].ExPrice, ExPrice);
					strcpy(products[i].productImage, image);
					index = i;
					break;
				}
			}
			storeProductDataToFile(products, &count);

			GtkWidget* label = gtk_label_new("Product updated!");
			gtk_label_set_xalign(GTK_LABEL(label), 1.0);
			g_object_set(label, "margin-bottom", 10, NULL);
			gtk_box_pack_start(GTK_BOX(content_area), label, 0, 0, 0);

			gtk_widget_show(label);
		}
		else {
			GtkWidget* label = gtk_label_new("Nothing updated!");
			gtk_label_set_xalign(GTK_LABEL(label), 1.0);
			g_object_set(label, "margin-bottom", 10, NULL);
			gtk_box_pack_start(GTK_BOX(content_area), label, 0, 0, 0);

			gtk_widget_show(label);
		}
		if (g_strcmp0("0", imQuantity) != 0 && atoi(imQuantity) > 0) {
			storeProductDataToImportFile(products[index], atoi(imQuantity));
		}
		break;
		
	default:
		gtk_widget_destroy(dialog);
		break;
	}
}
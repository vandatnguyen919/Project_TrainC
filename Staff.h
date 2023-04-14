
GtkWidget* create_add_account_box();
GtkWidget* create_delete_account_box();
GtkWidget* create_account_table_box();

static void add_account_clicked(GtkButton* button, gpointer data);
static void delete_account_clicked(GtkButton* button, gpointer data);

enum {
	S_COLUMN1, S_COLUMN2, S_COLUMN3, S_COLUMN4, S_COLUMN5, S_COLUMN6, S_COLUMN7, S_COLUMN8, S_COLUMN9,
	S_N_COLUMNS
};

GtkWidget* create_staff_box() {

	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

	GtkWidget* stack = gtk_stack_new();
	GtkWidget* stackSwitcher = gtk_stack_switcher_new();
	gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(stackSwitcher), GTK_STACK(stack));

	GtkWidget* add_account_box = create_add_account_box();
	GtkWidget* delete_account_box = create_delete_account_box();
	GtkWidget* account_table_box = create_account_table_box();

	//------ Account stackSwitcher
	gtk_grid_attach(GTK_GRID(grid), stackSwitcher, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), stack, 0, 1, 1, 1);

	gtk_stack_add_titled(GTK_STACK(stack), add_account_box, "add account", "Add");
	gtk_stack_add_titled(GTK_STACK(stack), delete_account_box, "delete account", "Delete");

	//------
	gtk_box_pack_start(GTK_BOX(box), grid, 0, 0, 0);
	gtk_box_pack_start(GTK_BOX(box), account_table_box, 1, 1, 0);

	return box;
}

GtkWidget* create_account_table_box() {

	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	GtkWidget* scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
		GTK_POLICY_AUTOMATIC,
		GTK_POLICY_AUTOMATIC);
	GtkListStore* model;
	GtkWidget* view;
	GtkTreeViewColumn* column;
	GtkCellRenderer* renderer;

	model = gtk_list_store_new(S_N_COLUMNS,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING
	);

	Account accounts[MAX_USER];
	gint count = 0;
	loadAccountDataFromFile(accounts, &count);
	for (gint i = 0; i < count; i++) {
		gtk_list_store_insert_with_values(model, NULL, -1,
			S_COLUMN1, accounts[i].no,
			S_COLUMN2, accounts[i].id,
			S_COLUMN3, accounts[i].username,
			S_COLUMN4, accounts[i].password,
			S_COLUMN5, accounts[i].fullName,
			S_COLUMN6, accounts[i].gender,
			S_COLUMN7, accounts[i].dob,
			S_COLUMN8, accounts[i].position,
			S_COLUMN9, accounts[i].phoneNumber,
			-1);
	}

	//------
	view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(model));
	g_object_unref(model);

	renderer = gtk_cell_renderer_text_new();

	column = gtk_tree_view_column_new_with_attributes("No",
		renderer,
		"text", S_COLUMN1,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("ID",
		renderer,
		"text", S_COLUMN2,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Username",
		renderer,
		"text", S_COLUMN3,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Password",
		renderer,
		"text", S_COLUMN4,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Full Name",
		renderer,
		"text", S_COLUMN5,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Gender",
		renderer,
		"text", S_COLUMN6,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("DOB",
		renderer,
		"text", S_COLUMN7,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Position",
		renderer,
		"text", S_COLUMN8,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	column = gtk_tree_view_column_new_with_attributes("Phone Number",
		renderer,
		"text", S_COLUMN9,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	//------
	gchar LABEL[31] = "Accounts List: ";
	gchar NUM_OF_ACCOUNT[16];
	sprintf(NUM_OF_ACCOUNT, "%d", count);
	strcat(LABEL, NUM_OF_ACCOUNT);

	GtkWidget* label = gtk_label_new(LABEL);
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);

	g_object_set(label, "margin", 10, NULL);

	gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
	gtk_container_add(GTK_CONTAINER(scrolled_window), view);
	gtk_box_pack_start(GTK_BOX(box), scrolled_window, 1, 1, 0);
	gtk_widget_set_size_request(box, 600, 365);
	g_object_set(scrolled_window, "margin-bottom", 270, NULL);

	return box;
}

GtkWidget* create_add_account_box() {

	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

	GtkWidget* label = gtk_label_new("New Account:");
	gtk_widget_set_size_request(label, 100, -1);
	GtkWidget* id_label = gtk_label_new("ID:");
	GtkWidget* fullName_label = gtk_label_new("Full Name:");
	GtkWidget* gender_label = gtk_label_new("Gender:");
	GtkWidget* dob_label = gtk_label_new("DOB:");
	GtkWidget* position_label = gtk_label_new("Position:");
	GtkWidget* phoneNumber_label = gtk_label_new("Phone Number:");

	GtkWidget* id_entry = gtk_entry_new();
	GtkWidget* fullName_entry = gtk_entry_new();
	GtkWidget* gender_entry = gtk_entry_new();
	GtkWidget* dob_entry = gtk_entry_new();
	GtkWidget* position_entry = gtk_entry_new();
	GtkWidget* phoneNumber_entry = gtk_entry_new();
	GtkWidget* add_account_button = gtk_button_new_with_label("Add");

	gtk_entry_set_max_length(GTK_ENTRY(phoneNumber_entry), 10);

	gtk_label_set_xalign(GTK_LABEL(label), 0);
	gtk_label_set_xalign(GTK_LABEL(id_label), 0);
	gtk_label_set_xalign(GTK_LABEL(fullName_label), 0);
	gtk_label_set_xalign(GTK_LABEL(gender_label), 0);
	gtk_label_set_xalign(GTK_LABEL(dob_label), 0);
	gtk_label_set_xalign(GTK_LABEL(position_label), 0);
	gtk_label_set_xalign(GTK_LABEL(phoneNumber_label), 0);

	gtk_grid_attach(GTK_GRID(grid), label,			   0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), id_label,		   0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), fullName_label,	   0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), gender_label,	   0, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), dob_label,		   0, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), position_label,	   0, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), phoneNumber_label, 0, 6, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), id_entry,		    1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), fullName_entry,	    1, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), gender_entry,	    1, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), dob_entry,		    1, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), position_entry,     1, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), phoneNumber_entry,  1, 6, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), add_account_button, 1, 7, 1, 1);

	gtk_box_pack_start(GTK_BOX(box), grid, 1, 1, 0);

	g_object_set_data(G_OBJECT(add_account_button), "id_entry", id_entry);
	g_object_set_data(G_OBJECT(add_account_button), "fullName_entry", fullName_entry);
	g_object_set_data(G_OBJECT(add_account_button), "gender_entry", gender_entry);
	g_object_set_data(G_OBJECT(add_account_button), "dob_entry", dob_entry);
	g_object_set_data(G_OBJECT(add_account_button), "position_entry", position_entry);
	g_object_set_data(G_OBJECT(add_account_button), "phoneNumber_entry", phoneNumber_entry);

	g_signal_connect_swapped(dob_entry, "key_press_event", G_CALLBACK(zero_to_nine_keys_callback), dob_entry);
	g_signal_connect_swapped(phoneNumber_entry, "key_press_event", G_CALLBACK(zero_to_nine_keys_callback), phoneNumber_entry);

	g_signal_connect(add_account_button, "clicked", G_CALLBACK(add_account_clicked), NULL);

	return box;
}

GtkWidget* create_delete_account_box() {

	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

	GtkWidget* label = gtk_label_new("Account:");
	gtk_widget_set_size_request(label, 100, -1);
	GtkWidget* id_label = gtk_label_new("ID:");
	GtkWidget* username_label = gtk_label_new("Username:");

	GtkWidget* id_entry = gtk_entry_new();
	GtkWidget* username_entry = gtk_entry_new();
	GtkWidget* delete_account_button = gtk_button_new_with_label("Delete");

	gtk_entry_set_max_length(GTK_ENTRY(id_entry), 8);

	gtk_label_set_xalign(GTK_LABEL(label), 0);
	gtk_label_set_xalign(GTK_LABEL(id_label), 0);
	gtk_label_set_xalign(GTK_LABEL(username_label), 0);

	gtk_grid_attach(GTK_GRID(grid), label,			0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), id_label,		0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), username_label, 0, 2, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), id_entry,			   1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), username_entry,		   1, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), delete_account_button, 1, 3, 1, 1);

	gtk_box_pack_start(GTK_BOX(box), grid, 1, 1, 0);

	g_object_set_data(G_OBJECT(delete_account_button), "id_entry", id_entry);
	g_object_set_data(G_OBJECT(delete_account_button), "username_entry", username_entry);

	g_signal_connect(delete_account_button, "clicked", G_CALLBACK(delete_account_clicked), NULL);

	return box;
}

static void add_account_clicked(GtkButton* button, gpointer data) {

	GtkWidget* id_entry =		   GTK_WIDGET(g_object_get_data(G_OBJECT(button), "id_entry"));
	GtkWidget* fullName_entry =    GTK_WIDGET(g_object_get_data(G_OBJECT(button), "fullName_entry"));
	GtkWidget* gender_entry =	   GTK_WIDGET(g_object_get_data(G_OBJECT(button), "gender_entry"));
	GtkWidget* dob_entry =		   GTK_WIDGET(g_object_get_data(G_OBJECT(button), "dob_entry"));
	GtkWidget* position_entry =	   GTK_WIDGET(g_object_get_data(G_OBJECT(button), "position_entry"));
	GtkWidget* phoneNumber_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "phoneNumber_entry"));

	const gchar* id = gtk_entry_get_text(GTK_ENTRY(id_entry));
	const gchar* fullName = gtk_entry_get_text(GTK_ENTRY(fullName_entry));
	const gchar* gender = gtk_entry_get_text(GTK_ENTRY(gender_entry));
	const gchar* dob = gtk_entry_get_text(GTK_ENTRY(dob_entry));
	const gchar* position = gtk_entry_get_text(GTK_ENTRY(position_entry));
	const gchar* phoneNumber = gtk_entry_get_text(GTK_ENTRY(phoneNumber_entry));
	
	GtkWidget* dialog = gtk_dialog_new_with_buttons("Delete an account",
		NULL,
		GTK_DIALOG_MODAL,
		"_Cancel",
		GTK_RESPONSE_CANCEL,
		NULL);
	GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_default_size(GTK_WINDOW(dialog), -1, -1);

	GtkWidget* label = gtk_label_new("A new account is added.");
	GtkWidget* label2 = gtk_label_new("This ID has been used.");
	GtkWidget* label3 = gtk_label_new("Please fill all of the information.");

	g_object_set(label, "margin", 10, NULL);
	g_object_set(label2, "margin", 10, NULL);
	g_object_set(label3, "margin", 10, NULL);

	//------ 
	Account accounts[MAX_USER];
	gint count = 0;
	loadAccountDataFromFile(accounts, &count);
	
	gint response = 1;
	for (gint i = 0; i < count; i++) {
		if (g_strcmp0(accounts[i].id, id) == 0) {
			response = 2;
			goto RESPONSE;
		}
	}
	if (id[0] == '\0' || fullName[0] == '\0' || gender[0] == '\0' || dob[0] == '\0' || position[0] == '\0' || phoneNumber[0] == '\0') {

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

	gchar no[MAX_USER];
	sprintf(no, "%d", count + 1);

	gchar word[10][51];
	gchar username[101] = "";
	gchar password[101] = "123456";
	gchar name[101];
	strcpy(name, fullName);

	for (gint i = 0; i < strlen(trim(name)); i++) {
		if (name[i] >= 'A' && name[i] <= 'Z') name[i] = name[i] + 32;
	}
	gint word_cnt = 0;
	gchar* token = strtok(name, " ");
	while (token != NULL) {
		strcpy(word[word_cnt], token);
		word_cnt++;
		token = strtok(NULL, " ");
	}
	if (word_cnt>1) strcat(username, word[word_cnt - 1]);
	strcat(username, word[0]);

	strcpy(accounts[count].no, no);
	strcpy(accounts[count].id, id);
	strcpy(accounts[count].username, username);
	strcpy(accounts[count].password, password);
	strcpy(accounts[count].fullName, fullName);
	strcpy(accounts[count].gender, gender);
	strcpy(accounts[count].dob, dob);
	strcpy(accounts[count].position, position);
	strcpy(accounts[count].phoneNumber, phoneNumber);
	strcpy(accounts[count].profileImage, "NULL");
	count++;
	storeAccountDataToFile(accounts, &count);

	//------
	gtk_box_pack_start(GTK_BOX(content_area), label, 0, 0, 0);
	gtk_widget_show_all(dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

static void delete_account_clicked(GtkButton* button, gpointer data) {

	GtkWidget* id_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "id_entry"));
	GtkWidget* username_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "username_entry"));

	const gchar* id = gtk_entry_get_text(GTK_ENTRY(id_entry));
	const gchar* username = gtk_entry_get_text(GTK_ENTRY(username_entry));

	GtkWidget* dialog = gtk_dialog_new_with_buttons("Delete an account",
		NULL,
		GTK_DIALOG_MODAL,
		"_Cancel",
		GTK_RESPONSE_CANCEL,
		NULL);
	GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_default_size(GTK_WINDOW(dialog), -1, -1);

	GtkWidget* label = gtk_label_new("This account is deleted successfully!");
	GtkWidget* label2 = gtk_label_new("Cannot find the ID or username.");

	g_object_set(label, "margin", 10, NULL);
	g_object_set(label2, "margin", 10, NULL);

	Account accounts[MAX_USER];
	gint count = 0;
	loadAccountDataFromFile(accounts, &count);

	gboolean found = FALSE;
	gint index;
	for (gint i = 0; i < count; i++) {
		if (g_strcmp0(accounts[i].id, id) == 0 && g_strcmp0(accounts[i].username, username) == 0) {
			index = i;
			found = TRUE;
			break;
		}
	}
	if (!found) {
		gtk_box_pack_start(GTK_BOX(content_area), label2, 0, 0, 0);
		gtk_widget_show_all(dialog);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return;
	}

	for (gint i = index; i < count - 1; i++) {
		
		strcpy(accounts[i].id,			 accounts[i+1].id);
		strcpy(accounts[i].username,	 accounts[i+1].username);
		strcpy(accounts[i].password,	 accounts[i+1].password);
		strcpy(accounts[i].fullName,	 accounts[i+1].fullName);
		strcpy(accounts[i].gender,		 accounts[i+1].gender);
		strcpy(accounts[i].dob,			 accounts[i+1].dob);
		strcpy(accounts[i].position,	 accounts[i+1].position);
		strcpy(accounts[i].phoneNumber,  accounts[i+1].phoneNumber);
		strcpy(accounts[i].profileImage, accounts[i+1].profileImage);
	}
	count--;
	storeAccountDataToFile(accounts, &count);

	//------
	gtk_box_pack_start(GTK_BOX(content_area), label, 0, 0, 0);
	gtk_widget_show_all(dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

GtkWidget* create_show_info_box(Account accounts);
GtkWidget* create_show_info_box2(Account accounts);

static void save_clicked(GtkButton* button, gpointer data);
static void change_password_clicked(GtkButton* button, gpointer data);
static void sign_out_clicked(GtkButton* button, gpointer data);

GtkWidget* create_info_box(Account accounts) {

	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

	GtkWidget* show_info_box = create_show_info_box(accounts);
	GtkWidget* show_info_box2 = create_show_info_box2(accounts);

	gtk_box_pack_start(GTK_BOX(box), show_info_box, 0, 0, 0);
	gtk_box_pack_start(GTK_BOX(box), show_info_box2, 0, 0, 0);

	return box;
}

GtkWidget* create_show_info_box(Account accounts) {

	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

	GtkWidget* label = gtk_label_new("PROFILE");
	gtk_widget_set_size_request(label, 100, -1);
	GtkWidget* ID_label = gtk_label_new("ID:");
	GtkWidget* fname_label = gtk_label_new("Full Name:");
	GtkWidget* gender_label = gtk_label_new("Gender:");
	GtkWidget* dob_label = gtk_label_new("DOB:");
	GtkWidget* position_label = gtk_label_new("Position:");
	GtkWidget* phone_number_label = gtk_label_new("Phone number:");

	GtkEntryBuffer* ID_buffer = gtk_entry_buffer_new(accounts.id, -1);
	GtkEntryBuffer* fname_buffer = gtk_entry_buffer_new(accounts.fullName, -1);
	GtkEntryBuffer* gender_buffer = gtk_entry_buffer_new(accounts.gender, -1);
	GtkEntryBuffer* dob_buffer = gtk_entry_buffer_new(accounts.dob, -1);
	GtkEntryBuffer* position_buffer = gtk_entry_buffer_new(accounts.position, -1);
	GtkEntryBuffer* phone_number_buffer = gtk_entry_buffer_new(accounts.phoneNumber, -1);

	GtkWidget* ID_entry = gtk_entry_new_with_buffer(ID_buffer);
	GtkWidget* fname_entry = gtk_entry_new_with_buffer(fname_buffer);
	GtkWidget* gender_entry = gtk_entry_new_with_buffer(gender_buffer);
	GtkWidget* dob_entry = gtk_entry_new_with_buffer(dob_buffer);
	GtkWidget* position_entry = gtk_entry_new_with_buffer(position_buffer);
	GtkWidget* phone_number_entry = gtk_entry_new_with_buffer(phone_number_buffer);

	gtk_entry_set_max_length(GTK_ENTRY(phone_number_entry), 10);

	gtk_label_set_xalign(GTK_LABEL(label), 0);
	gtk_label_set_xalign(GTK_LABEL(ID_label), 0);
	gtk_label_set_xalign(GTK_LABEL(fname_label), 0);
	gtk_label_set_xalign(GTK_LABEL(gender_label), 0);
	gtk_label_set_xalign(GTK_LABEL(dob_label), 0);
	gtk_label_set_xalign(GTK_LABEL(position_label), 0);
	gtk_label_set_xalign(GTK_LABEL(phone_number_label), 0);

	gtk_widget_set_sensitive(GTK_WIDGET(ID_entry), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(fname_entry), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(gender_entry), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(dob_entry), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(position_entry), FALSE);

	GtkWidget* save_button = gtk_button_new_with_label("Save");

	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), ID_label, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), fname_label, 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), gender_label, 0, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), dob_label, 0, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), position_label, 0, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), phone_number_label, 0, 6, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), ID_entry, 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), fname_entry, 1, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), gender_entry, 1, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), dob_entry, 1, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), position_entry, 1, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), phone_number_entry, 1, 6, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), save_button, 1, 7, 1, 1);

	g_object_set_data(G_OBJECT(save_button), "phone_number_entry", phone_number_entry);

	g_signal_connect_swapped(phone_number_entry, "key_press_event", G_CALLBACK(zero_to_nine_keys_callback), phone_number_entry);

	g_signal_connect(save_button, "clicked", G_CALLBACK(save_clicked), NULL);

	gtk_box_pack_start(GTK_BOX(box), grid, 0, 0, 0);

	return box;
}

GtkWidget* create_show_info_box2(Account accounts) {

	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

	GtkWidget* account_label = gtk_label_new("Account");
	GtkWidget* profile_image = gtk_button_new();
	GtkWidget* change_password_button = gtk_button_new_with_label("Change password");
	GtkWidget* sign_out_button = gtk_button_new_with_label("Sign out");

	GtkWidget* image;
	GdkPixbuf* pixbuf;

	pixbuf = gdk_pixbuf_new_from_file_at_scale("E:\\msys64\\home\\HIEU\\image\\profile_image.jpg", 150, 150, TRUE, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);

	gtk_widget_set_size_request(profile_image, 150, 150);
	gtk_button_set_image(GTK_BUTTON(profile_image), image);

	gtk_grid_attach(GTK_GRID(grid), account_label, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), profile_image, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), change_password_button, 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), sign_out_button, 0, 3, 1, 1);

	g_signal_connect(change_password_button, "clicked", G_CALLBACK(change_password_clicked), NULL);
	g_signal_connect(sign_out_button, "clicked", G_CALLBACK(sign_out_clicked), NULL);

	gtk_box_pack_start(GTK_BOX(box), grid, 0, 0, 0);

	return box;
}

static void save_clicked(GtkButton* button, gpointer data) {

	GtkWidget* phone_number_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "phone_number_entry"));

	const gchar* phoneNumber = gtk_entry_get_text(GTK_ENTRY(phone_number_entry));

	GtkWidget* dialog = gtk_dialog_new_with_buttons("Change Your Password",
		NULL,
		GTK_DIALOG_MODAL,
		"_Cancel",
		GTK_RESPONSE_CANCEL,
		NULL);
	gtk_container_set_border_width(GTK_CONTAINER(dialog), 20);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_default_size(GTK_WINDOW(dialog), -1, -1);

	GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	gint response;
	response = 1;

	if (strlen(phoneNumber) == 0 || strlen(phoneNumber) != 10) {
		response = 0;
		goto RESPONSE;
	}
	for (gint i = 0; i < strlen(phoneNumber); i++) {
		if (isspace(phoneNumber[i])) {
			response = 0;
			goto RESPONSE;
		}
	}

	//------
	Account accounts[MAX_USER]; gint count = 0;
	loadAccountDataFromFile(accounts, &count);
	for (gint i = 0; i < count; i++) {
		if (g_strcmp0(accounts[i].id, get_account.id) == 0) {
			strcpy(accounts[i].phoneNumber, phoneNumber);
			break;
		}
	}
	storeAccountDataToFile(accounts, &count);

RESPONSE:
	if (response == 1) {
		GtkWidget* label = gtk_label_new("Your information has been saved.");
		g_object_set(label, "margin-bottom", 10, NULL);
		gtk_box_pack_start(GTK_BOX(content_area), label, 0, 0, 0);
	}
	else if (response == 0) {
		GtkWidget* label = gtk_label_new("Invalid phone number.");
		g_object_set(label, "margin-bottom", 10, NULL);
		gtk_box_pack_start(GTK_BOX(content_area), label, 0, 0, 0);
	}
	gtk_widget_show_all(dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

static void change_password_clicked(GtkButton* button, gpointer data) {

	GtkWidget* dialog = gtk_dialog_new_with_buttons("Change Your Password",
		NULL,
		GTK_DIALOG_MODAL,
		"Continue",
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

	GtkWidget* old_password_label = gtk_label_new("Current Password:");
	GtkWidget* new_password_label = gtk_label_new("New Password:");
	GtkWidget* confirm_password_label = gtk_label_new("Confirm New Password:");
	GtkWidget* old_password_entry = gtk_entry_new();
	GtkWidget* new_password_entry = gtk_entry_new();
	GtkWidget* confirm_password_entry = gtk_entry_new();

	gtk_label_set_xalign(GTK_LABEL(old_password_label), 0);
	gtk_label_set_xalign(GTK_LABEL(new_password_label), 0);
	gtk_label_set_xalign(GTK_LABEL(confirm_password_label), 0);

	gtk_entry_set_visibility(GTK_ENTRY(old_password_entry), FALSE);
	gtk_entry_set_visibility(GTK_ENTRY(new_password_entry), FALSE);
	gtk_entry_set_visibility(GTK_ENTRY(confirm_password_entry), FALSE);

	gtk_grid_attach(GTK_GRID(grid), old_password_label, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), new_password_label, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), confirm_password_label, 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), old_password_entry, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), new_password_entry, 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), confirm_password_entry, 1, 2, 1, 1);

	const gchar* old_password;
	const gchar* new_password;
	const gchar* confirm_password;

	gtk_widget_show_all(dialog);

	gint response = gtk_dialog_run(GTK_DIALOG(dialog));
	switch (response) {
	case GTK_RESPONSE_OK:
		old_password = gtk_entry_get_text(GTK_ENTRY(old_password_entry));
		new_password = gtk_entry_get_text(GTK_ENTRY(new_password_entry));
		confirm_password = gtk_entry_get_text(GTK_ENTRY(confirm_password_entry));

		if (g_strcmp0(old_password, get_account.password) == 0 && g_strcmp0(old_password, new_password) != 0
			&& g_strcmp0(new_password, confirm_password) == 0 && new_password[0] != '\0') {

			Account accounts[MAX_USER]; gint count = 0;
			loadAccountDataFromFile(accounts, &count);
			for (gint i = 0; i < count; i++) {
				if (g_strcmp0(accounts[i].id, get_account.id) == 0) {
					strcpy(accounts[i].password, new_password);
					break;
				}
			}
			storeAccountDataToFile(accounts, &count);
			
			
			GtkWidget* label = gtk_label_new("Password changed!");
			gtk_label_set_xalign(GTK_LABEL(label), 1.0);
			g_object_set(label, "margin-bottom", 10, NULL);
			gtk_box_pack_start(GTK_BOX(content_area), label, 0, 0, 0);

			gtk_widget_show(label);
		}
		else {
			GtkWidget* label = gtk_label_new("Cannot change your password!");
			gtk_label_set_xalign(GTK_LABEL(label), 1.0);
			g_object_set(label, "margin-bottom", 10, NULL);
			gtk_box_pack_start(GTK_BOX(content_area), label, 0, 0, 0);

			gtk_widget_show(label);
		}
		gtk_widget_set_sensitive(old_password_entry, FALSE);
		gtk_widget_set_sensitive(new_password_entry, FALSE);
		gtk_widget_set_sensitive(confirm_password_entry, FALSE);
			break;
	default:
			gtk_widget_destroy(dialog);
			break;
	}

}

static void sign_out_clicked(GtkButton* button, gpointer data) {

	GtkWidget* dialog;
	GtkWidget* content_area;
	GtkWidget* label;
	gint response;

	dialog = gtk_dialog_new_with_buttons(NULL, NULL, GTK_DIALOG_MODAL, "Yes", 1, "No", 2, NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), 2);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);

	//------
	label = gtk_label_new("Are you sure you want to quit?");
	g_object_set(label, "margin", 10, NULL);
	content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	gtk_container_add(GTK_CONTAINER(content_area), label);

	gtk_widget_show_all(dialog);

	response = gtk_dialog_run(GTK_DIALOG(dialog));
	switch (response) {
	case 1:
		g_print("Yes\n");
		gtk_main_quit();
		break;
	case 2:
		gtk_widget_destroy(dialog);
		break;
	default:
		break;
	}
}
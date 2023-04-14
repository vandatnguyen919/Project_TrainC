#include "ProductData.h"

GtkWidget* create_good_info_grid(const gchar* id, const gchar* name, const gchar* price, const gchar* image_source);
GtkWidget* create_payment_good_grid(const gchar* id, const gchar* name, const gint quantity, const gdouble price);

static void print_good_to_menu(GtkWidget** grid, GtkWidget* good[], const gint MAX_NUM_OF_GOOD, gint MAX_COLUMN);
static void image_clicked(GtkButton* button, gpointer data);
static void continue_clicked(GtkWidget* button, gpointer data);
static void new_clicked(GtkButton* button, gpointer data);

#define MAX_TYPE_OF_GOOD 100
#define MAX_NUM_OF_ORDER 100	

GtkWidget* buttons[MAX_NUM_OF_ORDER];
GtkWidget* spinButtons[MAX_NUM_OF_ORDER];

GtkWidget* new_button;
GtkWidget* continue_button;

GtkWidget* create_menu_grid() {

	GtkWidget* grid;

	GtkWidget* sub_stack;
	GtkWidget* stackSwitcher;

	GtkWidget* image;
	GdkPixbuf* pixbuf;

	pixbuf = gdk_pixbuf_new_from_file_at_scale("E:\\msys64\\home\\HIEU\\image\\poster.jpg", 330, 635, FALSE, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);

	//------ Create Menu grid
	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

	sub_stack = gtk_stack_new();
	stackSwitcher = gtk_stack_switcher_new();
	gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(stackSwitcher), GTK_STACK(sub_stack));

	new_button = gtk_button_new_with_label("New Order");
	continue_button = gtk_button_new_with_label("Continue");

	gtk_grid_attach(GTK_GRID(grid), new_button,		 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), continue_button, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), stackSwitcher,	 0, 1, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), sub_stack,		 0, 2, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), image,			 2, 0, 1, 3);

	//------ Set the maximum column
	gint MAX_COLUMN = 3;

	Product products[MAX_GOOD];
	gint count = 0;
	loadProductDataFromFile(products, &count);

	//------ Create Food box --------------------------------------
	GtkWidget* food_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

	GtkWidget* food_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(food_scrolled_window),
		GTK_POLICY_AUTOMATIC,
		GTK_POLICY_AUTOMATIC);

	GtkWidget* food_grid = gtk_grid_new();;
	gtk_grid_set_row_spacing(GTK_GRID(food_grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(food_grid), 10);

	//------ Create Drinks grid 
	GtkWidget* drink_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(drink_scrolled_window),
		GTK_POLICY_AUTOMATIC,
		GTK_POLICY_AUTOMATIC);

	GtkWidget* drink_grid = gtk_grid_new();;
	gtk_grid_set_row_spacing(GTK_GRID(drink_grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(drink_grid), 10);

	gtk_container_add(GTK_CONTAINER(food_scrolled_window), food_grid);
	gtk_box_pack_start(GTK_BOX(food_box), food_scrolled_window, 1, 1, 0);
	gtk_widget_set_size_request(food_box, 525, 550);

	gtk_container_add(GTK_CONTAINER(drink_scrolled_window), drink_grid);

	//-------------------------------------------------------------
	GtkWidget* food[MAX_TYPE_OF_GOOD];
	GtkWidget* drink[MAX_TYPE_OF_GOOD];

	gint i, MAX_FOOD, MAX_DRINK;
	for (i = MAX_FOOD = MAX_DRINK = 0; i < count; i++) {
		if (g_strcmp0(products[i].type, "Food") == 0) {
			food[MAX_FOOD++] = create_good_info_grid(products[i].id, products[i].name, products[i].ExPrice, products[i].productImage);
		}
		else if (g_strcmp0(products[i].type, "Drink") == 0) {
			drink[MAX_DRINK++] = create_good_info_grid(products[i].id, products[i].name, products[i].ExPrice, products[i].productImage);
		}
	}
	
	// Print products to menu
	print_good_to_menu(&food_grid, food, MAX_FOOD, MAX_COLUMN);
	print_good_to_menu(&drink_grid, drink, MAX_DRINK, MAX_COLUMN);

	//------
	g_signal_connect(new_button, "clicked", G_CALLBACK(new_clicked), continue_button);
	g_signal_connect(continue_button, "clicked", G_CALLBACK(continue_clicked), new_button);

	//------ Attach to menu stackSwitcher
	gtk_stack_add_titled(GTK_STACK(sub_stack), food_box, "food", "Food");
	gtk_stack_add_titled(GTK_STACK(sub_stack), drink_scrolled_window, "drink", "Drinks");

	return grid;
}

static gint NUM_OF_GOOD = 0;

GtkWidget* create_good_info_grid(const gchar* id, const gchar* name, const gchar* price, const gchar* image_source) {

	GtkWidget* grid;

	GtkWidget* button;
	GtkWidget* image;
	GdkPixbuf* pixbuf;

	GtkWidget* name_label;
	GtkWidget* price_label;
	GtkWidget* id_label;
	GtkWidget* quantity_spinButton;

	grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 2);

	//------ Add name, price, ID, and image
	pixbuf = gdk_pixbuf_new_from_file_at_scale(image_source, 150, 150, TRUE, NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);

	button = gtk_button_new();
	name_label = gtk_label_new(name);
	id_label = gtk_label_new(id);
	price_label = gtk_label_new(price);
	quantity_spinButton = gtk_spin_button_new_with_range(0.0, 999.0, 1);

	gtk_button_set_image(GTK_BUTTON(button), image);
	gtk_widget_set_size_request(button, 150, 150);

	gtk_label_set_xalign(GTK_LABEL(name_label), 0);
	gtk_label_set_xalign(GTK_LABEL(id_label), 0);
	gtk_label_set_xalign(GTK_LABEL(price_label), 1.0);

	//------ Show name, price, and ID
	gtk_grid_attach(GTK_GRID(grid), button,				 0, 0, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), name_label,			 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), price_label,		 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), id_label,			 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), quantity_spinButton, 0, 3, 2, 1);

	buttons[NUM_OF_GOOD] = button;
	spinButtons[NUM_OF_GOOD] = quantity_spinButton;
	NUM_OF_GOOD++;

	gtk_widget_set_sensitive(GTK_WIDGET(button), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(quantity_spinButton), FALSE);

	g_object_set_data(G_OBJECT(button), "id_label", id_label);
	g_object_set_data(G_OBJECT(button), "name_label", name_label);
	g_object_set_data(G_OBJECT(button), "price_label", price_label);
	g_object_set_data(G_OBJECT(button), "spinButton", quantity_spinButton);

	g_signal_connect(button, "clicked", G_CALLBACK(image_clicked), NULL);

	return grid;
}

static void print_good_to_menu(GtkWidget** grid, GtkWidget* good[], const gint MAX_NUM_OF_GOOD, gint MAX_COLUMN) {
	gint MAX_ROW, count = 0;
	MAX_ROW = MAX_NUM_OF_GOOD / MAX_COLUMN;
	if (MAX_NUM_OF_GOOD % MAX_COLUMN > 0) {
		MAX_ROW++;
	}
	if (MAX_NUM_OF_GOOD < MAX_COLUMN) MAX_COLUMN = MAX_NUM_OF_GOOD % MAX_COLUMN;
	count = 0;
	for (gint i = 0; i < MAX_ROW; i++) {
		for (gint j = 0; j < MAX_COLUMN; j++) {
			if ((i == MAX_ROW - 1) && (MAX_NUM_OF_GOOD % MAX_COLUMN > 0)) MAX_COLUMN = MAX_NUM_OF_GOOD % MAX_COLUMN;
			gtk_grid_attach(GTK_GRID(*grid), good[count++], j, i, 1, 1);
		}
	}
}

static gint NUM_OF_ORDER = 0;

typedef struct {
	const gchar* id,* name;
	gdouble price;
	gint quantity;
	GtkWidget* spinButton;
} Order;

Order order[MAX_NUM_OF_ORDER];

static void image_clicked(GtkButton* button, gpointer data) {

	g_signal_handlers_disconnect_by_func(new_button, new_clicked, continue_button);

	GtkWidget* id_label = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "id_label"));
	GtkWidget* name_label = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "name_label"));
	GtkWidget* price_label = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "price_label"));
	GtkWidget* spinButton = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "spinButton"));

	gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinButton), 1.0, 999.0);

	order[NUM_OF_ORDER].id = gtk_label_get_text(GTK_LABEL(id_label));
	order[NUM_OF_ORDER].name = gtk_label_get_text(GTK_LABEL(name_label));
	order[NUM_OF_ORDER].price = atof(gtk_label_get_text(GTK_LABEL(price_label)));
	order[NUM_OF_ORDER].spinButton = spinButton;

	NUM_OF_ORDER++;
	g_print("%d %s %s %.0lf\n", NUM_OF_ORDER, order[NUM_OF_ORDER-1].id, order[NUM_OF_ORDER-1].name, order[NUM_OF_ORDER-1].price);

	gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(spinButton), TRUE);

}

static void new_clicked(GtkButton* button, gpointer data) {

	if (NUM_OF_ORDER < 1) return;
	
	g_signal_handlers_disconnect_by_func(button, new_clicked, continue_button);
	g_signal_connect(continue_button, "clicked", G_CALLBACK(continue_clicked), button);

	for (gint i = 0; i < NUM_OF_GOOD; i++) {
		g_signal_connect(buttons[i], "clicked", G_CALLBACK(image_clicked), NULL);
	}

	for (gint i = 0; i < NUM_OF_GOOD; i++) {

		gtk_widget_set_sensitive(buttons[i], TRUE);
		gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinButtons[i]), 0.0, 999.0);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinButtons[i]), 0.0);
		gtk_widget_set_sensitive(GTK_WIDGET(spinButtons[i]), FALSE);
	}
	NUM_OF_ORDER = 0;
}

static void continue_clicked(GtkWidget* button, gpointer data) {
	
	if (NUM_OF_ORDER < 1) return;

	g_signal_handlers_disconnect_by_func(button, continue_clicked, new_button);
	g_signal_connect(new_button, "clicked", G_CALLBACK(new_clicked), button);

	for (gint i = 0; i < NUM_OF_GOOD; i++) {
		g_signal_handlers_disconnect_by_func(buttons[i], image_clicked, NULL);
	}

	gdouble sum = 0;
	for (gint i = 0; i < NUM_OF_ORDER; i++) {

		gtk_widget_set_sensitive(GTK_WIDGET(order[i].spinButton), FALSE);
		order[i].quantity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(order[i].spinButton));

		sum += (order[i].quantity * order[i].price);
	}
	gchar sum_text[20];
	sprintf(sum_text, "%.0f", sum);

	time_t current_time;
	current_time = time(NULL);

	//------
	GtkWidget *dialog = gtk_dialog_new_with_buttons("Your Bill",
                                                    NULL,
                                                    GTK_DIALOG_MODAL,
                                                    "_Confirm",
                                                    GTK_RESPONSE_OK,
                                                    "_Cancel",
                                                    GTK_RESPONSE_CANCEL,
                                                    NULL);
    GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_default_size(GTK_WINDOW(dialog), -1, 500);

	GtkWidget* label = gtk_label_new("Bill");
	GtkWidget* date_label = gtk_label_new(ctime(&current_time));

	gtk_label_set_xalign(GTK_LABEL(label), 0);
	gtk_label_set_xalign(GTK_LABEL(date_label), 0);

	//------
    GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

	gchar total_text[31] = "Total:           ";
	strcat(total_text, sum_text);
	GtkWidget* sum_label = gtk_label_new(total_text);

	gtk_label_set_xalign(GTK_LABEL(sum_label), 1.0);

	//------ Print bill
	GtkWidget* payment[MAX_NUM_OF_ORDER];

	for (gint i = 0; i < NUM_OF_ORDER; i++) {
		payment[i] = create_payment_good_grid(order[i].id, order[i].name, order[i].quantity, order[i].price);
		gtk_grid_attach(GTK_GRID(grid), payment[i], 0, i, 1, 1);
	}
	gtk_grid_attach(GTK_GRID(grid), sum_label, 0, NUM_OF_ORDER, 1, 1);

	g_object_set(label,		 "margin", 10, NULL);
	g_object_set(date_label, "margin-left", 10, NULL);
	g_object_set(grid,		 "margin", 10, NULL);

	gtk_box_pack_start(GTK_BOX(content_area), label, 0, 0, 0);
	gtk_box_pack_start(GTK_BOX(content_area), date_label, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(content_area), grid, 0, 0, 0);

    gtk_widget_show_all(dialog);

	gint response = gtk_dialog_run(GTK_DIALOG(dialog));
	switch (response) {
	case GTK_RESPONSE_OK:
		Product products[MAX_GOOD]; gint count = 0;
		loadProductDataFromFile(products, &count);
		for (gint i = NUM_OF_ORDER - 1; i >= 0 ; i--) {
			for (gint j = 0; j < count; j++) {
				if (g_strcmp0(products[j].id, order[i].id) == 0) {
					
					gint new_quantity = atoi(products[j].quantity) - order[i].quantity;
					gchar new_quantity_text[21]; sprintf(new_quantity_text, "%d", new_quantity);
					strcpy(products[j].quantity, new_quantity_text);

					storeProductDataToExportFile(products[j], order[i].quantity);
					break;
				}
			}
		}
		storeProductDataToFile(products, &count);

		GtkWidget* notify_label = gtk_label_new("Your payment is successfull!");
		gtk_label_set_xalign(GTK_LABEL(notify_label), 1.0);
		g_object_set(notify_label, "margin-bottom", 10, NULL);
		gtk_box_pack_start(GTK_BOX(content_area), notify_label, 0, 0, 0);

		gtk_widget_show(notify_label);
		break;
	default:
		gtk_widget_destroy(dialog);
	}
   
}

GtkWidget* create_payment_good_grid(const gchar* id, const gchar* name, const gint quantity, const gdouble price) {

	GtkWidget* grid;
	GtkWidget* box;

	GtkWidget* name_label;
	GtkWidget* id_label;
	GtkWidget* price_label;
	GtkWidget* multiply_label;
	GtkWidget* quantity_label;
	GtkWidget* subTotal_label;


	grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

	gchar quantity_text[20];
	gchar price_text[20];
	gchar subTotal_text[20];
	sprintf(quantity_text, "%d", quantity);
	sprintf(price_text, "%.0f", price);
	sprintf(subTotal_text, "%.0f", price*quantity);

	name_label = gtk_label_new(name);
	id_label = gtk_label_new(id);
	price_label = gtk_label_new(price_text);
	multiply_label = gtk_label_new("x");
	quantity_label = gtk_label_new(quantity_text);
	subTotal_label = gtk_label_new(subTotal_text);

	//------
	gtk_label_set_xalign(GTK_LABEL(name_label), 0);
	gtk_label_set_xalign(GTK_LABEL(id_label), 0);
	gtk_label_set_xalign(GTK_LABEL(subTotal_label), 1.0);
	gtk_widget_set_size_request(price_label, 80, -1);

	gtk_box_pack_start(GTK_BOX(box), name_label, 0, 0, 0);
	gtk_box_pack_start(GTK_BOX(box), id_label, 0, 0, 0);
	gtk_widget_set_size_request(box, 120, -1);

	g_object_set(multiply_label, "margin-right", 10, NULL);
	gtk_grid_attach(GTK_GRID(grid), box, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), quantity_label, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), multiply_label, 2, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), price_label, 3, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), subTotal_label, 4, 0, 1, 1);

	return grid;
}
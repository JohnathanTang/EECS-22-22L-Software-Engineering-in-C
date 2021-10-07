#include <stdio.h> 
#include <stdlib.h> 
#include <gtk/gtk.h> 
#include <string.h> 

GtkWidget *username; 
const char *empty = ""; 
static void enter_callback(GtkWidget *widget, GtkWidget *entry){

const gchar *entry1; 
entry1 = gtk_entry_get_text (GTK_ENTRY (username)); 

if(strcmp(entry1,empty) == 0){
	g_print("You did not enter a username\n"); 
}
else{
	g_print("You've added: %s\n",entry1); 
	gtk_main_quit(); 
}
}

int main(int argc, char *argv[])
{

	GtkWidget *window; 
	GtkWidget *vbox, *hbox; 
	GtkWidget *button;
	GtkWidget *label;  
	GtkWidget *fixed; 

	gtk_init(&argc,&argv); 

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
	gtk_widget_set_size_request(GTK_WIDGET(window), 150, 150); 
	gtk_window_set_title(GTK_WINDOW(window), "Add"); 

	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL); 
	g_signal_connect(window,"delete-event",G_CALLBACK(gtk_widget_destroy),window); 

	label = gtk_label_new("Add Friend: "); 
	char *string = g_strdup_printf("<span font=\"10\" color =\"black\">" "<b>Add Friend: </b>" "</span>"); 
	gtk_label_set_markup(GTK_LABEL(label),string); 
	g_free(string); 

	username = gtk_entry_new(); 
	gtk_entry_set_max_length (GTK_ENTRY(username),50); 
	g_signal_connect(username,"activate",G_CALLBACK(enter_callback),username); 

	vbox = gtk_vbox_new(FALSE,0); 
	gtk_container_add(GTK_CONTAINER(window),vbox); 
	gtk_widget_show(vbox); 

	gtk_box_pack_start(GTK_BOX(vbox), username, TRUE, TRUE, 0); 
	gtk_widget_show(username); 

	hbox = gtk_hbox_new(FALSE,0); 
	gtk_container_add(GTK_CONTAINER(vbox),hbox); 
	gtk_widget_show(hbox); 

	button = gtk_button_new_from_stock(GTK_STOCK_CLOSE); 
	g_signal_connect_swapped (button,"clicked",G_CALLBACK(gtk_widget_destroy),window); 
	gtk_box_pack_start (GTK_BOX(vbox),button,TRUE,TRUE,0); 
	gtk_widget_set_can_default(button,TRUE); 
	gtk_widget_grab_default(button); 
	gtk_widget_show(button); 
	
	fixed = gtk_fixed_new(); 
	gtk_fixed_put(GTK_FIXED(fixed), label, 100,100); 
	gtk_container_add(GTK_CONTAINER(window),fixed);  
	
	gtk_widget_show(window); 
	gtk_main(); 
	return 0; 
}

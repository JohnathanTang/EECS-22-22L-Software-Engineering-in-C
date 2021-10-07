#include <gtk/gtk.h> 


/*static void print(GtkWidget *widget, gpointer data)
{
	g_print("%s was clicked\n", (char *) data); 
}*/


void Accept(GtkButton *button, void *user_data); 
void Decline(GtkButton *button, void *user_data); 
GtkWidget *window; 
GtkWidget *button1, *button2, *button3;
GtkWidget *grid; 
GtkWidget *hbox;
GtkWidget *box;
GtkWidget *fixed; 
GtkWidget *label;  
int main(int argc, char*argv[])
{
	gtk_init(&argc,&argv);
	
	/*Create Label*/
	label = gtk_label_new("Accept Friend Request?"); 
	char *string = g_strdup_printf("<span font=\"10\" color=\"black\">" "<b>Accept Friend Request?</b>" "</span>"); 
	gtk_label_set_markup (GTK_LABEL(label), string); 
	g_free (string);

	/*Create Window*/ 
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
	gtk_window_set_title (GTK_WINDOW(window), "Friend Request"); 
	gtk_widget_set_size_request(window,250,250); 	
	
	/*Connect X button to close and terminate window*/
	gtk_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC (gtk_exit),NULL); 
	gtk_signal_connect(GTK_OBJECT(window),"delete_event",GTK_SIGNAL_FUNC(gtk_exit),NULL); 

	/*Declare buttons*/
	button1 = gtk_button_new_with_label("Accept"); 
	g_signal_connect(button1,"clicked",G_CALLBACK(Accept),"Accept"); 
	
	button2 = gtk_button_new_with_label("Decline"); 
	g_signal_connect(button2,"clicked",G_CALLBACK(Decline),"Decline"); 
	
	button3 = gtk_button_new_with_label("Ignore"); 
	g_signal_connect_swapped(button3,"clicked",G_CALLBACK(gtk_widget_destroy),window);
	
	/*Place objects in fixed locations*/
	fixed = gtk_fixed_new(); 
	gtk_fixed_put(GTK_FIXED(fixed) , label, 25, 50); 
	gtk_fixed_put(GTK_FIXED(fixed) , button1, 50,150); 
	gtk_fixed_put(GTK_FIXED(fixed) , button2, 110,150);
	gtk_fixed_put(GTK_FIXED(fixed) , button3, 75,175);   
	gtk_container_add(GTK_CONTAINER(window),fixed); 
	
	gtk_widget_show_all(window); 
	gtk_main(); 
	return 0;  	
}

/*If user clicks accepts button, add the incoming user to contact list*/
void Accept(GtkButton *button, void *user_data)
{
	g_print("Accept Pressed\n");
	/*Add incoming contact to list*/ 
	/*Hide window*/ 
}
/*if user clicks decline button, remove the pending request from user's feed*/ 
void Decline (GtkButton *button, void *user_data)
{
	g_print("Decline Pressed\n"); 
	/*Destroy pending request*/	
	/*Hide window*/ 
}

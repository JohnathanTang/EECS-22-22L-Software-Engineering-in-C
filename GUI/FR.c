#include <gtk/gtk.h>


static GtkWidget *window;
static GtkWidget *fixed; 
static GtkWidget *box;
static int AcceptorDecline = 0;

void AorD (GtkButton *button, void *user_data); 
void callback(GtkWidget *widget, gpointer data)
{
	g_print("%s was pressed\n",(char *) data); 
}
int main (int argc, char *argv[]){
	gtk_init(&argc, &argv); 
	GtkWidget *vbox;
	GtkWidget *hbox; 

	/*Main Window*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	g_signal_connect(window,"destroy", G_CALLBACK(gtk_main_quit),NULL); 
	g_signal_connect_swapped (window,"delete-event",G_CALLBACK (gtk_widget_destroy),window); 
	/*Title of the Window*/ 
	gtk_window_set_title(GTK_WINDOW(window),"Friend Request"); 
	gtk_widget_set_size_request(window,200,200); 

		

	GtkWidget *btn1, *btn2; 
	
	btn1 = gtk_button_new_with_label("Accept"); 
	btn2 = gtk_button_new_with_label("Decline"); 
	
	GtkWidget *sepbox1,*sepbox2,*sepbox3,*sepbox4;
	sepbox1 = gtk_hbox_new(0,0); 
	sepbox2 = gtk_hbox_new(0,0); 
	sepbox3 = gtk_hbox_new(0,0); 
	sepbox4 = gtk_hbox_new(0,0); 
	
	 
	hbox = gtk_hbox_new(FALSE,0); 
	gtk_container_add(GTK_CONTAINER (hbox),hbox); 
	gtk_widget_show(hbox); 
	
	gtk_signal_connect(GTK_OBJECT(btn1),"clicked", GTK_SIGNAL_FUNC(callback),(gpointer)"Accept"); 
	gtk_signal_connect(GTK_OBJECT(btn2),"clicked", GTK_SIGNAL_FUNC(callback),(gpointer)"Decline"); 


	gtk_widget_show(btn1); 
	gtk_widget_show(btn2); 


	/*gtk_box_pack_start(GTK_BOX(hbox),btn1,1,1,0); 
	g_signal_connect(btn1,"clicked",G_CALLBACK(AorD),&AcceptorDecline);*/ 

	/*fixed = gtk_fixed_new() ; 
	gtk_fixed_put(GTK_FIXED(fixed), btn1, 50, 100);
	gtk_fixed_put(GTK_FIXED(fixed), btn2 , 70,100);*/

		
	
	gtk_widget_show_all(window); 
	gtk_main(); 
	return 0;

}

void AorD (GtkButton *button, void *user_data)
{
	if (AcceptorDecline == 0)
	{
		g_print("Decline"); 
	}
	else if (AcceptorDecline == 1)
	{
		g_print("Accept"); 
	}
	
}

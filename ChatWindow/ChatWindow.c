#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>


#define MSG_LEN 1024
GtkWidget *send_view;
GtkWidget *receive_view; 
GtkWidget *send_scrolled_window;
GtkWidget *receive_scrolled_window; 
GtkTextBuffer *sendbuffer;
GtkTextBuffer *receivebuffer;
GtkWidget *msg;
GtkTextMark *mark; 
GtkTextIter iter; 
GtkTextIter end;


static gboolean on_delete_event (GtkWidget *widget, GdkEvent  *event, gpointer data);
static void send_message(GtkWidget *widget,GtkWidget *entry); 
int main(int argc, char *argv[])
{
		gtk_init(&argc,&argv);
		GtkWidget *ChatWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL); 
		gtk_widget_set_size_request(ChatWindow,420,400); 
		gtk_window_set_title (GTK_WINDOW(ChatWindow),"Chat");
		gtk_window_set_position (GTK_WINDOW(ChatWindow), GTK_WIN_POS_CENTER);  
		gtk_window_set_resizable(GTK_WINDOW(ChatWindow), FALSE) ;
		
		GtkWidget *SendButton = gtk_button_new_with_label("Send"); 
		gtk_widget_set_size_request(SendButton, 75, 40);

	
	
		msg = gtk_entry_new(); 
		gtk_entry_set_max_length(GTK_ENTRY(msg),500); 
		gtk_widget_set_size_request(msg, 330, 40);
		gtk_widget_show(msg);
		
		
		
		
		send_view = gtk_text_view_new_with_buffer (sendbuffer);
		gtk_widget_set_size_request(send_view,50,300); 
		gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (send_view), GTK_WRAP_WORD); 
		send_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (send_scrolled_window), 
                                  GTK_POLICY_AUTOMATIC, 
                                  GTK_POLICY_AUTOMATIC); 
								  
		gtk_text_view_set_editable(GTK_TEXT_VIEW(send_view), FALSE);

		gtk_container_add (GTK_CONTAINER (send_scrolled_window), 
                                         send_view);
		gtk_container_set_border_width (GTK_CONTAINER (send_scrolled_window), 1);
 
  
		sendbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (send_view));
	/************************************Receive*****************************************/

		receive_view = gtk_text_view_new_with_buffer(receivebuffer); 
		gtk_widget_set_size_request(receive_view,200,300); 
		gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(receive_view),GTK_WRAP_WORD); 
		receive_scrolled_window = gtk_scrolled_window_new(NULL,NULL); 
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (receive_scrolled_window), 
                                  GTK_POLICY_AUTOMATIC, 
                                  GTK_POLICY_AUTOMATIC); 
		
		gtk_text_view_set_editable(GTK_TEXT_VIEW(receive_view),FALSE); 
		gtk_container_add (GTK_CONTAINER(receive_scrolled_window),receive_view);
		gtk_container_set_border_width(GTK_CONTAINER(receive_scrolled_window),1); 
		
		receivebuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(receive_view)); 
		
		
		
		GtkWidget *fixed  = gtk_fixed_new();
		gtk_fixed_put(GTK_FIXED(fixed),send_scrolled_window,5,40);
		gtk_fixed_put(GTK_FIXED(fixed),receive_scrolled_window, 70,40); 
		gtk_fixed_put(GTK_FIXED(fixed),msg,5,350); 
		gtk_fixed_put(GTK_FIXED(fixed),SendButton,335,350); 
		gtk_container_add(GTK_CONTAINER(ChatWindow),fixed); 
		
		g_signal_connect(ChatWindow, "delete_event", G_CALLBACK(on_delete_event), NULL) ;
		g_signal_connect(msg,"activate",G_CALLBACK(send_message),msg); 
		gtk_widget_show_all(ChatWindow); 
	 
		gtk_main();
		return 0; 

}
static gboolean on_delete_event (GtkWidget *widget, GdkEvent  *event, gpointer data)
{
	printf("delete event occurred\n");
	gtk_main_quit();
	return TRUE;
}

static void send_message(GtkWidget *widget,GtkWidget *entry)
{
		
		
		const gchar *entry1;
		entry1 = gtk_entry_get_text(GTK_ENTRY(msg));
		mark = gtk_text_buffer_get_insert(receivebuffer); 
		gtk_text_buffer_get_iter_at_mark(receivebuffer,&iter,mark);
		
		gtk_text_buffer_get_end_iter(sendbuffer,&end); 
		
		if(gtk_text_buffer_get_char_count(receivebuffer))
		{
			gtk_text_buffer_insert(receivebuffer,&iter,"\n",1);
		}
		/*gtk_text_buffer_set_text (buffer, entry1, -1);*/
		gtk_text_buffer_insert(receivebuffer,&iter,entry1,-1); 
		gtk_text_buffer_insert (sendbuffer, &end, "You:\n",-1);
		gtk_entry_set_text(GTK_ENTRY(msg),""); 
	
}
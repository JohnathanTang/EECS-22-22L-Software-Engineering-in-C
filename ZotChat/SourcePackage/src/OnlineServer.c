#define _POSIX_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

#define DEBUG
#define STR_LEN 64
#define MSG_LEN 1024
#define max_users 10

int MakeServerSocket(int portno);
int VerifyUserName(char* username);
int VerifyPassword(char* username, char* password);
void PrintScanOptions(int* option, char* buffer);
void AppendUser(char* username, char* password);
int LoginUser(int client_fd, int message_size, char client_username[10][64], int max_client);
int ChatRecieve(int client_fd, int message_size, char client_username[10][64], char* message, int client_fds[10]);
int AddFriend (int client_fd, int message_size, char client_username[10][64],int client_fds[10]);
int DeleteFriendClient (int client_fd, int message_size, char client_username[10][64],int client_fds[10]);
int PullUserFriends(int client_fd, int message_size, char client_username[10][64],int client_fds[10]);
int ChangeUserPassword (int client_fd, int message_size, char client_username[10][64],int client_fds[10]);
int FindClient(int client_fd, int client_fds[10]);
int CheckOnline(char sent_user[64], char client_username[10][64]);
int AlreadyFriend(char* username, char* friend);
void RegisterUser(int client_fd, int message_size, char client_username[10][64], int client_fds[10]);
int AppendFriend(char* username, char* friend);
void PrintOnline(int client_fds[10], char client_username[10][64]);
int LogOutUser(int client_fd, int client_fds[10],char client_username[10][64]);
int DeleteAllUsername(char* your_name, char* find);
void DeleteFriend(char* path, char* name);
void ChangePassword(char *new_password, char* username);
int NumFriends(char *username);
int SendStatus(char* username, char* status, int fd);
int SendAllFriendStatus(char* status, int sender_fd, int client_fds[10], char client_username[10][64]);
int Exit = 0;

int main(int argc, char *argv[])
{
    int serversocket_fd, client_fd;
    int portno;
    struct sockaddr_in client;
	socklen_t client_size;
	
	
    char* program_name = argv[0];
#ifdef DEBUG
    printf("Number of Argc : %d \n", argc);
    printf("%s: Starting...\n", program_name);
    int index = 0;
    for(index= 1; index <argc; index++) {
        printf("The %d argument is : %s \n", index,argv[index]);
    }
#endif
    
    /*Autobinds port to 5050 if not defined */
    /*If defined then define to port number set */
    if(argc < 2 ) {
        printf("Autobinding port to #5050 \n");
        portno = 5050;
    } else {
        portno = atoi(argv[1]);
        if(portno <= 2000) { fprintf(stderr,"Port Number should be greater than 2000. Closing\n"); return 0;}
    }
    
	client_size = sizeof(client);
	
	/*make, listen, and bind server */
	
    serversocket_fd = MakeServerSocket(portno);
	
	/*reuse socket*/
	int reuse = 1;
	setsockopt(serversocket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
	
	fd_set active_fds, read_fds;
//	int max_users=10;
	int client_fds[max_users];	memset(client_fds, 0, sizeof(client_fds));
	
	char client_username[max_users][STR_LEN];
	memset(client_username, 0, sizeof(client_username)); 
	int select_res, i;

	
		/*trying to get some messages from the client */
	int w_or_r; /*integer to see if there is an invalid output or input from write or read */
	char buffer[MSG_LEN];
	char command[MSG_LEN];
	memset(buffer,'\0',MSG_LEN);
	memset(command,'\0',MSG_LEN);
	
	
	
    FD_ZERO(&active_fds);   /*empty all file descriptors resets every use*/
	FD_SET(serversocket_fd, &active_fds);   /*add server */ 
	FD_SET(STDIN_FILENO, &active_fds);
	struct timeval timeout;
	pid_t pid;
    while(!Exit){   
		if ((pid = fork()) == 0)  {
			printf("Hello from Child!\n");  
			while(1) {
				int stuff;
				for(stuff = 0; stuff < max_users; stuff++) {
					send(client_fds[stuff] , "$refresh" , strlen("$refresh") , 0 ); 
					
				}
				sleep(5);
			}
		}else {
			printf("Hello from Parent!\n"); 
		}
	
	
	
		/*lets start reading all the connected fds*/
		read_fds = active_fds;
     
        //wait for an activity on one of the sockets , timeout is 15 seconds
		timeout.tv_sec = 120;
		timeout.tv_usec = 0;
		
		
        select_res = select( FD_SETSIZE , &read_fds , NULL , NULL , &timeout);   
       
        if ((select_res < 0)){ perror("select error"); exit(10);
        } else if(select_res == 0 ) {   
			kill(pid, SIGKILL);
			close(serversocket_fd);
			perror("we timed out bois"); 

			exit(10);
		}
		
		if(FD_ISSET(STDIN_FILENO,&read_fds)) { 
			memset(command,'\0',MSG_LEN);
			fgets(command, MSG_LEN-1, stdin);
#ifdef DEBUG
			printf("\nyou typed in %s \n \n", command);
#endif		
			if( strcmp(command,"ONLINE\n") ==0) {
				PrintOnline(client_fds,client_username);		
			}
		}
		/* new connection to server */
        if (FD_ISSET(serversocket_fd, &read_fds))   
        {   
			/*accept signals */
			client_fd = accept(serversocket_fd, (struct sockaddr*)&client, &client_size);
	
			if(client_fd < 0) { perror("did not accept client"); exit(1);} else 
				printf("server accepted new client \n");
				
			/*print the deets */
			printf("New client ip: %s:%hu.\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
			
             
            if( send(client_fd, "Hello", 5, 0) != strlen(buffer) )   
            {   
                perror("send");   
            }   
#ifdef DEBUG                
            puts("Welcome message sent successfully");   
#endif                
			/*add to the set of file descriptors checked next cycle */
			FD_SET(client_fd, &active_fds);
			
        }   
             
        /* some existing socket */
        for (i = 0; i < FD_SETSIZE; ++i) {
                        
            if (FD_ISSET( i , &read_fds))   
            {   
				if(( i != serversocket_fd) && (i!=STDIN_FILENO)){
					/*reads and puts in buffer */
					
					w_or_r = recv(i, buffer, sizeof(buffer), 0); 
					
					/*if 0 then the user disconnected*/
					if(w_or_r == 0) {
						getpeername(i , (struct sockaddr*)&client , &client_size);   
						printf("A user disconnected. The ip: %s. The port :%d \n" ,  inet_ntoa(client.sin_addr) , ntohs(client.sin_port));   
                         
						/************************SHOULD ALSO CLEAR CLIENT FDS  IN ARRAY AND NAME***********************/
						SendAllFriendStatus("offline", i, client_fds,client_username);
						LogOutUser(i,client_fds,client_username);

						close( i );
						FD_CLR(i, &active_fds);					   
					} else {
						/*this is something the client sent to the server */
					
						

						
						printf("The string recieved was %s \n",buffer);  
						
						
						/*Parses string depending on what is recieved, if it not a command then it is just an echo to the server */
						
						/*TIME TO PARSE STRING */
						if( strcmp(buffer,"$login") ==0) {
							int fd_place_holder;
#ifdef DEBUG
							printf("Recieved $Login String! \n"); 
#endif
							send(i , buffer , strlen(buffer) , 0 );           /*send login back */
		
							fd_place_holder = LoginUser(i, sizeof(buffer) ,client_username, max_users); 
							if(fd_place_holder != -1) {
								client_fds[fd_place_holder] = i;
#ifdef DEBUG
								printf("Client File Descriptor has now also been updated in location : %d \n", fd_place_holder);
#endif	
							}
							SendAllFriendStatus("online", i, client_fds,client_username);
/*************************************************** LOT OF STUFF TO SUPPORT ***********************************************************/
						} else if(strcmp(buffer,"$register") == 0) { 
							printf("Recieved $register String! \n");
						
							send(i , buffer , strlen(buffer) , 0 );           /*send register back */
						
							RegisterUser(i, sizeof(buffer), client_username, client_fds);
						
						}else if(strcmp(buffer,"$chat") == 0) {
#ifdef DEBUG
							printf("Recieved $Chat String! \n"); 
#endif							
							int fd_location;
							char message[MSG_LEN];
							memset(message,'\0',MSG_LEN);
							send(i , buffer , strlen(buffer) , 0 );           /*send $chat back */
							
							fd_location = ChatRecieve(i, sizeof(buffer), client_username, message, client_fds);
							
							
							
						} else if ( strcmp(buffer,"$addfriend") == 0) { 
#ifdef DEBUG
							printf("Recieved $addfriend String! \n"); 
#endif							
							send(i , buffer , strlen(buffer) , 0 );           /*send $addfriend back */
							
							AddFriend(i, sizeof(buffer), client_username, client_fds);
							
						} else if (strcmp(buffer,"$logout") == 0) {
#ifdef DEBUG
							printf("Recieved $logout String! \n"); 
#endif
							SendAllFriendStatus("offline", i, client_fds,client_username);
							int temp = LogOutUser(i,client_fds, client_username);
							if(temp != max_users) {
								send(i , "successfully logged out\n" , strlen("successfully logged out\n") , 0 );
							} else {
								send(i , "never logged in\n" , strlen("never logged in\n") , 0 );
							}
						} else if (strcmp(buffer,"$deletefriend") == 0) {
#ifdef DEBUG
							printf("Recieved $deletefriend String! \n"); 
#endif							
							send(i , buffer , strlen(buffer) , 0 );           /*send $deletefriend back */
							
							DeleteFriendClient(i, sizeof(buffer), client_username, client_fds);
						} else if (strcmp(buffer,"$changepassword") == 0) {
#ifdef DEBUG
							printf("Recieved $changepassword String! \n"); 
#endif							
							send(i , buffer , strlen(buffer) , 0 );           /*send $changepassword back */
							
							ChangeUserPassword(i, sizeof(buffer), client_username, client_fds);
						} else if (strcmp(buffer,"$pull") == 0) {
#ifdef DEBUG
							printf("Recieved $pull String! \n"); 
#endif
							//send(i , buffer , strlen(buffer) , 0 );   /*send $pull back */

							PullUserFriends(i, sizeof(buffer), client_username, client_fds);
							if(0 == 0 ){
								printf("I got here\n");
							}
						}else {
							/*ECHOES*/
							
							/*makes last char null */
							if(buffer[0] != '\n') {
								buffer[strcspn(buffer, "\r\n")] = 0;
							}
							send(i , buffer , strlen(buffer) , 0 );   
							
						}
						/*Resets buffer to recieve a new message after every message */
						memset(buffer,'\0',MSG_LEN);
						
						
					}
				}
			
            }   
        
		}


		kill(pid, SIGKILL);
		/*end of while loop*/
    }   	
	
	
	
	
	
	
	

	close(serversocket_fd);
	
	
	

    return 0;
}
void RegisterUser(int client_fd, int message_size, char client_username[10][64], int client_fds[10]) {
	char username[STR_LEN] = {0};
	char password[STR_LEN] = {0};
	char discard[MSG_LEN] = {0};
	int username_found = 0;
	

		recv(client_fd, username, message_size, 0);       /*get username */             //recv(client_fd, username, sizeof(buffer), 0); 
		send(client_fd , username , strlen(username) , 0 ); /*send username back */
		recv(client_fd, password, message_size, 0);       /*get password */             //recv(client_fd, password, sizeof(buffer), 0); 
		//never need this				                                                 //send(i , password , strlen(password) , 0 ); /*send password back */
#ifdef DEBUG
		printf("username recieved : %s \n", username);      /*THESE PRINT STATEMENTS STALL */
		printf("password recieved : %s \n", password);      /*THESE PRINT STATEMENTS STALL */
#endif
	
	username_found = VerifyUserName(username);
	if(username_found) {
#ifdef DEBUG
		printf("Username already exists! Cannot Register \n \n \n ");
#endif
		write(client_fd, "$register", strlen("$register"));
		read(client_fd, discard, MSG_LEN);
		write(client_fd, "failed", strlen("failed"));
		read(client_fd, discard, MSG_LEN);
		write(client_fd, "Username already taken\n", strlen("Username already taken\n"));
		return;
	} else if ((strcmp(username,"admin") ==0 ) || (strcmp(username,"replica") ==0 )) {
		write(client_fd, "$register", strlen("$register"));
		read(client_fd, discard, MSG_LEN);
		write(client_fd, "failed", strlen("failed"));
		read(client_fd, discard, MSG_LEN);
		write(client_fd, "Invalid Username\n", strlen("Invalid Username\n"));
		return;
		
	}
	AppendUser(username, password);
	write(client_fd, "$register", strlen("$register"));
	read(client_fd, discard, MSG_LEN);
	write(client_fd, "success", strlen("success"));
	read(client_fd, discard, MSG_LEN);
	write(client_fd, "Successful. Please Login\n", strlen("Successful. Please Login\n"));
	return;
}

/*friend does not have to be online to be deleted and uh needs to be updated to tell the other person offline  */
int DeleteFriendClient(int client_fd, int message_size, char client_username[10][64],int client_fds[10]) {
	char friend[STR_LEN] = {0};
	char discard[MSG_LEN] = {0};
		recv(client_fd, friend, message_size, 0);         /*get friend username */ 
#ifdef DEBUG
	printf("the soon deleted friend username is: %s \n", friend);
#endif	
	int client_index = FindClient(client_fd, client_fds);
	
	char user[STR_LEN] = {0};
	strcpy(user,client_username[client_index]);
	int valid_name = DeleteAllUsername(user, friend);
	if(valid_name == 0) {
		printf("Not already a friend! \n");
		write(client_fd, "$deletefriend", strlen("$addfriend"));                             /*send $deletefriend */
		read(client_fd, discard, MSG_LEN);                                                /*recv */
		write(client_fd, "failed", strlen("failed"));                                     /*send fail */
		read(client_fd, discard, MSG_LEN);                                                /*recv*/
		write(client_fd, "That person not on your friendlist\n", strlen("That person not on your friendlist\n")); /*send reason */
		
		return -1;
	}
	write(client_fd, "$deletefriend", strlen("$deletefriend"));                             /*send $deletefriend */
	read(client_fd, discard, MSG_LEN);                                                /*recv */
	write(client_fd, "success", strlen("success"));                                     /*send  */
	read(client_fd, discard, MSG_LEN);                                                /*recv*/
	write(client_fd, "Friend Deleted\n", strlen("Friend Deleted\n")); /*send reason */
	
	int recv_index = CheckOnline(friend,client_username);
	if(recv_index != -1 ) {
		SendStatus(friend, "offline",client_fd);
		SendStatus(user, "offline",client_fds[recv_index]);
	} 
	return 1 ;
	
}

/*if we deleted all the instances return 1, else if not found return -1 */
/*calls already friend to see if they are already a friend, if they are not a friend
then there is nothing to delete */
/*if they are a friend, then open new text called replica.c copy the contents beside that name, then destroy old file and rename */

int DeleteAllUsername(char* your_name, char* find){
#ifdef DEBUG
    printf("This is the username passed in DeleteAllUsername : %s \n", your_name);
	printf("This is the friend passed in DeleteAllUsername : %s \n", find);
#endif
	int on_list;
	
	on_list = AlreadyFriend(your_name, find);
	if(on_list == -1 ) { /* not already friends and thus do not need delete */
#ifdef DEBUG
		printf("you cannot delete people who you are not friends with!");
#endif	
		return -1;
	}

	/* Need to Delete the friend from both lists */
	DeleteFriend(your_name,find);
	DeleteFriend(find,your_name);
	

	return 1;
}

/* path is the person's friend list, and deleting name from it */
void DeleteFriend(char* path, char* name) {
	FILE *fp, *fp2;

	/*changes file path*/
	char currentpath[256] = "./Users/";
	char filetype[STR_LEN] = ".txt";
	char duplicate[256] = "./Users/replica"; 
	char line[256] = {0};
	strcat(currentpath,path);
	strcat(currentpath,filetype);
	
	fp = fopen(currentpath,"a+");	
	fp2 = fopen(duplicate,"w+");
	
 /*password */
	fgets(line, sizeof(line), fp); 
	fprintf(fp2,"%s",line);
	memset(line,'\0',STR_LEN);
 /*friendlist */
	fgets(line, sizeof(line), fp); 
	fprintf(fp2,"%s",line);
	memset(line,'\0',STR_LEN);
	
	
	char name_nl[STR_LEN] = {0};
	
	strcat(name_nl,name);
	strcat(name_nl,"\n");
	while (fgets(line, sizeof(line), fp)) {
		if(strcmp(line, name_nl) == 0) {
			memset(line,'\0',STR_LEN);

		} else {
			fprintf(fp2,"%s",line);
			memset(line,'\0',STR_LEN);
		}
	}
	
	
	fclose(fp2);
	fclose(fp);	
	
	remove(currentpath);
	rename(duplicate, currentpath);	
	
}




/*ret -1 if failed because offline or already on friend list else 1 and appends to both lists */
int AddFriend (int client_fd, int message_size, char client_username[10][64],int client_fds[10]) {
	char friend[STR_LEN] = {0};
	char discard[MSG_LEN] = {0};
	int client_index;
	int recv_index;
		recv(client_fd, friend, message_size, 0);         /*get friend username */ 
		
	
#ifdef DEBUG
	printf("the friend username is: %s \n", friend);
#endif

	client_index = FindClient(client_fd, client_fds);
	
	char user[STR_LEN] = {0};
	strcpy(user,client_username[client_index]);
	
	recv_index = CheckOnline(friend,client_username);
	if(recv_index == -1){
#ifdef DEBUG
		printf(" the user who you are trying to send to is offline! \n");
#endif	
		write(client_fd, "$addfriend", strlen("$addfriend"));                             /*send $addfriend */
		read(client_fd, discard, MSG_LEN);                                                /*recv */
		write(client_fd, "failed", strlen("failed"));                                     /*send fail */
		read(client_fd, discard, MSG_LEN);                                                /*recv*/
		write(client_fd, "That person is offline\n", strlen("That person is offline\n")); /*send reason */
		
		
		return -1;
	}
	/*********** ALSO MAKE SURE THE PERSON IS NOT ALREADY ON FRIEND LIST, else return error; If person is not already on friend list then add  ******/
	int valid_add = AppendFriend(user,friend);
	if(valid_add == -1){
#ifdef DEBUG
		printf(" the user who you are trying to send to is offline! \n");
#endif	
		write(client_fd, "$addfriend", strlen("$addfriend"));                             /*send $addfriend */
		read(client_fd, discard, MSG_LEN);                                                /*recv */
		write(client_fd, "failed", strlen("failed"));                                     /*send fail */
		read(client_fd, discard, MSG_LEN);                                                /*recv*/
		write(client_fd, "tried adding yourself as friend or already on friend list\n", strlen("tried adding yourself as friend or already on friend list\n")); /*send reason */
		
		
		return -1;
	}
	int friend_num_friends = NumFriends(user);
	int user_num_friends = NumFriends(friend);
	if(friend_num_friends == 15){
#ifdef DEBUG
		printf(" max friends for friend  \n");
#endif	
		write(client_fd, "$addfriend", strlen("$addfriend"));                             /*send $addfriend */
		read(client_fd, discard, MSG_LEN);                                                /*recv */
		write(client_fd, "failed", strlen("failed"));                                     /*send fail */
		read(client_fd, discard, MSG_LEN);                                                /*recv*/
		write(client_fd, "That user has the maximum amount of friends: 15\n", strlen("That user has the maximum amount of friends: 15\n")); /*send reason */
		return -1;
	} else if (user_num_friends == 15) {
#ifdef DEBUG
		printf(" max friends for user  \n");
#endif	
		write(client_fd, "$addfriend", strlen("$addfriend"));                             /*send $addfriend */
		read(client_fd, discard, MSG_LEN);                                                /*recv */
		write(client_fd, "failed", strlen("failed"));                                     /*send fail */
		read(client_fd, discard, MSG_LEN);                                                /*recv*/
		write(client_fd, "You have maximum number of friends: 15\n", strlen("You have maximum number of friends: 15\n")); /*send reason */
		return -1;	

	}
	write(client_fd, "$addfriend", strlen("$addfriend"));                             /*send $addfriend */
	read(client_fd, discard, MSG_LEN);                                                /*recv */
	write(client_fd, "success", strlen("success"));                                     /*send  */
	read(client_fd, discard, MSG_LEN);                                                /*recv*/
	write(client_fd, "Say hi to your new friend\n", strlen("Say hi to your new friend\n")); /*send reason */
	
	/***** NOW MUST SEND MESSAGE TO OTHER PERSON LETTING THEM KNOW  *******/
	write(client_fds[recv_index], "$addfriend", strlen("$addfriend"));                             /*send $addfriend */
	read(client_fds[recv_index], discard, MSG_LEN);                                                /*recv */
	write(client_fds[recv_index], user, strlen(user));                                     /*send  */
	read(client_fds[recv_index], discard, MSG_LEN);                                                /*recv*/
	write(client_fds[recv_index], "Added you as a friend\n", strlen("Added you as a friend\n")); /*send reason */
	
	/***** sending online messages to both of them ************/
	SendStatus(friend, "online",client_fd);
	SendStatus(user, "online",client_fds[recv_index]);
	return 0;
}
/*return index of online or -1 for offline */
int CheckOnline(char sent_user[64], char client_username[10][64]){
	int i = 0;
	for(i = 0; i <max_users; i++) {
		if(strcmp(sent_user,client_username[i]) == 0 ) {
			return i;
		}
	}
	return -1;
}
/*return client index or -1 for offline */
int FindClient(int client_fd, int client_fds[10]) {
	int i =0;
	for(i =0; i < max_users; i ++ ) { 
		if(client_fd == client_fds[i]) {
			return i;
		}
	}
	return -1;
}
/* should be logged in to change password */
int ChangeUserPassword (int client_fd, int message_size, char client_username[10][64],int client_fds[10]) {
	char old_pass[STR_LEN] = {0};
	char new_pass[STR_LEN] = {0};
	char discard[MSG_LEN] = {0};
		recv(client_fd, old_pass, message_size, 0);         /*get old password */   
		send(client_fd , old_pass , strlen(old_pass) , 0 ); /*send oldpassword back */		
		recv(client_fd, new_pass, message_size, 0);         /*get new password */
#ifdef DEBUG	
		printf("old password is : %s", old_pass);
		printf("new password is :%s", new_pass);
#endif	
	int index = FindClient(client_fd,client_fds);
 
	if(index == -1) {
		write(client_fd, "$changepassword", strlen("$changepassword"));
		read(client_fd, discard, MSG_LEN);
		write(client_fd, "failed", strlen("failed"));
		read(client_fd, discard, MSG_LEN);
		write(client_fd, "User offline\n", strlen("User offline\n"));
		return -1;
	}
	char user[STR_LEN] = {0};
	strcpy(user,client_username[index]);
	int pass_found = VerifyPassword(user,old_pass);
	
	if(pass_found != 1) {
		write(client_fd, "$changepassword", strlen("$changepassword"));
		read(client_fd, discard, MSG_LEN);
		write(client_fd, "failed", strlen("failed"));
		read(client_fd, discard, MSG_LEN);
		write(client_fd, "Wrong Password\n", strlen("WrongPassword\n"));
		return -1;
	}

	ChangePassword(new_pass, user);
		
	write(client_fd, "$changepassword", strlen("$changepassword"));                             /*send $changepassword */
	read(client_fd, discard, MSG_LEN);                                                /*recv */
	write(client_fd, "success", strlen("success"));                                     /*send  */
	read(client_fd, discard, MSG_LEN);                                                /*recv*/
	write(client_fd, "Password updated for next login\n", strlen("Password updated for next login\n")); /*send reason */	
		
		
	return 0;	
}
void ChangePassword(char *new_password, char* username) {
	FILE *fp, *fp2;

	/*changes file path*/
	char currentpath[STR_LEN] = "./Users/";
	char filetype[STR_LEN] = ".txt";
	char duplicate[256] = "./Users/replica"; 
	char line[256] = {0};
	strcat(currentpath,username);
	strcat(currentpath,filetype);
	
	fp = fopen(currentpath,"a+");	
	fp2 = fopen(duplicate,"w+");
	
	fprintf(fp2,"%s\n",new_password);
	fgets(line, sizeof(line), fp); memset(line,'\0',STR_LEN);
	while (fgets(line, sizeof(line), fp)) {
	
		fprintf(fp2,"%s",line);
		memset(line,'\0',STR_LEN);
	}
	
	
	fclose(fp2);
	fclose(fp);	
	
	remove(currentpath);
	rename(duplicate, currentpath);
}
int ChatRecieve(int client_fd, int message_size, char client_username[10][64], char* message, int client_fds[10]) {
	char reciever[STR_LEN] = {0};
	char discard[MSG_LEN] = {0};
	
		recv(client_fd, reciever, message_size, 0);         /*get reciever */   
		send(client_fd , reciever , strlen(reciever) , 0 ); /*send reciever back */		
		recv(client_fd, message, message_size, 0);         /*get message */

#ifdef DEBUG
		for(int x = 0; x<STR_LEN; x++ ) {
			if(reciever[x] =='\n') {
				printf(" somehow i copied a newline !");
			}
		}
		printf("The reciever sent to is: %s \n", reciever);
		printf("The message is: %s \n", message);
#endif
	/****************************************if user if offline, not on friend list, else successful  *************************/
	int online_index = CheckOnline(reciever, client_username);
	int sender_index = FindClient(client_fd, client_fds);
	char sender_name[STR_LEN] = {0};
	strcpy(sender_name, client_username[sender_index]);
	int already_friend = AlreadyFriend(sender_name, reciever);
	
#ifdef DEBUG
	printf("The sender is: %s \n", sender_name);
	printf("The index he is at is: %d \n", sender_index);
	printf("Reciever is: %d (shows the index which they online, -1 for offline) \n", online_index);
	printf("AlreadyFriend is: %d (1 for already a friend, -1 for not a friend) \n", already_friend);
#endif
	/**************** PERSON NEEDS TO BE ABLE TO CHAT WITH THEM SELF ******************************/
	if(strcmp(sender_name,reciever) == 0) {
#ifdef DEBUG
	printf("I should be able to talk to myself \n");
#endif
	} else {
		/*person is not a friend */
		if(already_friend == -1) {
			write(client_fd, "$chat", strlen("$chat"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "failed", strlen("failed"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "Not your friend. Try $addfriend\n", strlen("Not your friend. Try $addfriend\n"));
			return -1;
		}
	/*person receiving is offline */
		if(online_index == -1) {
			write(client_fd, "$chat", strlen("$chat"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "failed", strlen("failed"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "The person you are trying to access if offline\n", strlen("The person you are trying to access if offline\n"));
			return -1;
		}
	}
	/** person is online and a friend, tell the user it was a success **/
	write(client_fd, "$chat", strlen("$chat"));
	read(client_fd, discard, MSG_LEN);
	write(client_fd, "success", strlen("success"));
	read(client_fd, discard, MSG_LEN);
	write(client_fd, "Chat sent successfully\n", strlen("Chat sent successfully\n"));
	
	/** time to write to the other person that a message was sent  **********/
	write(client_fds[online_index], "$chat", strlen("$chat"));                             /*send $chat */
	read(client_fds[online_index], discard, MSG_LEN);                                                /*recv */
	write(client_fds[online_index], sender_name, strlen(sender_name));                                     /*send  */
	read(client_fds[online_index], discard, MSG_LEN);                                                /*recv*/
	write(client_fds[online_index], message, strlen(message));                  /*send message */
	return 1;
}
/*** returns -1 if not successful returns 1 if successfully added ****/
int AppendFriend(char* username, char* friend){ 
#ifdef DEBUG
    printf("This is the username passed in AppendFriend : %s \n", username);
	printf("This is the friend passed in AppendFriend : %s \n", friend);
#endif

	FILE *fp, *fp2;

	/*changes file path*/
	char currentpath[256] = "./Users/";
	char filetype[STR_LEN] = ".txt";
	char line[256] = {0};
	strcat(currentpath,username);
	strcat(currentpath,filetype);
	
	/*./Users/username.txt is opened */
	fp2 = fopen(currentpath,"a+");
	while (fgets(line, sizeof(line), fp2)) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
#ifdef DEBUG
        printf("%s", line);
#endif	
		if( line[0] !='\n') {
			line[strcspn(line, "\r\n")] = 0; /* if last char  == '\0' then replaces again with null; this is not met if first char is new line because then it would be an empty string */
		} 
		if( strcmp(line,".friendlist") == 0 ){
			memset(line,'\0',STR_LEN);
#ifdef DEBUG
			printf("I found friendlist! \n");
#endif
			break;
		}
		memset(line,'\0',STR_LEN);
    }
	
	/*now checking after friendslist */
#ifdef DEBUG
        printf("Checking Username List for that friend \n");
#endif	

	if(strcmp(friend,username)==0 ) {
#ifdef DEBUG
		printf("you cannot add yoruself");
#endif
		return -1;
	}
	while (fgets(line, sizeof(line), fp2)) {
#ifdef DEBUG
        printf("%s", line);
#endif	
		if( line[0] !='\n') {
			line[strcspn(line, "\r\n")] = 0; /* if last char  == '\0' then replaces again with null; this is not met if first char is new line because then it would be an empty string */
		} 
		if(strcmp(line,friend) == 0 ) {
			printf("friend already added \n");
			fclose(fp2);
			return -1;
		}
		memset(line,'\0',STR_LEN);
	}
	
	/***** Does not have that friend ****/
	
	/*username addds the friend ***/
	fprintf(fp2,"%s\n",friend);
	
	char currentpath2[256] = "./Users/";
	char filetype2[STR_LEN] = ".txt";
	strcat(currentpath2,friend);
	strcat(currentpath2,filetype2);
	fp = fopen(currentpath2,"a+");
	
	/*friend adds the username */
	
	fprintf(fp,"%s\n",username);
	fclose(fp2);
	fclose(fp);




	return 1;
}

/* returns 1 if you are already friends */
int AlreadyFriend(char* username, char* friend) {
#ifdef DEBUG
    printf("This is the username passed in AppendFriend : %s \n", username);
	printf("This is the friend passed in AppendFriend : %s \n", friend);
#endif

	FILE *fp2;

	/*changes file path*/
	char currentpath[256] = "./Users/";
	char filetype[STR_LEN] = ".txt";
	char line[STR_LEN] = {0};
	strcat(currentpath,username);
	strcat(currentpath,filetype);
	
	/*./Users/username.txt is opened */
	fp2 = fopen(currentpath,"a+");
	while (fgets(line, sizeof(line), fp2)) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
#ifdef DEBUG
        printf("%s", line);
#endif	
		if( line[0] !='\n') {
			line[strcspn(line, "\r\n")] = 0; /* if last char  == '\0' then replaces again with null; this is not met if first char is new line because then it would be an empty string */
		} 
		if( strcmp(line,".friendlist") == 0 ){
			memset(line,'\0',STR_LEN);
#ifdef DEBUG
			printf("I found friendlist! \n");
#endif
			break;
		}
		memset(line,'\0',STR_LEN);
    }
	
	/*now checking after friendslist */
#ifdef DEBUG
        printf("Checking Username List for that friend \n");
#endif	


	while (fgets(line, sizeof(line), fp2)) {
#ifdef DEBUG
        printf("%s", line);
#endif	
		if( line[0] !='\n') {
			line[strcspn(line, "\r\n")] = 0; /* if last char  == '\0' then replaces again with null; this is not met if first char is new line because then it would be an empty string */
		} 
		if(strcmp(line,friend) == 0 ) {
			printf("friend already added \n");
			fclose(fp2);
			return 1;
		}
		memset(line,'\0',STR_LEN);
	}	
	
	fclose(fp2);
	return -1;
}
/*login will return the location where the person will slot in the array, if there are max users, already logged in, wrong username or password, this will return -1 */
int LoginUser(int client_fd, int message_size, char client_username[10][64], int max_client) {
	char username[STR_LEN] = {0};
	char password[STR_LEN] = {0};
	char discard[MSG_LEN] = {0};
	int username_found = 0, password_found = 0;
	

		recv(client_fd, username, message_size, 0);       /*get username */             //recv(client_fd, username, sizeof(buffer), 0); 
		send(client_fd , username , strlen(username) , 0 ); /*send username back */
		recv(client_fd, password, message_size, 0);       /*get password */             //recv(client_fd, password, sizeof(buffer), 0); 
		//never need this				                                                 //send(i , password , strlen(password) , 0 ); /*send password back */
#ifdef DEBUG
		printf("username recieved : %s \n", username);      /*THESE PRINT STATEMENTS STALL */
		printf("password recieved : %s \n", password);      /*THESE PRINT STATEMENTS STALL */
#endif
							
	/*now has the username and password */	
	
	username_found = VerifyUserName(username);
	if(username_found) {
		password_found = VerifyPassword(username, password);
	}
#ifdef DEBUG
	printf("\n \n password found : %d",password_found);
#endif
		if((username_found == 1) && (password_found ==1)) {
			/*both username and password are found; client should be ready to login unless he is already logged in*/
#ifdef DEBUG
			printf("both username and password found \n");
#endif
			/*CHECK IF HE IS ALREADY LOGGED IN */
			int j = 0; 
			int already_logged_in = 0;
			int full = 0;
			for(j=0;j< max_client; j++) {
				if(strcmp(username,client_username[j]) == 0 ) {
					already_logged_in = 1;
#ifdef DEBUG
					printf("user already logged in at position : %d \n", j); 
					printf(" already logged in value is now :%d \n", already_logged_in);
#endif
				break;
				}
			}
			
			/*if not already logged in */
			
			if(!already_logged_in) {
				/*add the user, unless full */
				for(j=0;j< max_client; j++) {
					if(strcmp("",client_username[j]) == 0 ) {
						strcpy(client_username[j], username);
#ifdef DEBUG
					printf("user appended at position in array : %d \n", j); 
					printf("user: '%s' is now logged in \n", username);
#endif
						break; /*only executes once */
					}
					if(j == (max_client-1)) {
#ifdef DEBUG
						printf("server is full");
#endif
						full = 1;
					}
				}
				
				/*return not logged in, max users */
				if(full) {
					write(client_fd, "$login", strlen("$login"));
					read(client_fd, discard, MSG_LEN);
					write(client_fd, "failed", strlen("failed"));
					read(client_fd, discard, MSG_LEN);
					write(client_fd, "max users. please wait and log in later\n", strlen("max users. please wait and log in later\n"));
					return -1;
				}
				
				/*Client actually logs in */
				
				write(client_fd, "$login", strlen("$login"));
				read(client_fd, discard, MSG_LEN);
				write(client_fd, "success", strlen("success"));
				read(client_fd, discard, MSG_LEN);
				write(client_fd, "welcome\n", strlen("welcome\n"));
				return j;
			} else {
				/*already logged in fail */
				write(client_fd, "$login", strlen("$login"));
				read(client_fd, discard, MSG_LEN);
				write(client_fd, "failed", strlen("failed"));
				read(client_fd, discard, MSG_LEN);
				write(client_fd, "already logged in\n", strlen("already logged in\n"));
				return -1;
			}

		} else if(username_found) {
			/*has entered the wrong password */
#ifdef DEBUG
			printf("Only username found \n");
#endif
			write(client_fd, "$login", strlen("$login"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "failed", strlen("failed"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "password not found\n", strlen("password not found\n"));
			return -1;
		} else {
			/*user name not found */
#ifdef DEBUG
			printf("Username not found \n");
#endif
			write(client_fd, "$login", strlen("$login"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "failed", strlen("failed"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "username not found\n", strlen("username not found\n"));
			return -1;
		}
	return -1;
}

int MakeServerSocket(int portno) {
    int socket_fd;
    struct sockaddr_in serveraddress;
    
    /*create socket */
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd <0) {
        perror("failed to create socket");
        exit(1);
    }
    
    /*bind socket to server */
    
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(portno); /*The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.*/
    /*The htonl() function converts the unsigned integer hostlong from host byte order to network byte order. */
    /*binds to all avaliable sources refer https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming */
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(socket_fd, (struct sockaddr*)&serveraddress, sizeof(serveraddress)) < 0 ) {
        perror("binding socket to addr failed");
        exit(2);
    }
    if( listen(socket_fd,3 ) < 0) { /*also makes this passive socket */
        perror("failed to listen; too many in backlog");
        exit(3);
    }
    return socket_fd;
}
int VerifyUserName(char* username) {
    
#ifdef DEBUG
    printf("This is the username passed in VerifyUserName : %s \n", username);
#endif

    FILE *fp;
    char ch;
    char currentname[STR_LEN] = {0};
    char currentchar[STR_LEN] = {0};
    
    fp = fopen("./Users/UserList.txt","a+");
    
    if (fp == NULL) {
        printf ("Error opening file; errno = %d\n", errno);
        return 1;
    }


    while((ch=fgetc(fp))!=EOF){
        if ((ch != '\n') && (ch!= '\r')) {
            printf("%c ",ch);
            memset(currentchar, ch, 1);   /*makes the char a string of length 1 with that char */
            strcat(currentname,currentchar); /* concats the two strings together */
        } else {
            /* ch = '\n' or '\r' */
            /*string is done */
            
#ifdef DEBUG
            printf("%s \n", currentname);
#endif

            if (strcmp(username, currentname) != 0) { /*they are not the same */
                memset(currentname, '\0', sizeof(currentname));
            } else { /*they are the same, found username */
                return 1;
            }
            
        }
    }
    
    fclose(fp);    
    return 0;
}
int VerifyPassword(char* username, char* password) {
	FILE *fp;
    char ch;
    char currentpass[STR_LEN] = {0};
    char currentchar[STR_LEN] = {0};
	
	/*changes file path*/
	char currentpath[256] = "./Users/";
	char filetype[STR_LEN] = ".txt";
	strcat(currentpath,username);
	strcat(currentpath,filetype);
	
	fp = fopen(currentpath,"a+");
	
#ifdef DEBUG 
	printf("the path is: %s \n", currentpath);
	printf("the username passed in password() is : %s \n", username);
	printf("the password passed in password() is : %s \n", password);
#endif
	if (fp == NULL) {
        printf ("Error opening file; errno = %d\n", errno);
        return -1;
    }
	
	while((ch=fgetc(fp))!=EOF){
        if ((ch != '\n') && (ch!= '\r')) {
            printf("%c ",ch);
            memset(currentchar, ch, 1);   /*makes the char a string of length 1 with that char */
            strcat(currentpass,currentchar); /* concats the two strings together */
        } else {
            /* ch = '\n' or '\r' */
            /*string is done */
            
#ifdef DEBUG
            printf("%s \n", currentpass);
#endif

            if (strcmp(password, currentpass) != 0) { /*they are not the same */
                memset(currentpass, '\0', sizeof(currentpass));
				break;
            } else { /*they are the same, found username */
                return 1;
            }
            
        }
    }
    

	fclose(fp);
    
    
    return -1;
}

/*pulls user friends who are online and offline */
int PullUserFriends(int client_fd, int message_size, char client_username[10][64],int client_fds[10]) {
	int online_index = FindClient(client_fd, client_fds);
	char discard[MSG_LEN] = {0};
	if(online_index == -1) {
		write(client_fd, "$chat", strlen("$chat"));
		read(client_fd, discard, MSG_LEN);
		write(client_fd, "failed", strlen("failed"));
		read(client_fd, discard, MSG_LEN);
		write(client_fd, "You are not logged in\n", strlen("You are not logged in\n"));
		return -1;
	}
	FILE* fp;
	char user[STR_LEN] = {0};
		strcpy(user,client_username[online_index]);
	char currentpath[256] = "./Users/";
	char filetype[STR_LEN] = ".txt";
	char line[256] = {0};
	char online_str[MSG_LEN] = {0}; int online = 0;
	char offline_str[MSG_LEN] = {0}; int offline = 0;
	
	strcat(currentpath,user);
	strcat(currentpath,filetype);
	fp = fopen(currentpath,"a+");
					/*password */
					fgets(line, sizeof(line), fp); 
					memset(line,'\0',STR_LEN);
					/*friendlist */
					fgets(line, sizeof(line), fp);
					memset(line,'\0',STR_LEN);
	while (fgets(line, sizeof(line), fp)) {
		if( line[0] !='\n') {
			line[strcspn(line, "\r\n")] = 0; 
		} 
		int online_index = CheckOnline(line, client_username);
		if(online_index == -1){
			//offline
			offline++;
			strcat(offline_str,line);
			strcat(offline_str,"\n");
		} else {
			online++;
			strcat(online_str,line);
			strcat(online_str,"\n");
		}
		memset(line,'\0',STR_LEN);
	}
	char final_online[MSG_LEN] = {0};
	char final_offline[MSG_LEN] = {0};
	char temp[STR_LEN] = {0};
	sprintf(temp,"%d",online);
	strcat(final_online,temp);
	strcat(final_online,":");
	strcat(final_online, online_str);
	
	memset(temp, '\0', sizeof(temp));
	
	sprintf(temp,"%d",offline);
	strcat(final_offline, temp);
	strcat(final_offline,":");
	strcat(final_offline, offline_str);
	
	printf("Online string:%s \n",final_online);
	printf("Offline string:%s \n",final_offline);
//	int online_index = CheckOnline(reciever, client_username);
//	int sender_index = FindClient(client_fd, client_fds);



	write(client_fd, "$pull", strlen("$pull"));
	read(client_fd, discard, MSG_LEN);
	write(client_fd, final_online, strlen(final_online));
	read(client_fd, discard, MSG_LEN);
	write(client_fd, final_offline, strlen(final_offline));
	return 0;
}


/*appends the both users to their respective friends list */
void AppendUser(char* username, char* password) {
	FILE *fp, *fp2;

	
	fp = fopen("./Users/UserList.txt","a");
	fprintf(fp,"%s\n",username);
	
	
	/*changes file path*/
	char currentpath[256] = "./Users/";
	char filetype[STR_LEN] = ".txt";
	strcat(currentpath,username);
	strcat(currentpath,filetype);
	
	fp2 = fopen(currentpath,"a");	
	fprintf(fp2,"%s\n",password);	
	fprintf(fp2,".friendlist\n");
	
	fclose(fp2);
	fclose(fp);
	
}
/*empties out the location the user was at and clears the name */
void PrintOnline(int client_fds[10], char client_username[10][64]) {
	int index;
#ifdef DEBUG
	printf("Grabbing locations of all users who are online \n");
#endif
	for(index = 0; index < max_users; index ++ ) {
		printf("client_fds[%d] : ", index);
		if(client_fds[index] == 0) {
			printf("empty \n");
		} else {
			printf("%s \n", client_username[index]);
		}
	}
	printf("Command Finished \n \n");
}

int LogOutUser(int client_fd, int client_fds[10],char client_username[10][64]) {
	int temp;
	for(temp = 0 ; temp < max_users; temp++) {
		if(client_fds[temp] == client_fd) {
			break;
		}
	}
	if(temp != max_users) {
		client_fds[temp] = 0;
		memset(client_username[temp], 0, strlen(client_username[temp]));
#ifdef DEBUG
		printf("\n client cleared at %d\n", temp);
		printf("client_fds[temp] = %d\n", client_fds[temp]);
		if( strcmp(client_username[temp],"") == 0 ) {printf("client_username in that position is now null \n");}
#endif
	}
	return temp;
}
/*return number of friends */
int NumFriends(char *username) {
	int count = 0;
	FILE *fp;

	/*changes file path*/
	char currentpath[256] = "./Users/";
	char filetype[STR_LEN] = ".txt";
	char line[256] = {0};
	strcat(currentpath,username);
	strcat(currentpath,filetype);	
	fp = fopen(currentpath,"a+");
/*password */
	fgets(line, sizeof(line), fp); 

	memset(line,'\0',STR_LEN);
 /*friendlist */
	fgets(line, sizeof(line), fp); 

	memset(line,'\0',STR_LEN);
	while (fgets(line, sizeof(line), fp)) {
		count++;
	}
	
	return count;
}
int SendStatus(char* username, char* status, int fd) {
	char verif[STR_LEN] = "$";
	char discard[MSG_LEN] = {0};
	strcat(verif,status);
	write(fd, verif, strlen(verif));
	read(fd, discard, MSG_LEN);
	write(fd, username, strlen(username));
	return 0;
}
int SendAllFriendStatus(char* status, int sender_fd, int client_fds[10], char client_username[10][64]) {
	
	int i;
	int is_friend;
	int client_index = FindClient(sender_fd,client_fds);
	for(i =0; i < max_users ; i++) {
		if(client_fds[i] != 0 ) {
			if(client_fds[i] != sender_fd) {
				//send message	
				is_friend = AlreadyFriend(client_username[i], client_username[client_index]);
				printf("%d \n",is_friend);
				if(is_friend == 1){
					SendStatus(client_username[client_index],status, client_fds[i]);
				}
			}
		}
	}
	/* returns 1 if you are already friends */
	//int AlreadyFriend(char* username, char* friend) {
	//int FindClient(int client_fd, int client_fds[10]) {
	
	return 0;
}

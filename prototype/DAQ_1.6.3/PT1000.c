int  daq_TCP_init(void);                 // Init TCP Connection (as Server)
int daq_TCP_read();   // Read the value of the TCP Connection

// Initialization of the TCP Connection
int my_socket; 
int working_socket;
int flag=0;
int daq_TCP_init(void){
  
  // int working_socket;
  struct sockaddr_in my_address; 
  
  memset(&my_address, 0, sizeof(struct sockaddr_in));
  // address family is always AF_INET ...
  my_address.sin_family = AF_INET;
  // strcpy(my_address.sin_data, ADDRESS);
  // unlink(my_address.my_addr);
  my_address.sin_addr.s_addr = INADDR_ANY;
  my_address.sin_port = htons(5000);
  // getting a socket ...
  my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (my_socket < 0) {
    printf("could not create a socket ... bye\n");
    
    return 1;
  }
  else {
    printf("got a socket, will bind now ... \n");
  }
  
  int on = 1;
  int status = 0;
  status = setsockopt(my_socket, SOL_SOCKET, SO_REUSEADDR, (const char *) &on, sizeof(on));
  if (-1 == status)perror("setsockopt(...,SO_REUSEADDR,...)");
  
  int BINDret=0;
  
  if ((BINDret=bind(my_socket, (const struct sockaddr *)&my_address, sizeof(struct sockaddr_in)) )< 0){
    printf("BIND ref %i\n",BINDret);
    printf("could not bind to the port ... \n");
    close(my_socket);
    
    return 1;
  }
 

  if (listen(my_socket, 0) < 0) {
    printf("failed to listen ... \n");
    close(my_socket);
    
    return 1;
  }
 
  working_socket = accept(my_socket, NULL, NULL);
  if (working_socket < 0) {
    printf("failed to accept new connection ... \n");
    close(my_socket);
    return 1;
  }
  
  //  write(working_socket, "0", 32);

  return my_socket;
}

char buffer_old[32];
int conto = 0;
int daq_TCP_read(){

  char buffer[32];
  
  //bzero(buffer,sizeof(buffer));
 read_flag:
  // write(working_socket, "1", 32); 
  usleep(10);
  read(working_socket, buffer, 32);
  
  //int idem = strcmp(buffer_old, buffer); 
  //printf("TCP Buffer= %s\t idem= %i\n", buffer, idem);
  //if(idem==0)goto read_flag;
  conto++;
  //  if(!(conto%10))
  //  printf("TCP Buffer buoni %s\t conto=%i \n", buffer, conto);
  
  eh.laser_energy = (float) atof(buffer);
  printf("eh.laser_energy= %f \n", eh.laser_energy);
  //bzero(buffer,sizeof(buffer)); 
  // write(working_socket, "0", 32);

  strcpy(buffer_old,buffer);
  
  if (strlen(buffer) < 3) {
    printf("there was no data ... \n");
  }
    

  return 0;
}

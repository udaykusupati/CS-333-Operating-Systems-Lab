Experimental setup:
A 4 core Dell Inspiron i7 laptop hosting the client and a four core Lenovo Z50-70 laptop hosting the server both with 1 TB HDDs and 8 GB RAMs connected via ethernet LAN network.
Instructions:
to compile server: g++ server-mp.cpp -o server-mp
to compile client: g++ multi-client.cpp -o multi-client -lpthread
to run server: ./server-mp PORTNO
to run client: ./multi-client *server hostname or IP* *PORTNO* *No of threads* *total run time of each thread* *sleep time of each thread* *random or fixed*

	Average response time 1 : 
	Time taken between the client asking for the file and the server getting the first byte of the file 

	Average response time 2 : 
	Time taken between the client asking for the file and the server getting the entire file 



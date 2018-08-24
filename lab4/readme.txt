 setup:
	  Lab computers. The network bottleneck bandwidth was 931 Mbps which translates roughly to 58 req/sec. We used client to query a fixed file(/tmp/foo1.txt in our case).
 to run:
	server side:
		./server-mt <port_no> <num_threads> <MAX_SIZE_QUEUE>
	Client side:
		./multi-client <hostname> <port> <numOfThreads> <timeOfRequest> <timeOfSleep> <fixed/random>

 to compile:
	Compile the file server-mt.cpp using makefile (command: make) (or) 
	run the command: g++ server-mt.cpp -o server-mt -lpthread
	Also use a multi-client and compile it.

Sample test runs:
in testruns folder
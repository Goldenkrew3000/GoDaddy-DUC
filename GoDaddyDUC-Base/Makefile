output: main.o extractConfig.o getExternalIP.o getGoDaddyIP.o sendGoDaddyIP.o
	gcc main.o extractConfig.o getExternalIP.o getGoDaddyIP.o sendGoDaddyIP.o -lcurl -lcjson -o godaddy-duc

main.o: main.c
	gcc -c main.c

extractConfig.o: extractConfig.c
	gcc -c extractConfig.c

getExternalIP.o: getExternalIP.c
	gcc -c getExternalIP.c

getGoDaddyIP.o: getGoDaddyIP.c
	gcc -c getGoDaddyIP.c

sendGoDaddyIP.o: sendGoDaddyIP.c
	gcc -c sendGoDaddyIP.c

clean:
	rm *.o godaddy-duc

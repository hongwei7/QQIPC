qq:qqIPCserver.c  qqIPCclient.c clean
	gcc qqIPCserver.c -o qqIPCserver
	gcc qqIPCclient.c -o qqIPCclient	

clean:
	rm -f qqIPCserver qqIPCclient

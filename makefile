qq: qqIPCserver.c  qqIPCclient.c clean
	gcc qqIPCserver.c -o qqIPCserver -g
	gcc qqIPCclient.c -o qqIPCclient -g	

clean:
	rm -f qqIPCserver qqIPCclient

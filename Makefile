http_web:	http_web.c
	gcc -W -Wall -lpthread -o http_web http_web.c
clean:
	rm http_web

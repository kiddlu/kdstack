cc -g -o test_server  main_server.o main_db.o err_me.o -Wl,-dn -L/usr/local/mysql/lib/ -lmysqlclient  -Wl,-dy -lpthread -lm -ldl -lcrypt

OPENSSL_DIR = /opt/homebrew/opt/openssl

all: tord.so client_ssl

tord.so: tord.c tord.h
	gcc tord.c -o tord.so -fPIC -shared -ldl

client_ssl: client_ssl.c
	gcc client_ssl.c -o client_ssl -Wl,-flat_namespace \
	-I$(OPENSSL_DIR)/include \
	-L$(OPENSSL_DIR)/lib \
	-lssl -lcrypto

clean:
	rm -f tord.so client_ssl
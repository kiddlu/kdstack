#!/bin/bash

#private key
openssl genrsa -out private.pem 2048

#public key
openssl rsa -in private.pem -pubout -out public.pem

#request cert
openssl req -new -out cert.csr -key private.pem

#signed X509
openssl x509 -req -in cert.csr -out public.der -outform der -signkey private.pem -days 365

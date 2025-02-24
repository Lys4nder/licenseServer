#!/bin/bash

# Configuration variables
CA_CERT="ca_cert.pem"
CA_KEY="ca_key.pem"
CA_SRL="ca_cert.srl"
SERVER_KEY="server_key.pem"
SERVER_CERT="server_cert.pem"
SERVER_CHAIN="server_chain.pem"
SERVER_CSR="server.csr"
CLIENT_KEY="client_key.pem"
CLIENT_CERT="client_cert.pem"
CLIENT_CSR="client.csr"
CONFIG_FILE="openssl.cnf"
DAYS_VALID=365

# Create OpenSSL configuration file with SANs
cat > ${CONFIG_FILE} <<EOL
[ req ]
default_bits       = 4096
default_md         = sha256
distinguished_name = req_distinguished_name
req_extensions     = req_ext
x509_extensions    = v3_ca # The extensions to add to the self-signed cert

[ req_distinguished_name ]
countryName                = Country Name (2 letter code)
countryName_default        = US
stateOrProvinceName        = State or Province Name (full name)
stateOrProvinceName_default = California
localityName               = Locality Name (eg, city)
localityName_default       = San Francisco
organizationName           = Organization Name (eg, company)
organizationName_default   = My Company
commonName                 = Common Name (eg, fully qualified host name)
commonName_default         = my.server.com

[ req_ext ]
subjectAltName = @alt_names

[ v3_ca ]
subjectAltName = @alt_names
basicConstraints = critical,CA:TRUE
keyUsage = critical, digitalSignature, keyEncipherment
extendedKeyUsage = serverAuth

[ alt_names ]
DNS.1   = my.server.com
DNS.2   = localhost
IP.1    = 127.0.0.1
IP.2    = 0.0.0.0
EOL

# Generate CA key and certificate
openssl req -x509 -new -nodes -keyout ${CA_KEY} -out ${CA_CERT} -days ${DAYS_VALID} 
-subj "/C=US/ST=California/L=San Francisco/O=My Company/CN=my.ca.com"

# Generate server key
openssl genpkey -algorithm RSA -out ${SERVER_KEY} -pkeyopt rsa_keygen_bits:4096

# Generate server CSR
openssl req -new -key ${SERVER_KEY} -out ${SERVER_CSR} -config ${CONFIG_FILE}

# Sign server certificate with CA
openssl x509 -req -in ${SERVER_CSR} -CA ${CA_CERT} -CAkey ${CA_KEY} -CAcreateserial 
-out ${SERVER_CERT} -days ${DAYS_VALID} -extfile ${CONFIG_FILE} -extensions v3_ca

# Create server certificate chain
cat ${SERVER_CERT} ${CA_CERT} > ${SERVER_CHAIN}

# Generate client key
openssl genpkey -algorithm RSA -out ${CLIENT_KEY} -pkeyopt rsa_keygen_bits:4096

# Generate client CSR
openssl req -new -key ${CLIENT_KEY} -out ${CLIENT_CSR} -subj 
"/C=US/ST=California/L=San Francisco/O=My Company/CN=my.client.com"

# Sign client certificate with CA
openssl x509 -req -in ${CLIENT_CSR} -CA ${CA_CERT} -CAkey ${CA_KEY} -CAcreateserial 
-out ${CLIENT_CERT} -days ${DAYS_VALID}

echo "SSL files generated successfully:"
echo "CA Certificate: ${CA_CERT}"
echo "CA Key: ${CA_KEY}"
echo "Server Key: ${SERVER_KEY}"
echo "Server Certificate: ${SERVER_CERT}"
echo "Server Chain: ${SERVER_CHAIN}"
echo "Client Key: ${CLIENT_KEY}"
echo "Client Certificate: ${CLIENT_CERT}"

FROM	debian:bookworm

#install all the program
RUN		apt-get update
RUN		apt-get install -y nginx openssl

# nginx SSL
RUN mkdir /etc/nginx/ssl
RUN openssl req -newkey rsa:4096 -x509 -sha256 -days 365 -nodes \
       -out /etc/nginx/ssl/jumarque.crt \
       -keyout /etc/nginx/ssl/jumarque.key \
       -subj "/C=SP/ST=Spain/L=Malaga/O=42 School/OU=jumarque/CN=jumarque/"

COPY ./conf/nginx.conf /etc/nginx/conf.d

# nginx config
RUN mkdir -p /run/nginx

#the only port allowed
EXPOSE	443

# Start nginx
CMD ["nginx", "-g", "daemon off;"]
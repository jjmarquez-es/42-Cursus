FROM debian:bookworm

RUN apt-get update && apt-get install -y nginx
COPY ./conf/default /etc/nginx/sites-available/default
COPY ./tools/index.html /var/www/html/index.html
CMD ["nginx", "-g", "daemon off;"]
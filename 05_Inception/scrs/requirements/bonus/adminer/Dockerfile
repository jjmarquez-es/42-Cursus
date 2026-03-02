FROM debian:bookworm

# install prerequisites
# https://wordpress.com/support/php-environment/
RUN apt-get update && apt-get install -y \
    php8.2-fpm \
    php8.2-mysql \
    php8.2-curl \
    php8.2-gd \
    php8.2-intl \
    php8.2-mbstring \
    php8.2-xml \
    php8.2-zip \
    php8.2-redis \
    mariadb-client \
    wget \
    curl \
    && apt-get clean && rm -rf /var/lib/apt/lists/*
#RUN the adminer part, create html directory, move the adminer directory and add user
RUN mkdir -p /var/www/html
RUN wget "https://www.adminer.org/latest.php" -O /var/www/html/adminer.php

#RUN useradd -r -g www-data www-data

COPY /conf/www.conf /etc/php8.2/php-fpm.d/www.conf

EXPOSE 9000
STOPSIGNAL SIGQUIT

CMD ["php-fpm8.2", "--nodaemonize"]
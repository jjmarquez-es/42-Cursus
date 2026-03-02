FROM    debian:bookworm

WORKDIR /var/hugo/me

RUN apt update && apt upgrade -y && apt install -y hugo git
# shadow
# create new hugo site
RUN hugo new site /var/hugo/me
RUN cd /var/hugo/me
RUN git clone https://github.com/opera7133/tella.git themes/tella   
RUN hugo -b https://jumarque.42.fr/me

COPY    ./conf/config.toml        /var/hugo/me/config.toml
COPY    conf/avatar.jpg         /var/hugo/me/themes/tella/static/avatar.jpg

EXPOSE 1313

CMD     ["hugo", "server", \
         "--bind=0.0.0.0", \
         "--baseURL=https://jumarque.42.fr/me", \
         "-p", "1313", \
         "--logFile=/dev/stdout", \
        "--appendPort=false"]
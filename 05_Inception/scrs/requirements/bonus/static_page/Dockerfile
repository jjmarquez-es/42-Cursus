FROM    debian:bookworm

WORKDIR /var/hugo/static

RUN     apt update && apt upgrade -y && apt install -y hugo git
# shadow
# create new hugo site
RUN hugo new site /var/hugo/static \
 	&&     cd /var/hugo/static \
 	&&     git clone https://github.com/luizdepra/hugo-coder themes/hugo-coder \
 	&&     hugo new posts/presentation.md \
 	&&     hugo -b https://jumarque.42.fr/static \
	&& mkdir -p /var/hugo/me/content/about

COPY    ./conf/config.toml        /var/hugo/static/config.toml
COPY    conf/presentation.md    /var/hugo/static/content/posts/presentation.md
COPY    conf/about.md           /var/hugo/static/content/about/index.md
COPY    conf/avatar.jpg         /var/hugo/static/themes/hugo-coder/static/avatar.jpg
COPY	conf/42fitness.jpg		/var/hugo/static/themes/hugo-coder/static/42fitness.jpg

EXPOSE 1313

CMD     ["hugo", "server", \
         "--bind=0.0.0.0", \
         "--baseURL=https://jumarque.42.fr/static", \
         "-p", "1313", \
         "--logFile=/dev/stdout", \
        "--appendPort=false"]
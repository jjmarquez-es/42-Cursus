FROM debian:bookworm

RUN	apt update && apt upgrade -y && apt install -y vsftpd

COPY conf/vsftpd.conf /etc/vsftpd.conf
COPY tools/server_ftp.sh /etc/server_ftp.sh
ENTRYPOINT ["sh", "/etc/server_ftp.sh"]

#!/bin/sh

if [ ! -f "/etc/vsftpd/vsftpd.conf.bak" ]; then

    mkdir -p /var/www/html
	mkdir -p /var/run/vsftpd/empty

    # Add the FTP_USER, change his password and declare him as the owner of wordpress folder and all subfolders
    adduser --disabled-password --gecos "" jumarque
    echo "jumarque:$FTP_PWD" | chpasswd
    chown -R $FTP_USR:$FTP_USR /var/run/vsftpd/empty

	#chmod +x /etc/vsftpd/vsftpd.conf
    echo $FTP_USR | tee -a /etc/vsftpd.userlist &> /dev/null

fi

echo "FTP started on :21"
/usr/sbin/vsftpd /etc/vsftpd.conf
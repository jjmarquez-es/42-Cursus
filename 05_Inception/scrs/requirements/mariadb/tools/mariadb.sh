#!/bin/sh

# 1. Initialize if empty
if [ ! -d "/var/lib/mysql/mysql" ]; then
    echo "Initializing MariaDB data directory..."
    mysql_install_db --user=mysql --datadir=/var/lib/mysql --skip-test-db
fi

# 2. Start MariaDB in a way that ignores grant tables temporarily
# This allows us to configure the root user without needing the old password
echo "Starting MariaDB in safe mode..."
/usr/sbin/mysqld --user=mysql --datadir=/var/lib/mysql --skip-grant-tables --skip-networking &
PID=$!

# 2. Wait for it to be ready
# We use the CLIENT (mysqladmin) to check if the SERVER is awake
until mysqladmin ping >/dev/null 2>&1; do
    echo "Waiting for MariaDB..."
    sleep 1
done

# 3. Apply configurations
echo "Applying SQL configurations..."
mysql << _EOF_
FLUSH PRIVILEGES;
ALTER USER 'root'@'localhost' IDENTIFIED BY '$MYSQL_ROOT_PASSWORD';
CREATE DATABASE IF NOT EXISTS \`$MYSQL_DATABASE\`;
CREATE USER IF NOT EXISTS '$MYSQL_USER'@'%' IDENTIFIED BY '$MYSQL_PASSWORD';
GRANT ALL PRIVILEGES ON \`$MYSQL_DATABASE\`.* TO '$MYSQL_USER'@'%';
FLUSH PRIVILEGES;
_EOF_

# 4. Import WordPress data if the file exists
if [ -f "/usr/local/bin/wordpress.sql" ]; then
    mysql -u root -p"$MYSQL_ROOT_PASSWORD" "$MYSQL_DATABASE" < /usr/local/bin/wordpress.sql
fi

# 5. Shut down the temporary background process
echo "Shutting down temporary MariaDB..."
kill "$PID"
wait "$PID"

# 6. Start MariaDB normally (as the main process)
echo "Starting MariaDB normally..."
/usr/sbin/mysqld --user=mysql --datadir=/var/lib/mysql --bind-address=0.0.0.0
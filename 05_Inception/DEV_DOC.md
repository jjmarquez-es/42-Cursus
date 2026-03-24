# 🛠️ DEV_DOC.md - Technical Specifications (Inception) EN Version

## 1. Microservices Architecture
The project is based on a multi-container infrastructure orchestrated by **Docker Compose**, using custom images built on **Debian BookWorm**.

### Data Flow Diagram
1.  **Nginx (Entry Point):** Acts as a Reverse Proxy managing SSL traffic (Port 443).
2.  **WordPress & MariaDB:** The CMS core communicates via port 9000 (FastCGI) and port 3306 (SQL).
3.  **Redis Cache:** WordPress uses Redis (Port 6379) to reduce database load by storing objects in memory.
4.  **Ollama & Static Site:** A local AI service accessible via a static interface that consumes an internal API.

---

## 2. Network Configuration
A `bridge` type network named `network` is used to isolate services.
* **Isolation:** Only the `nginx` service exposes port 443 to the outside.
* **DNS Resolution:** Containers communicate with each other using their service names as hostnames (e.g., `http://mariadb:3306`).

---

## 3. Container Details (Building Logic)

### Nginx (Secure Frontend)
* **Base:** Debian BookWorm.
* **Protocols:** TLS 1.2 and 1.3 only.
* **Certificates:** Generated using `openssl` during the build process.
* **Dynamic Routes:** Implements `location` blocks to redirect traffic to Adminer, Ollama, and the static site.

### WordPress + Redis
* **Processor:** PHP-FPM configured on port 9000.
* **Dependency:** Waits for MariaDB to be available before starting.
* **Redis Integration:** Configured to connect to the `redis` service on port 6379 for object caching.

### MariaDB (Persistence)
* **Storage:** Data resides in `/var/lib/mysql` inside the container, mapped to the host machine for persistence.
* **Security:** Automatic initialization of databases and users via environment variables defined in `.env`.

### Ollama AI (Bonus)
* **Model:** Local AI inference server.
* **Model Persistence:** Downloaded models are stored in `/root/.ollama` to avoid re-downloads after service restarts.

---

## 4. Volume and Permission Management
Volumes are centralized in the user's folder for easy maintenance:
* **MySQL:** `/home/${USER}/Inception/data/mysql`.
* **WordPress:** `/home/${USER}/Inception/data/wordpress`.
* **Ollama Models:** `/home/${USER}/Inception/data/models`.

---

## 5. Entrypoint Scripts (Automation)
Each critical service has a `.sh` script responsible for transforming `.env` variables into system configurations.

### 5.1. MariaDB Initialization
1.  **Directory Check:** Verifies if `/var/lib/mysql/mysql` exists; if not, it runs `mysql_install_db`.
2.  **Security Setup:** Creates a temporary `.sql` file to set the root password, create the `${MYSQL_DATABASE}`, and grant privileges to `${MYSQL_USER}`.
3.  **Execution:** Launches `mysqld` processing the temporary file, then deletes it for security.

### 5.2. WordPress Autoinstallation (WP-CLI)
1.  **Connection Wait:** Polling for the `mariadb` container on port 3306.
2.  **Download & Config:** Downloads the core and creates `wp-config.php` using `.env` credentials.
3.  **Core Install:** Sets the URL (`jumarque.42.fr`), site title, and admin user.
4.  **Redis Integration:** Installs the `redis-cache` plugin and configures `WP_REDIS_HOST` and `WP_CACHE` constants.

---

## 6. Advanced Bonus Service Configuration

### 6.1. FTP Server (vsftpd) Passive Mode
The `ftp-server` allows external editing of WordPress files.
* **Network Isolation:** Port `21` for control and ports `21100-21110` for the data channel.
* **Host Configuration:** These ports are mapped directly in `docker-compose.yml` for external clients like FileZilla.

### 6.2. Ollama API Architecture
* **REST API:** Exposed on port `11434`.
* **Reverse Proxy:** Nginx handles requests to `https://jumarque.42.fr/ollama/api/tags`, resolving CORS issues for the `static_site`.

---

## 7. Development Lifecycle (Makefile)
The `Makefile` automates deep cleaning:
* **`make all`**: Progressive build.
* **`make clean`**: 
    1.  `docker compose down --volumes --rmi all`.
    2.  `docker system prune -a --force`.
    3.  `sudo rm -rf data/` (Cleans physical host files).

---
---

## 8. Detailed Entrypoint Logic (Automation)

**EN:** Entrypoint scripts are the "brain" of each container. Their function is to transform `.env` file variables into actual system configurations.

### 8.1. MariaDB: Database Initialization / Inicialización de la Base de Datos
**EN:** The MariaDB script follows this logical flow to ensure persistence:
1. **Directory Check:** Verifies if `/var/lib/mysql/mysql` exists. If not, it runs `mysql_install_db` to initialize the database filesystem.
2. **Security Setup:** Creates a temporary `.sql` file executing:
    * Sets the `root` password.
    * Creates the database specified in `${MYSQL_DATABASE}`.
    * Creates `${MYSQL_USER}` and grants full privileges.
3. **Execution:** Launches `mysqld` processing the temp file and then deletes it for security.

### 8.2. WordPress: Autoinstallation via WP-CLI 
**EN:** Unlike a manual install, this container uses **WP-CLI** for deployment:
1. **Connection Wait:** Polling for `mariadb` on port 3306. It won't proceed until the DB responds.
2. **Download & Config:** If `wp-config.php` is missing, it runs `wp core download` and `wp config create`.
3. **Core Installation:** Runs `wp core install` defining the URL (`jumarque.42.fr`), site title, and admin.
4. **Redis Integration:** Installs/activates `redis-cache` and configures `WP_REDIS_HOST` and `WP_CACHE` in `wp-config.php`.

---

## 9. Web Server Configuration (Nginx)

### 9.1. Server Blocks & SSL
**EN:** Port 443 exclusively listens to HTTPS requests using TLSv1.2 and TLSv1.3. Certificates are located at `/etc/nginx/ssl/jumarque.crt`.

### 9.2. Service Routing (Reverse Proxy)
| Route (External) | Internal Destination | Protocol |
| :--- | :--- | :--- |
| `/` | `wordpress:9000` | FastCGI (PHP) |
| `/adminer` | `adminer:8080` | HTTP Proxy |
| `/ollama/` | `ollama-ai:11434` | HTTP Proxy |

---

## 10. Optimization & Bonus

### 10.1. Redis (Object Cache)
**EN:** Redis stores frequent SQL query results. Flow: `Nginx` → `WordPress` → `Redis` (if hit) OR `MariaDB` (if miss).

### 10.2. FTP (vsftpd)
**EN:** Uses passive mode for Docker firewall compatibility (Ports 21100-21110). Root points to `/var/www/html`.

---

## 11. Development Lifecycle (Makefile)

**EN:** `make all` builds and starts. `make clean` removes containers, volumes, images, and physically deletes the host `data/` folder.

---

## 12. Advanced Bonus Configuration

### 12.1. FTP Passive Mode
**EN:** Ports are mapped directly to the host in `docker-compose.yml` for external clients like FileZilla.

### 12.2. Ollama API Architecture
**EN:** Models are stored in `/home/${USER}/Inception/data/models`. Nginx handles CORS for the `static_site`.

---

## 13. System Security & Hardening

### 13.1. SSL Management / Gestión SSL
**EN:** Strong cipher suites are used to ensure Forward Secrecy.

### 13.2. Restart Policies / Políticas de Reinicio
**EN:** Services use `restart: always`. `depends_on` ensures the stack boots in the correct order.

---

## 14. Troubleshooting

**EN:** 1. Check networks: `docker network inspect network`.
2. Check permissions: Verify host permissions on `/home/${USER}/Inception/data/`.
3. Check build logs: Review `Dockerfiles` in `requirements/`.

# 🛠️ DEV_DOC.md - Especificaciones Técnicas (Inception) ES Version

## 1. Arquitectura de Microservicios
El proyecto se basa en una infraestructura multi-contenedor orquestada por **Docker Compose**, utilizando imágenes personalizadas construidas sobre **Debian BookWorm**.

### Diagrama de Flujo de Datos
1.  **Nginx (Entry Point):** Actúa como Proxy Inverso gestionando el tráfico SSL (Puerto 443).
2.  **WordPress & MariaDB:** El núcleo del CMS se comunica mediante el puerto 9000 (FastCGI) y el puerto 3306 (SQL).
3.  **Redis Cache:** WordPress utiliza Redis (Puerto 6379) para reducir la carga de la base de datos mediante el almacenamiento de objetos en memoria.
4.  **Ollama & Static Site:** Un servicio de IA local accesible mediante una interfaz estática que consume una API interna.

---

## 2. Configuración de Redes (Networks)
Se utiliza una red de tipo `bridge` denominada `network` para aislar los servicios.
* **Aislamiento:** Solo el servicio `nginx` expone el puerto 443 al exterior.
* **Resolución DNS:** Los contenedores se comunican entre sí utilizando sus nombres de servicio como hostnames (ej: `http://mariadb:3306`).

---

## 3. Detalles de los Contenedores (Building Logic)

### Nginx (Frontend Seguro)
* **Base:** Debian BookWorm.
* **Protocolos:** TLS 1.2 y 1.3 únicamente.
* **Certificados:** Generados mediante `openssl` en el proceso de construcción.
* **Rutas Dinámicas:** Implementa bloques `location` para redirigir tráfico a Adminer, Ollama y el sitio estático.

### WordPress + Redis
* **Procesador:** PHP-FPM configurado en el puerto 9000.
* **Dependencia:** Espera a que MariaDB esté disponible antes de iniciar.
* **Integración Redis:** Configurado para conectarse al servicio `redis` en el puerto 6379 para la caché de objetos.

### MariaDB (Persistencia)
* **Almacenamiento:** Los datos residen en `/var/lib/mysql` dentro del contenedor, mapeados a la máquina host para persistencia.
* **Seguridad:** Inicialización automática de bases de datos y usuarios mediante variables de entorno definidas en `.env`.

### Ollama AI (Bonus)
* **Modelo:** Servidor de inferencia de IA local.
* **Persistencia de Modelos:** Los modelos descargados se almacenan en `/root/.ollama` para evitar re-descargas tras el reinicio del servicio.

---

## 4. Gestión de Volúmenes y Permisos
Los volúmenes están centralizados en la carpeta del usuario para facilitar el mantenimiento:
* **MySQL:** `/home/${USER}/Inception/data/mysql`.
* **WordPress:** `/home/${USER}/Inception/data/wordpress`.
* **Ollama Models:** `/home/${USER}/Inception/data/models`.

> **Nota para Devs:** El comando `make clean` utiliza `sudo rm -rf` sobre estas rutas, lo que garantiza una limpieza total del entorno antes de una nueva reconstrucción.

---

## 5. Scripts de Entrada (Entrypoints)
Cada servicio crítico cuenta con un script `.sh` encargado de:
1.  Verificar la existencia de archivos de configuración.
2.  Configurar usuarios y contraseñas dinámicamente según el `.env`.
3.  Lanzar el servicio en primer plano para mantener el contenedor activo.

---

## 6. Variables de Entorno Requeridas
El archivo `.env` debe definir, como mínimo, las siguientes claves para el correcto despliegue:
* `MYSQL_DATABASE`, `MYSQL_USER`, `MYSQL_PASSWORD`.
* `DOMAIN_NAME` (jumarque.42.fr).
* Configuraciones para Redis y el servidor FTP.

---

## 7. Comandos de Depuración
Para inspeccionar el estado técnico durante el desarrollo:
```bash
# Ver logs detallados de un servicio específico
docker compose -f ./scrs/docker-compose.yml logs -f <servicio>

# Acceder a la shell de un contenedor en ejecución
docker exec -it <container_name> /bin/bash

# Verificar la conectividad de red interna
docker run --rm --network scrs_network alpine ping mariadb
```

---

## 8. Lógica Detallada de los Entrypoints (Automatización)

Los scripts de entrada son el "cerebro" de cada contenedor. Su función es transformar las variables del archivo `.env` en configuraciones reales del sistema.

### 8.1. MariaDB: Inicialización de la Base de Datos
El script de MariaDB sigue este flujo lógico para garantizar la persistencia:
1.  **Comprobación de Directorio:** Verifica si la carpeta `/var/lib/mysql/mysql` existe. Si no existe, ejecuta `mysql_install_db` para inicializar el sistema de archivos de la base de datos.
2.  **Configuración de Seguridad:** Crea un archivo temporal `.sql` que ejecuta los siguientes comandos:
    * Configura la contraseña del `root`.
    * Crea la base de datos especificada en `${MYSQL_DATABASE}`.
    * Crea el usuario `${MYSQL_USER}` y le otorga privilegios totales.
3.  **Ejecución:** Lanza el demonio `mysqld` procesando el archivo temporal y luego lo elimina por seguridad.



### 8.2. WordPress: Autoinstalación via WP-CLI
A diferencia de una instalación manual, tu contenedor utiliza **WP-CLI** para automatizar el despliegue:
1.  **Espera de Conexión:** El script intenta conectar con el contenedor `mariadb` en el puerto 3306. No avanza hasta que la base de datos responde.
2.  **Descarga y Configuración:** * Si `wp-config.php` no existe, ejecuta `wp core download` y `wp config create` usando las credenciales del `.env`.
3.  **Instalación del Core:** Ejecuta `wp core install` definiendo la URL (`jumarque.42.fr`), el título del sitio y el usuario administrador.
4.  **Integración con Redis:**
    * Instala y activa el plugin `redis-cache`.
    * Configura las constantes `WP_REDIS_HOST` (apuntando al servicio `redis`) y `WP_CACHE` en el archivo `wp-config.php`.

---

## 9. Configuración del Servidor Web (Nginx)

El archivo de configuración en `requirements/nginx/conf` es vital para el routing del proyecto:

### 9.1. Bloques de Servidor y SSL
* **Puerto 443:** Escucha exclusivamente peticiones HTTPS con protocolos TLSv1.2 y TLSv1.3.
* **Certificados:** Utiliza la ruta `/etc/nginx/ssl/jumarque.crt` generada en el Dockerfile.

### 9.2. Routing de Servicios (Reverse Proxy)
Nginx redirige el tráfico interno basándose en los nombres de los servicios definidos en el `docker-compose.yml`:

| Ruta Externa | Destino Interno | Protocolo |
| :--- | :--- | :--- |
| `/` | `wordpress:9000` | FastCGI (PHP) |
| `/adminer` | `adminer:8080` | HTTP Proxy |
| `/ollama/` | `ollama-ai:11434` | HTTP Proxy |

---

## 10. Optimización y Bonus: Redis & FTP

### 10.1. Redis (Object Cache)
El contenedor de Redis funciona como un almacén de clave-valor en memoria. 
* **Beneficio:** Almacena los resultados de las consultas SQL frecuentes de WordPress. 
* **Flujo:** `Nginx` → `WordPress (PHP)` → `Redis` (Si existe el dato) OR `MariaDB` (Si no existe).



### 10.2. FTP (vsftpd)
El servidor FTP permite la gestión de archivos directamente en el volumen de WordPress:
* **Configuración:** Utiliza el modo pasivo para trabajar correctamente detrás de un firewall de Docker, con un rango de puertos definido (21100-21110).
* **Acceso:** El usuario FTP tiene su `local_root` apuntando a `/var/www/html`, sincronizado con el volumen de WordPress.

---

## 11. Ciclo de Vida del Desarrollo (Makefile)

El `Makefile` automatiza la limpieza de bajo nivel que Docker a veces ignora:
* **`make all`**: Construcción progresiva.
* **`make clean`**: 
    1.  `docker compose down --volumes --rmi all` (Borra contenedores, volúmenes de compose e imágenes).
    2.  `docker system prune -a --force` (Elimina cualquier rastro oculto).
    3.  `sudo rm -rf data/` (Limpia los archivos físicos del host para evitar conflictos de permisos en el próximo inicio).

---

Para finalizar el **`DEV_DOC.md`** con un nivel de detalle técnico superior, profundizaremos en la configuración de red del servidor FTP y la arquitectura de comunicación de la API de Ollama, basándonos en tu estructura de contenedores.

---

## 12. Configuración Avanzada de Servicios Bonus

### 12.1. Servidor FTP (vsftpd) y Modo Pasivo
El servicio `ftp-server` está diseñado para permitir la edición externa de los archivos de WordPress mapeados en `/var/www/html`.
* **Aislamiento de Red:** Utiliza el puerto `21` para el canal de control y un rango definido de puertos (`21100-21110`) para el canal de datos.
* **Configuración del Host:** Estos puertos están mapeados directamente al host en el `docker-compose.yml` para permitir la conexión desde clientes externos como FileZilla.
* **Persistencia Compartida:** El volumen se sincroniza con el contenedor `wordpress` y `nginx`, permitiendo que los cambios realizados vía FTP se reflejen instantáneamente en la web.

### 12.2. Arquitectura de la API de Ollama
El contenedor `ollama-ai` funciona como un servidor de inferencia que expone una API REST en el puerto `11434`.
* **Gestión de Modelos:** Los modelos se almacenan en el host bajo `/home/${USER}/Inception/data/models`, lo que permite que el binario de Ollama acceda a ellos rápidamente sin necesidad de re-descargarlos tras un `make re`[cite: 1, 6].
* **Proxy Inverso y CORS:** Nginx actúa como intermediario, lo que permite que el `static_site` realice peticiones `fetch` a `https://jumarque.42.fr/ollama/api/tags` de forma segura.
* **Variables de Entorno:** El servicio carga configuraciones desde el archivo `.env`, permitiendo definir parámetros de ejecución del motor de IA.

---

## 13. Seguridad y Hardening del Sistema

### 13.1. Gestión de Certificados SSL
* **Protocolos:** El servidor Nginx está restringido para aceptar únicamente **TLSv1.2** y **TLSv1.3**, descartando versiones vulnerables como SSLv3 o TLSv1.0.
* **Cifrado:** Se utilizan suites de cifrado fuertes para garantizar el *Forward Secrecy*.

### 13.2. Políticas de Reinicio y Disponibilidad
* **Restart Policies:** La mayoría de los servicios críticos (Nginx, MariaDB, WordPress, Redis) están configurados con `restart: always` para garantizar que el sistema se recupere automáticamente tras un fallo del demonio de Docker o un reinicio del host.
* **Dependencias Secuenciales:** El uso de `depends_on` asegura que el stack se levante en el orden correcto, evitando que los servicios de aplicación (WordPress, Adminer) fallen al intentar conectar con una base de datos que aún no ha inicializado su socket.

---

## 14. Troubleshooting para Desarrolladores

Si un servicio no arranca correctamente, sigue este flujo de diagnóstico:
1.  **Verificar Redes:** Asegúrate de que todos los contenedores están en la red `network` mediante `docker network inspect network`.
2.  **Permisos de Carpeta:** Si MariaDB o WordPress fallan al escribir, verifica que el usuario del host tiene permisos sobre `/home/${USER}/Inception/data/`.
3.  **Logs de Construcción:** Si una imagen falla al compilar, revisa el `Dockerfile` correspondiente en la carpeta `requirements/`.

---

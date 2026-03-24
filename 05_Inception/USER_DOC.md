# 📁 USER_DOC.md - Inception Infrastructure (jumarque) EN Version

## 1. Introduction
**Brief Description:** A robust and scalable microservices infrastructure based on Docker. It deploys a complete web environment featuring **WordPress**, **MariaDB**, **Redis** for caching, **FTP** for file management, **Adminer** for database administration, and a **local AI node (Ollama)** with a static monitoring dashboard.

**Project Purpose:** To provide an isolated and secure service platform under the `jumarque.42.fr` domain, allowing experimentation with backend technologies, data persistence, and Large Language Models (LLM) in a controlled local environment.

---

## 2. Prerequisites
* **OS:** Debian 12 BookWorm.
* **Resources:** 8GB RAM (recommended for Ollama) and 30GB of disk space.
* **Networking:** It is mandatory to map the domains in your `/etc/hosts` file:
    ```text
    127.0.0.1 jumarque.42.fr static.jumarque.42.fr
    ```

---

## 3. Installation and Startup
The project utilizes a `Makefile` to simplify the management of containers located in `./scrs/`.

1.  **Launch the project:** Compiles images and starts all services in the background.
    ```bash
    make all
    ```
2.  **Rebuild:** Forces image recompilation and restarts all services.
    ```bash
    make re
    ```

---

## 4. Service Map
| Service | URL / Access | Key Function |
| :--- | :--- | :--- |
| **Nginx** | `https://jumarque.42.fr` | Main Server (Secure Proxy). |
| **Static Site** | `https://static.jumarque.42.fr` | Monitoring Dashboard and AI status. |
| **Adminer** | `https://jumarque.42.fr/adminer` | Graphical interface for MariaDB. |
| **Ollama** | `https://jumarque.42.fr/ollama` | AI model inference engine. |
| **FTP** | Port 21 | File transfer for WordPress. |
| **Redis** | Port 6379 (Internal) | WordPress optimization and caching. |

---

## 5. Usage Examples
* **AI Management:** Access the static web interface to verify if the Ollama service is "Online" and list the available models in the `/models` volume.
* **Persistence:** All WordPress data, the MySQL database, and AI models are stored locally in the `data/` folder.

---

## 6. Maintenance and Deep Cleaning
The `make clean` command in this project is exhaustive and performs the following actions:
* **Containers:** Stops and removes all active containers.
* **Images and Volumes:** Deletes all Docker images and associated volumes to free up space.
* **Networks:** Removes custom networks created by Docker.
* **Local Data:** Physically deletes the contents of the `data/mysql/`, `data/wordpress/`, and `data/ollama/` folders using `sudo rm -rf`.

**Total cleaning command:**
```bash
make clean
```

---

## 7. Support
For issues regarding the FTP server or Redis configuration, contact the project lead (**jumarque**). For real-time logs, use:
```bash
docker compose -f ./scrs/docker-compose.yml logs -f
```

---

## 8. Conclusion
This `Inception` environment stands out due to the integration of **Ollama** and **Redis**, offering an infrastructure that goes beyond basic web development to delve into system optimization and private AI deployment.

---
In **Debian 12 (Bookworm)**, the `sudo` package is indeed not installed by default if you didn't set a root password during the installation process or if you chose a minimal setup.

To allow a non-root user to manage Docker and perform administrative tasks, you must first switch to the actual **root** account to install the necessary tools.

### 1. Switch to Root and Install Sudo
Since you cannot use `sudo` yet, use the `su` (substitute user) command:

```bash
# Switch to the root user (you will be prompted for the root password)
su -

# Update the package list
apt update

# Install the sudo package
apt install sudo -y
```

### 2. Add your user to the Sudo and Docker groups
Once `sudo` is installed, you need to grant your user administrative permissions and Docker access:

```bash
# Add your user to the 'sudo' group
usermod -aG sudo your_username

# Add your user to the 'docker' group
# (Create the group first if it doesn't exist)
groupadd docker
usermod -aG docker your_username
```
*Replace `your_username` with your actual login name.*

### 3. Apply the changes
For the system to recognize these new group memberships, you **must** log out and log back in, or restart the machine.

---

### Summary of Commands (Root Console)

| Step | Command | Description |
| :--- | :--- | :--- |
| **Log in as Root** | `su -` | Required to perform initial setup. |
| **Install Sudo** | `apt update && apt install sudo` | Installs the missing admin tool in Debian 12. |
| **Group: Sudo** | `usermod -aG sudo $USER` | Allows the user to use the `sudo` command. |
| **Group: Docker** | `usermod -aG docker $USER` | Allows the user to run containers without `sudo`. |

> **Note on Debian 12:** If you are working on a 42 Virtual Machine, ensure you have enough disk space before proceeding, as we previously saw "No space left on device" errors. Installing packages and running Docker requires at least a few hundred MBs of free space.

---

# 📁 USER_DOC.md - Inception Infrastructure (jumarque) ES Version

## 1. Introducción
**Descripción Breve:** Una infraestructura de microservicios robusta y escalable basada en Docker. Despliega un entorno web completo con **WordPress**, **MariaDB**, **Redis** para caché, **FTP** para gestión de archivos, **Adminer** para bases de datos y un nodo de **IA local (Ollama)** con un Dashboard de monitoreo estático.

**Propósito del Proyecto:** Proporcionar una plataforma de servicios aislada y segura bajo el dominio `jumarque.42.fr`, permitiendo la experimentación con tecnologías de backend, persistencia de datos y modelos de lenguaje (LLM) en un entorno local controlado.

---

## 2. Requisitos Previos
* **SO:** Debian 12 BookWorm.
* **Recursos:** 8GB RAM (recomendado para Ollama) y 30GB de espacio en disco.
* **Red:** Es obligatorio mapear los dominios en su archivo `/etc/hosts`:
    ```text
    127.0.0.1 jumarque.42.fr static.jumarque.42.fr
    ```

---

## 3. Instalación e Inicio
El proyecto utiliza un `Makefile` para simplificar la gestión de los contenedores situados en `./scrs/`.

1.  **Levantar el proyecto:** Compila las imágenes y arranca todos los servicios en segundo plano.
    ```bash
    make all
    ```
2.  **Reconstruir:** Fuerza la recompilación de las imágenes y reinicia los servicios.
    ```bash
    make re
    ```

---

## 4. Mapa de Servicios
| Servicio | URL / Acceso | Función Clave |
| :--- | :--- | :--- |
| **Nginx** | `https://jumarque.42.fr` | Servidor principal (Proxy Seguro). |
| **Static Site** | `https://static.jumarque.42.fr` | Dashboard de monitoreo y estado de la IA. |
| **Adminer** | `https://jumarque.42.fr/adminer` | Interfaz gráfica para MariaDB. |
| **Ollama** | `https://jumarque.42.fr/ollama` | Motor de inferencia de modelos de IA. |
| **FTP** | Puerto 21 | Transferencia de archivos a WordPress. |
| **Redis** | Puerto 6379 (Interno) | Optimización y caché de WordPress. |

---

## 5. Ejemplos de Uso
* **Gestión de IA:** Accede a la web estática para verificar si el servicio Ollama está "Online" y listar los modelos disponibles en el volumen `/models`.
* **Persistencia:** Todos los datos de WordPress, la base de datos MySQL y los modelos de IA se guardan localmente en la carpeta `data/`.

---

## 6. Mantenimiento y Limpieza Profunda
El comando `make clean` de este proyecto es exhaustivo y realiza las siguientes acciones:
* **Contenedores:** Detiene y elimina todos los contenedores activos.
* **Imágenes y Volúmenes:** Borra todas las imágenes de Docker y los volúmenes asociados para liberar espacio.
* **Redes:** Elimina las redes personalizadas creadas por Docker.
* **Datos Locales:** Borra físicamente el contenido de las carpetas `data/mysql/`, `data/wordpress/` y `data/ollama/` mediante `sudo rm -rf`.

**Comando de limpieza total:**
```bash
make clean
```

---

## 7. Soporte
Para incidencias con el servidor FTP o la configuración de Redis, contactar con el responsable del proyecto (**jumarque**). Para logs en tiempo real, utilice:
```bash
docker compose -f ./scrs/docker-compose.yml logs -f
```

---

## 8. Conclusión
Este entorno `Inception` destaca por la integración de **Ollama** y **Redis**, ofreciendo una infraestructura que va más allá del desarrollo web básico para adentrarse en la optimización de sistemas y el despliegue de IA privada.

---
En **Debian 12 (Bookworm)**, es muy común que el paquete `sudo` no venga instalado por defecto si configuraste una contraseña de root durante la instalación. Para solucionar esto y permitir que tu usuario gestione Docker sin ser root, debes seguir estos pasos desde la cuenta de **superusuario**.

### 1. Entrar como Root e instalar `sudo`
Como tu usuario aún no tiene permisos, primero debes transformarte en el administrador real usando el comando `su` (substitute user):

```bash
# Cambia al usuario root (te pedirá la contraseña de root)
su -

# Actualiza la lista de paquetes e instala sudo
apt update && apt install sudo -y
```

### 2. Añadir tu usuario a los grupos `sudo` y `docker`
Una vez instalado `sudo`, debes darte permisos administrativos y de gestión de contenedores. Sustituye `tu_usuario` por tu nombre de login real:

```bash
# Añade tu usuario al grupo 'sudo'
usermod -aG sudo tu_usuario

# Añade tu usuario al grupo 'docker'
# (Crea el grupo primero por si acaso no existe)
groupadd docker
usermod -aG docker tu_usuario
```

### 3. Aplicar los cambios
Para que Debian reconozca que ahora perteneces a estos nuevos grupos, **debes cerrar sesión y volver a entrar** (o reiniciar la máquina).



---

### Resumen de comandos (Consola de Root)

| Paso | Comando | Descripción |
| :--- | :--- | :--- |
| **Entrar como Root** | `su -` | Obligatorio para la configuración inicial. |
| **Instalar Sudo** | `apt update && apt install sudo` | Instala la herramienta de administración en Debian 12. |
| **Grupo Sudo** | `usermod -aG sudo $USER` | Permite al usuario usar el comando `sudo`. |
| **Grupo Docker** | `usermod -aG docker $USER` | Permite ejecutar contenedores sin usar `sudo`. |

> **Nota Crítica:** Recuerda este error "No space left on device". Instalar paquetes y gestionar Docker requiere espacio en disco. Si tu partición está al 100%, estos comandos podrían fallar al intentar escribir en `/var/lib/`.


# 💬 Mini_serv - Servidor de Chat Multicliente

Implementación de un servidor de chat TCP/IP en C que utiliza **multiplexación de E/S** para manejar múltiples conexiones simultáneas sin hilos. El servidor escucha exclusivamente en **127.0.0.1** (localhost) y actúa como un **relay central** que retransmite mensajes entre clientes conectados.

> 💡 **Contexto histórico:** Este tipo de servidores de chat fueron los **pioneros de la comunicación digital** en los años 80-90, siendo la base tecnológica de sistemas como IRC, primeros MUDs, y salas de chat de universidades. Antes de WhatsApp, Discord o Slack, estos **servidores "bare-metal"** en C eran la columna vertebral de las primeras comunidades online.

**Características principales:** Arquitectura **event-driven** basada en `select()`, asignación automática de IDs secuenciales (0, 1, 2...), gestión transparente de conexiones/desconexiones, y protocolo de comunicación minimalista con delimitadores `\n`. Diseñado para ser **no bloqueante** y robusto ante fallos de red o clientes lentos.

## ✨ Características

- ✅ Múltiples conexiones simultáneas
- ✅ Multiplexación con `select()` sin hilos
- ✅ Broadcast automático de mensajes
- ✅ Gestión automática de conexiones/desconexiones
- ✅ Protocolo TCP/IP en localhost (127.0.0.1)
- ✅ IDs únicos secuenciales para cada cliente
- ✅ Operación no bloqueante

## 📋 Especificaciones

### Argumentos y Validación
- **Puerto requerido**: El programa toma como argumento el puerto de escucha
- **Red específica**: Solo escucha en la red 127.0.0.1
- **Validación estricta**: 
  - Sin argumentos → "Wrong number of arguments"
  - Error de sistema → "Fatal error"
  - Error de memoria → "Fatal error"

### Comportamiento del Servidor
- **Asignación de IDs**: El primer cliente recibe ID 0, los siguientes ID+1
- **Mensajes del sistema**:
  - Conexión: `"server: client %d just arrived\n"`
  - Desconexión: `"server: client %d just left\n"`
- **Retransmisión**: Mensajes de clientes precedidos por `"client %d: "`

### Restricciones Técnicas
- ❌ Sin `#define` de preprocesador
- ❌ Sin bloqueo en operaciones de red
- ❌ Sin desconexión por clientes lentos
- ✅ Solo funciones permitidas del sistema
- ✅ Gestión de memoria sin leaks

## 🚀 Compilación y Uso

```bash
# Compilar (compatible con cc del campus)
cc -Wall -Wextra -Werror mini_serv.c -o mini_serv
# También funciona con:
gcc -Wall -Wextra -Werror mini_serv.c -o mini_serv

# Ejecutar (requiere puerto como argumento)
./mini_serv 8080

# Conectar múltiples clientes
nc 127.0.0.1 8080
```

### ⚡ Funciones Permitidas
`write`, `close`, `select`, `socket`, `accept`, `listen`, `send`, `recv`, `bind`, `strstr`, `malloc`, `realloc`, `free`, `calloc`, `bzero`, `atoi`, `sprintf`, `strlen`, `exit`, `strcpy`, `strcat`, `memset`

## 💡 Ejemplo de Funcionamiento

```bash
# Terminal 1 - Servidor
$ ./mini_serv 8080

# Terminal 2 - Cliente 0 (primer cliente conectado)
$ nc 127.0.0.1 8080
# (No ve nada hasta que llegue otro cliente)

# Terminal 3 - Cliente 1 (segundo cliente conectado)  
$ nc 127.0.0.1 8080
server: client 1 just arrived

# Ahora en Terminal 2 - Cliente 0 aparece:
server: client 1 just arrived

# Cliente 0 escribe:
Hola desde cliente 0

# Cliente 1 ve:
client 0: Hola desde cliente 0

# Cliente 1 escribe:
Mensaje desde cliente 1  

# Cliente 0 ve:
client 1: Mensaje desde cliente 1

# Si cliente 0 se desconecta, cliente 1 ve:
server: client 0 just left
```

## 📁 Archivos

- `mini_serv_v1.c` - Implementación principal optimizada
- `mini_serv_v2.c` - Segunda implementación alternativa
- `test_00.sh` - Script de prueba básico con 100 clientes
- `test-chat-100-clientes.sh` - Script de prueba avanzado para 200 clientes

## 🏗️ Arquitectura

El servidor implementa un **patrón Reactor** con un loop principal basado en `select()` que:

1. **Acepta** nuevas conexiones en el socket servidor
2. **Lee** mensajes de clientes existentes de forma no bloqueante
3. **Retransmite** cada mensaje a todos los demás clientes conectados
4. **Gestiona** desconexiones automáticamente y notifica al resto

### 🔧 Componentes Clave

- **`select()`**: Multiplexación de E/S para manejar múltiples descriptores
- **Socket servidor**: Acepta conexiones en 127.0.0.1:puerto
- **Array de clientes**: Gestión de conexiones activas con IDs únicos
- **Buffers**: Acumulación y formateo de mensajes
- **Sistema de broadcast**: Retransmisión eficiente a todos los clientes

### 📡 Protocolo de Comunicación

```
Cliente conecta    → server: client X just arrived
Cliente envía      → client X: [mensaje]
Cliente desconecta → server: client X just left
```

## 🎓 Conceptos Técnicos

- **Socket programming** TCP/IP en localhost
- **I/O multiplexing** con `select()` no bloqueante
- **Event-driven architecture** sin hilos
- **Network protocol design** minimalista
- **Buffer management** para mensajes parciales
- **Client lifecycle management**

## ✅ Cumplimiento del Subject

### 📋 Verificación Exhaustiva de Requisitos

| Requisito | Estado | Implementación |
|-----------|--------|----------------|
| **Puerto como argumento** | ✅ | `if(argc != 2) err("Wrong number of arguments")` |
| **Escucha en 127.0.0.1** | ✅ | `htonl(2130706433)` (127.0.0.1 en network byte order) |
| **Mensaje sin argumentos** | ✅ | `"Wrong number of arguments"` exacto |
| **Error de sistema** | ✅ | `err(NULL)` → `"Fatal error"` para socket/bind/listen |
| **Sin #define** | ✅ | Código verificado sin directivas de preprocesador |
| **Operación no bloqueante** | ✅ | `select()` antes de todas las operaciones de E/S |
| **ID secuencial** | ✅ | `clients[c].id = next_id++` (0, 1, 2, ...) |
| **Mensaje llegada** | ✅ | `"server: client %d just arrived\n"` exacto |
| **Mensaje salida** | ✅ | `"server: client %d just left\n"` exacto |
| **Formato cliente** | ✅ | `"client %d: %s\n"` exacto |
| **Gestión memoria** | ✅ | `bzero()` para limpieza, sin memory leaks |
| **Solo funciones permitidas** | ✅ | Verificado exhaustivamente |
| **Buffer management** | ✅ | Acumulación de mensajes hasta `\n` |
| **Sin desconexión forzada** | ✅ | No hay timeouts ni desconexiones por lentitud |
| **Compatibilidad compilador** | ✅ | Optimizado para `cc` del campus (sin compound literals) |

### 🔍 Funciones y constantes Utilizadas (Todas Permitidas)
```c
// Todas las funciones están en la lista permitida del subject
write()     // Error messages y funciones auxiliares
close()     // Cleanup de conexiones
select()    // Multiplexación principal: monitorea múltiples sockets simultáneamente
socket()    // Creación del servidor: establece punto de comunicación TCP
accept()    // Nuevas conexiones: acepta clientes y crea socket individual
listen()    // Modo escucha: pone servidor en espera de conexiones entrantes
send()      // Broadcast de mensajes: envía datos a través de sockets TCP
recv()      // Recepción de datos: lee mensajes entrantes desde clientes
bind()      // Vinculación al puerto: asigna IP:puerto específico al socket
strlen()    // Longitud de strings
exit()      // Terminación en errores
bzero()     // Limpieza de buffers
atoi()      // Conversión puerto string→int
vsprintf()  // Formateo de strings con argumentos variádicos

// Constantes y macros del sistema
fd_set         // Estructura: conjunto de descriptores para select()
FD_ISSET()     // Macro: verifica si descriptor está activo en conjunto
FD_SET()       // Macro: añade descriptor al conjunto
FD_CLR()       // Macro: elimina descriptor del conjunto
FD_ZERO()      // Macro: limpia todos los descriptores del conjunto
AF_INET        // Constante: familia de direcciones IPv4
SOCK_STREAM    // Constante: tipo de socket TCP confiable
htons()        // Función: convierte puerto a network byte order
htonl()        // Función: convierte IP a network byte order
sizeof()       // Operador: obtiene tamaño en bytes de estructura/tipo
sockaddr_in    // Estructura: dirección de socket IPv4
socklen_t      // Tipo: tamaño de estructura de dirección
```

### 🛡️ Robustez Implementada
- **Error handling**: Todos los syscalls verificados
- **Buffer overflow protection**: Límites estrictos (370000 - 1)
- **Connection cleanup**: `FD_CLR()`, `close()`, `bzero()`
- **Partial message handling**: Acumulación hasta `\n`
- **Dynamic max_fd**: Optimización de `select()`
- **Memory management**: Arrays estáticos, sin malloc dinámico
- **Compiler compatibility**: Optimizado para `cc` del campus (C89/C99)

---
** Proyecto educativo - Exam Rank 6 - jumarque 42 Málaga (2026) **
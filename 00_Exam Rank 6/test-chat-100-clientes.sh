#!/bin/bash
PORT="${1:-8081}"
NCLIENTS=${NUM_CLIENTS:-200}

cleanup(){ 
    echo -e "\n[!] Limpiando procesos..."
    pkill mini_serv nc >/dev/null 2>&1
    sleep .3 
}
trap cleanup EXIT

echo "=== Mini Serv Multi-Client Test ==="
# Asegúrate de que el binario tiene permisos de ejecución
chmod +x ./mini_serv 2>/dev/null

./mini_serv "$PORT" & s=$! 
sleep .5

# Lanzar los clientes en segundo plano
for i in $(seq 1 $NCLIENTS); do
    # Usamos TCP estándar y un sleep para mantener la conexión abierta un momento
    (printf "[CLI$i] Hola sala!\n"; sleep 2) | nc localhost "$PORT" &
done

# Esperamos a que los clientes terminen (los 2 segundos del sleep)
sleep 2.5

echo "=== Estado del servidor ==="
# Corregido 'ss-tlnp' por 'ss -tlnp' (con espacio)
ss -tlnp | grep "$PORT" || true

# Matamos el servidor explícitamente si sigue vivo antes del trap
kill $s >/dev/null 2>&1
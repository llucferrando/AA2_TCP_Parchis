# AA2_TCP_Parchis

OBSERVACIONES:
-Tenemos un repositorio solamente porque creíamos que era mucho más óptimo 
tenerlo todo bajo una misma solución y dos proyectos distintos, el server y el client.
-Hemos montado un sistema intentando replicar ECS de unity y UnityEvents
-Para hacer funcionar el server hay que crear una data base, llamada videogame en local y para poder utilizar bien la ip hay que cambiarla manualmente, en nuestro caso la tenemos en local por default,
se encuentra en el GameManager.cpp en la línea 95, _client->ConnectToBoostrapServer(ip deseada)

COSAS QUE NO ESTÁN IMPLEMENTADAS:
-No hay sistema de barreras
-Si un cliente se va de la partida, se limpia bien la lista del cliente pero no se
puede seguir jugando.
- Muy de vez en cuando crashea pero no hemos podido encontrar el error de ninguna manera. Creemos
que tiene algo que ver con el puntero a algun componente (al hacer clic un boton o cosas del estilo)
pero no suele ocurrir.

INTEGRANTES:

Lluc Ferrando
Roger Companys
Nahuel Aparicio

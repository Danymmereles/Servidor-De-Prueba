/*
 ============================================================================
 Name        : ServidorDePrueba.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int puerto = 7770;
int creandoSocket();
int bindearSocket(int);

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	int socketAConectar = creandoSocket();
	//Es el socket que va a estar a la escucha de los clientes

	if (socketAConectar == -1){
		return 1;
		//Por lo que acabara la funcion ya que hubo un error en la creacion
	} else {
		printf ("Socket creado \n");
	}

	int conexion = bindearSocket(socketAConectar);

	if (conexion == -1){
		return 1;
	} else {
		printf ("Enlace realizado\n");
	}

	printf ("Estoy escuchando\n");
	listen(socketAConectar, 10);
	//Estamos usando listen para saber el maximo de conexiones acumulables
	//Estoy armando la cola de conexiones que se puede tener

	struct sockaddr_in direccionCliente;
	unsigned int tamanioDireccion;
	int cliente = accept(socketAConectar, (void*) &direccionCliente, &tamanioDireccion);
	//Accept pueda identificar mediante esas dos variables quien se conexto

	printf("Recibi una conexion en %d\n\n", cliente);
	//Devuelve el identificador del cliente que se conecto

	send(cliente, "Ya me conecte al fin", 20, 0);
	//Le paso el mensaje mas el buffer que ocupa. El ultimo es 0 ya que no me interesa las flags.

	close();

	return EXIT_SUCCESS;
}

int creandoSocket (){

	int socketCreado = socket(PF_INET, SOCK_STREAM, 0);
	//PF_INET para dos compus en distintos lugares
	//SOCK_STREAM flujo de datos que este en la red

	if (socketCreado == -1){
		printf ("Error al abrir socket\n");
	} else {
		printf ("Creando socket...\n");
	}

	return socketCreado;
}

int bindearSocket (int socketParaBindear){
	//Es una structura para almacenar la informacion a donde nos vamos a conectar
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = INADDR_ANY;
	direccionServidor.sin_port = htons(puerto);

	//Como hay problemas de conexion con el puerto en el caso que la conexion se haya cerrado incorrectamente
	//tenemos que decirle a la conexion con este puerto que es reutilizable para que justamente no tengamos que
	//esperar 2 min hasta volver a conectarnos

	int reutilizable = 1;
	setsockopt(socketParaBindear, SOL_SOCKET, SO_REUSEADDR, &reutilizable, sizeof(reutilizable));

	int conexion = bind(socketParaBindear, (struct sockaddr*) &direccionServidor, sizeof(direccionServidor));

	if (conexion == -1){
		printf ("No se pudo enlazar con el puerto elegido, puerto en uso\n");
	} else {
		printf ("Terminando el enlace\n");
	}

	return conexion;
}

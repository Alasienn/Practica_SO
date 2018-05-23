#include <pthread.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>

#define MAX_PROCESOS 1
#define NUM_HILOS 32
#define TAMHASH 104729

pthread_mutex_t mutexG;

FILE *f1;
FILE *fso; //Se crea el apuntador del objeto FILE
int creg;

struct dogType {
    char nombre[32];
    char tipo[32]; //Nombre vulgar del especimen ej.Gato,Perro
    int edad;
    char raza[16];
    int estatura; //Tomado en cm
    float peso; //Masa en kg
    char sexo[1]; //H/M
	int del;//indica si el archivo debe ser borrado
};

int recibirs(void *ap) {
    struct dogType *dato;
    dato = ap;
    char val = 'g';
    while (val != 'm' && val != 'M' && val != 'h' && val != 'H') {
        printf("Sexo(h/m): ");
        scanf("%s", dato->sexo);
        val = dato->sexo[0];
    }
}

int recibir(void *ap) {
    struct dogType *dato;
    dato = ap;
    int i;
    for (i = 0; i < 32; i++) {
        dato->nombre[i] = ' ';
    }
    printf("Nombre: ");
    scanf("%s", dato->nombre); //Dado que es una arreglo es de por si un apuntador
    printf("Tipo: ");
    scanf("%s", dato->tipo); //Dado que es una arreglo es de por si un apuntador
    printf("Edad: ");
    scanf("%i", &dato->edad);
    printf("Raza: ");
    scanf("%s", dato->raza); //Dado que es una arreglo es de por si un apuntador
    printf("Estatura(cm): ");
    scanf("%i", &dato->estatura);
    printf("Peso(Kg): ");
    scanf("%f", &dato->peso);
    recibirs(ap);
}

int recibir2(void *ap) {
    struct dogType *dato;
    dato = ap;
    int i;
    for (i = 0; i < 32; i++) {
        dato->nombre[i] = ' ';
    }
    //fgets(dato->nombre,32,fso);
    //fgets(dato->tipo,32,fso);
    fscanf(fso, "%s", dato->nombre); //Dado que es una arreglo es de por si un apuntador
    fscanf(fso, "%s", dato->tipo); //Dado que es una arreglo es de por si un apuntador
    fscanf(fso, "%i", &dato->edad);
    //fgets(dato->raza,32,fso);
    fscanf(fso, "%s", dato->raza); //Dado que es una arreglo es de por si un apuntador
    fscanf(fso, "%i", &dato->estatura);
    fscanf(fso, "%f", &dato->peso);
    fscanf(fso, "%s", dato->sexo);

}

int aguante() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    printf("Presione cualquier tecla... \n");
    getchar();
}

int imprimir(void *ap) {
    struct dogType *dato;
    dato = ap;
    printf("Nombre: %s\n", dato->nombre);
    printf("Tipo: %s\n", dato->tipo);
    printf("Edad: %i\n", dato->edad);
    printf("Raza: %s\n", dato->raza);
    printf("Estatura: %i\n", dato->estatura);
    printf("Peso: %f\n", dato->peso);
    printf("Sexo: %s\n", dato->sexo);
}

typedef struct {
    int *datos;
    int num_datos;
} datos_t;

typedef struct {
    int k[7];
    int val;
    char nomarch[32];
} hash_datos_t;

typedef struct {
    hash_datos_t *datos;
    int num_datos;
    int num_max_datos;
} hash_node_t;

inline static int hash_key(char *k) {
    int sum = 0;
    int i;
    for (i = 0; k[i] != 0; i++) {
        if (k[i] > 64 && k[i] < 91) {
            k[i] = k[i] + 32;
            sum = ((i + 1) * k[i]) + sum;
            //printf("%d  %c %d\n",sum,k[i],k[i]);
        }
    }
    return (sum + 100000) % TAMHASH;
}

hash_node_t *hash_create(void) {
    hash_node_t *hash;

    hash = malloc(sizeof (hash_node_t) * TAMHASH);
    int i;
    for (i = 0; i < TAMHASH; i++) {
        hash[i].datos = malloc(sizeof (hash_datos_t) * 4);
        hash[i].num_datos = 0;
        hash[i].num_max_datos = 4;
    }

    return hash;
}

inline static void hash_add(hash_node_t *hash, void *ap, int valor, char *x) {
    int ind;
    int i;
    struct dogType *dato;
    dato = ap;
    char *p = dato->nombre;
    ind = hash_key(p);
    // comprobamos si existe
    for (i = 0; i < hash[ind].num_datos; i++) {
        if (hash[ind].datos[i].k[0] == p[0] &&
                hash[ind].datos[i].k[1] == p[1] &&
                hash[ind].datos[i].k[2] == p[2] &&
                hash[ind].datos[i].k[3] == p[3] &&
                hash[ind].datos[i].k[4] == p[4] &&
                hash[ind].datos[i].k[5] == p[5] &&
                hash[ind].datos[i].k[6] == p[6] &&
                hash[ind].datos[i].k[7] == p[7])
            break;

    }

    if (i == hash[ind].num_datos) {
        // no se ha encontrado la información
        // creamos un elemento nuevo
        if (hash[ind].num_datos == hash[ind].num_max_datos) {
            //si hemos llegado al máximo ampliamos
            hash[ind].num_max_datos += 4;
            hash[ind].datos = realloc(hash[ind].datos, sizeof (hash_datos_t) * hash[ind].num_max_datos);
        }
        hash[ind].datos[i].k[0] = p[0];
        hash[ind].datos[i].k[1] = p[1];
        hash[ind].datos[i].k[2] = p[2];
        hash[ind].datos[i].k[3] = p[3];
        hash[ind].datos[i].k[4] = p[4];
        hash[ind].datos[i].k[5] = p[5];
        hash[ind].datos[i].k[6] = p[6];
        hash[ind].datos[i].k[7] = p[7];

        hash[ind].datos[i].val = valor;
        hash[ind].num_datos++;
    } else {
        // lo hemos encontrado.
        hash[ind].datos[i].val = valor;
    }
}

struct datos_tipo{
	int dato;
	hash_node_t *hsh;
	int CS;
	struct dogType *ap;
}

*proceso(void *datos)
{
	pthread_mutex_lock(&mutexG);//se desbloquea el mutex	
	struct datos_tipo *datos_proceso;
	struct dogType *animal;
	datos_proceso = (struct datos_tipo *) datos;
    socklen_t longc; 
	int a, i, r, pos, p,opc,conexion_cliente;
	hash_node_t *hsh;
	hsh= datos_proceso->hsh;
	animal=datos_proceso->ap;
    //Declaramos una variable que contendrá los mensajes que recibamos
    char buffer[32],msg[32];
	struct sockaddr_in cliente;
	a = datos_proceso->dato;
	/*zona critica */
	longc = sizeof (cliente); //Asignamos el tamaño de la estructura a esta variable
    conexion_cliente = accept(datos_proceso->CS, (struct sockaddr *) &cliente, &longc); //Esperamos una conexion
    if (conexion_cliente < 0) {
        printf("Error al aceptar trafico\n");
        close(datos_proceso->CS);
        exit(1);
    }
    printf("Conectando con %s:%d\n", inet_ntoa(cliente.sin_addr), htons(cliente.sin_port));
	do {
        if (recv(conexion_cliente, buffer, 100, 0) < 0) { 
			//Comenzamos a recibir datos del cliente
		    //Si recv() recibe 0 el cliente ha cerrado la conexion. Si es menor que 0 ha habido algún error.
		    printf("Error al recibir los datos\n");
		    close(datos_proceso->CS);
		    exit(1);
    	}else {
        printf("%s\n", buffer);
        //bzero((char *) &buffer, sizeof (buffer));
        //send(conexion_cliente, "Recibido\n", 32, 0);
        //int i;
		opc=atoi(buffer);
        switch (opc) {
            case 1:
                recibir(animal);
                fseek(f1, 0, SEEK_END); //Se lleva el puntero del archivo al final
                pos = ftell(f1);
                int npos = pos / (sizeof(struct dogType));
                sprintf(msg, "Historia%d", pos);
                if (0 >= (fwrite(animal, sizeof (*animal), 1, f1))){ //Escritura en archivo
                    sprintf(buffer,"\n   Registro no ingresado  \n");
		    		send(conexion_cliente, buffer, 32, 0);
                }else {
                    sprintf(buffer,"\n   Registro ingresado, %i \n", npos);
		    		send(conexion_cliente, buffer, 32, 0);
                    hash_add(hsh, animal, pos, msg);
                    creg = creg + 1;
                }
                break;

			case 2: //system("sudo gedit historia");
				bzero((char *) &buffer, sizeof (buffer));
                sprintf(buffer,"Hay %d registros\n", creg);
				send(conexion_cliente, buffer, 32, 0);
				bzero((char *) &buffer, sizeof (buffer));
                sprintf(buffer,"\n   Ingrese numero de registro a ver:  \n");
				send(conexion_cliente, buffer, 32, 0);
				if (recv(conexion_cliente, buffer, 100, 0) < 0) {
					//Comenzamos a recibir datos del cliente
					//Si recv() recibe 0 el cliente ha cerrado la conexion. Si es menor que 0 ha habido algún error.
					printf("Error al recibir los datos\n");
					close(datos_proceso->CS);
					exit(1);
				}else {
		    		r=atoi(buffer);
				    i = r * sizeof(struct dogType);
				    fseek(f1, i, SEEK_SET);
				    r = fread(animal, sizeof (*animal), 1, f1);
				    if (r== 0) {
				        //system("clear");
						bzero((char *) &buffer, sizeof (buffer));
				        sprintf(buffer,"\n %i   Registro vacio o inexistente\n", r);
						send(conexion_cliente, buffer, 32, 0);
				    }else {
						bzero((char *) &buffer, sizeof (buffer));
				        sprintf(buffer,"\n   Registro existente\n");
						send(conexion_cliente, buffer, 32, 0);
				        imprimir(animal);
				        sprintf(msg, "gedit Historia%d", i);
				        //p se extrae del guardado en
				        system(msg);
				    }
			}
                break;
            case 3: //borrar
				bzero((char *) &buffer, sizeof (buffer));
				sprintf(buffer, "Hay %d registros\n", creg);
				send(conexion_cliente, buffer, 32, 0);
				bzero((char *) &buffer, sizeof (buffer));
				sprintf(buffer, "\n   Ingrese numero de registro a borrar:  \n");
				send(conexion_cliente, buffer, 32, 0);
				if (recv(conexion_cliente, buffer, 100, 0) < 0) {
					//Comenzamos a recibir datos del cliente
		    		//Si recv() recibe 0 el cliente ha cerrado la conexion. Si es menor que 0 ha habido algún error.
		    		printf("Error al recibir los datos\n");
					close(datos_proceso->CS);
					exit(1);
				}else {
					r=atoi(buffer);
					i = r * sizeof(struct dogType);
					fseek(f1, 0, SEEK_END);
					int fpos = ftell(f1);
					if (i < fpos){
						fseek(f1,i,SEEK_SET);
						fread(animal,sizeof(struct dogType),1,f1);
						animal -> del= 0;
						fseek(f1,i,SEEK_SET);
						fwrite(animal,sizeof(struct dogType),1,f1);
						FILE *ftemp;
						ftemp = fopen("temporal.tmp","wb");
						rewind(f1);
						while(fread(animal,sizeof(struct dogType),1,f1))
							if(animal -> del == 1)
								fwrite(animal,sizeof(struct dogType),1,ftemp);
						fclose(ftemp);
						fclose(f1);

						rename("dataDogs.dat", "dataDogs.old");
						rename("temporal.tmp", "dataDogs.dat");
						remove("dataDogs.old");
						f1 = fopen("dataDogs.dat","r+b");
						bzero((char *) &buffer, sizeof (buffer));
						sprintf(buffer, "Eliminación Exitosa\n");
						send(conexion_cliente, buffer, 32, 0);
                		creg = creg - 1;
					}else {//fuera del documento
						bzero((char *) &buffer, sizeof (buffer));
						sprintf(buffer, "\n   Registro no existente\n");
						send(conexion_cliente, buffer, 32, 0);
					}
				}
                break;
            case 4: //buscar
				bzero((char *) &buffer, sizeof (buffer));
                sprintf(buffer,"\n  Ingrese el nombre completo a buscar\n");
				send(conexion_cliente, buffer, 32, 0);
				int y;
				for (y = 0; y < 32; y++) {
					msg[y] = ' ';
				}
                if (recv(conexion_cliente, 	msg, 100, 0) < 0) {
					//Comenzamos a recibir datos del cliente
		    		//Si recv() recibe 0 el cliente ha cerrado la conexion. Si es menor que 0 ha habido algún error.
		    		printf("Error al recibir los datos\n");
					close(datos_proceso->CS);
					exit(1);
				}else {
		            int ind = hash_key(msg);
		            if (hsh[ind].num_datos > 0) {
						bzero((char *) &buffer, sizeof (buffer));
		                sprintf(buffer,"\n Registro(s) Encontrado(s) \n");
						send(conexion_cliente, buffer, 32, 0);
						r=hsh[ind].num_datos;
						bzero((char *) &buffer, sizeof (buffer));
		                sprintf(buffer,"%d",r);
						send(conexion_cliente, buffer, 32, 0);
		                for (i = 0; i < r; i++) {
		                    int ret = hsh[ind].datos[i].val;
							bzero((char *) &buffer, sizeof (buffer));
		                    sprintf(buffer,"%lu \n", ret / sizeof (struct dogType));
							send(conexion_cliente, buffer, 32, 0);
		                }
		            }else {
						bzero((char *) &buffer, sizeof (buffer));
		                sprintf(buffer,"\n Registros  no encontrados \n");
						send(conexion_cliente, buffer, 32, 0);
						send(conexion_cliente, "0", 32, 0);
		            }
				}
                //imprimir(animal);//esto mostraria el ultimo valor guardado en animal
                break;
            default:break;
        }

    }
	}while (opc != 5);
/* fin zona */
	pthread_mutex_unlock(&mutexG);//se desbloquea el mutex
}

int main(int argc, char **argv){
	int error, i, opc, pos, his;
    char auxch[32];
	char *valor_devuelto;
/* Variables para hilos*/
	struct datos_tipo hilo_datos[NUM_HILOS];
	pthread_mutex_init(&mutexG, NULL);//Se inicia
	pthread_t idhilo[NUM_HILOS];
	if (argc < 2) { 
	//Especifica los argumentos
        printf("%s <puerto>\n", argv[0]);
        return 1;
    }
    //Declaramos las variables
    //Debemos declarar una variable que contendrá la longitud de la estructura
    int conexion_servidor, conexion_cliente, puerto; 
    struct sockaddr_in servidor, cliente; 
    puerto = atoi(argv[1]);
    //Creamos el socket
    conexion_servidor = socket(AF_INET, SOCK_STREAM, 0); 
    //Llenamos la estructura de 0's
    bzero((char *) &servidor, sizeof (servidor)); 
    //Asignamos a la estructura
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(puerto);
    //Esta macro especifica nuestra dirección
    servidor.sin_addr.s_addr = INADDR_ANY; 
    if (bind(conexion_servidor, (struct sockaddr *) &servidor, sizeof (servidor)) < 0) { //asignamos un puerto al socket
        printf("Error al asociar el puerto a la conexion\n");
        close(conexion_servidor);
        return 1;
    }
	struct dogType *animal; //Se crea un apuntador de la estructura dogType
    animal = malloc(sizeof (struct dogType)); //Se reserva un espacio en memoria de el tamaño de la estructura dogType
    f1 = fopen("dataDogs.dat", "a+b");
    fso = fopen("source", "r"); //Abre el fichero y guarda el puntero
    if (!f1) //Revisa que el archivo se hubiera abierto(exista)
        f1 = fopen("dataDogs.dat", "wb"); //Si no existe lo crea(en el caso de que existier lo sobreescribe)
    if (!fso) //Revisa que el archivo se hubiera abierto(exista)
        printf("\n   no hay fuente inicial  \n"); //Si no existe lo crea(en el caso de que existiera lo sobreescribe)
    hash_node_t *hsh;
    hsh = hash_create();
    for (i = 0; i < 1000; i++) {//insecion prev
        recibir2(animal);
        fseek(f1, 0, SEEK_END); //Se lleva el puntero del archivo al final
        pos = ftell(f1);
        int npos = pos / sizeof(struct dogType);
        sprintf(auxch, "Historia%d", pos);
	if (0 >= (fwrite(animal, sizeof (*animal), 1, f1))) //Escritura en archivo
                    printf("\n   Registro no ingresado  \n");
                else {
                    //printf("\n   Registro ingresado, %i \n", npos);
                    hash_add(hsh, animal, pos, auxch);
                    creg = creg + 1;
                }
    }
	for(i=0; i<NUM_HILOS; i++){
		hilo_datos[i].dato = i;
		hilo_datos[i].hsh=hsh;
		hilo_datos[i].CS=conexion_servidor;
		hilo_datos[i].ap=animal;
	}
	listen(conexion_servidor, NUM_HILOS); //Estamos a la escucha
	printf("A la escucha en el puerto %d\n", ntohs(servidor.sin_port));
	/*se lanzan los hilos*/
	for(i=0; i<NUM_HILOS; i++){//crea y ejecuta los hilos
		error=pthread_create(&idhilo[i], NULL, (void *)proceso, (void *)(&hilo_datos[i]));
	//sleep(1);
		if (error != 0){//validacion de errores
			perror("No puedo crear hilo");
			exit(-1);
		}	
	}
	/*Esperar a que terminen*/
	for(i=0; i<NUM_HILOS; i++){//supervisa que los hilos hubieran terminado
		pthread_join(idhilo[i], (void **)&valor_devuelto);
	}
	for (i = 0; i < TAMHASH; i++) {
        free(hsh[i].datos);
    }
	close(conexion_servidor);
	pthread_mutex_destroy(&mutexG);//Se destruye el mutex
    free(hsh);
    free(animal); //Se libera el espacio antes reservado con malloc
    fclose(f1);
    system("rm dataDogs.dat");
    fclose(fso); //Se cierra el archivo
	return 0;
}
		send(conexion_cliente, buffer, 32, 0);

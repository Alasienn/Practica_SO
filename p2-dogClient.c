#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

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
    scanf("%31s", dato->nombre); //Dado que es una arreglo es de por si un apuntador
    printf("Tipo: ");
    scanf("%31s", dato->tipo); //Dado que es una arreglo es de por si un apuntador
    printf("Edad: ");
    scanf("%i", &dato->edad);
    printf("Raza: ");
    scanf("%15s", dato->raza); //Dado que es una arreglo es de por si un apuntador
    printf("Estatura(cm): ");
    scanf("%i", &dato->estatura);
    printf("Peso(Kg): ");
    scanf("%f", &dato->peso);
    recibirs(ap);
	dato->del=1;
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
	dato->del=1;

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

int menu() {
    printf("\n.:*************:.");
    printf("\n   1. Ingresar registro.");
    printf("\n   2. Ver registro.");
    printf("\n   3. Borrar Registro.");
    printf("\n   4. Buscar Registro.");
    printf("\n   5. Salir.");
    printf("\n   ________________________");
    printf("\n\n   Introduzca opción (1-5): ");

}

#define TAMHASH 104729

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
    //se cambio el condicional ojo con eso
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
    //printf("%d \n",ind);
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
        //memcpy(hash[ind].datos[i].nomarch, x, sizeof (&x));
        hash[ind].num_datos++;
    } else {
        // lo hemos encontrado.
        hash[ind].datos[i].val = valor;
    }
}

int main() {
    struct dogType *animal; //Se crea un apuntador de la estructura dogType
    animal = malloc(sizeof (struct dogType)); //Se reserva un espacio en memoria de el tamaño de la estructura dogType
    f1 = fopen("dataDogs.dat", "a+b");
    fso = fopen("source", "r"); //Abre el fichero y guarda el puntero
    int opc, aux, pos, his, i;
    char nomarch[32], find[32];
    int y;
    for (y = 0; y < 32; y++) {
        find[y] = ' ';
    }
    if (!f1) //Revisa que el archivo se hubiera abierto(exista)
        f1 = fopen("dataDogs.dat", "wb"); //Si no existe lo crea(en el caso de que existier lo sobreescribe)
    if (!fso) //Revisa que el archivo se hubiera abierto(exista)
        printf("\n   no hay fuente inicial  \n"); //Si no existe lo crea(en el caso de que existiera lo sobreescribe)
    hash_node_t *hsh;
    hsh = hash_create();
    for (i = 0; i < 1000; i++) {
        recibir2(animal);
        fseek(f1, 0, SEEK_END); //Se lleva el puntero del archivo al final
        pos = ftell(f1);
        int npos = pos / sizeof(struct dogType);
        sprintf(nomarch, "Historia%d", pos);
	if (0 >= (fwrite(animal, sizeof (*animal), 1, f1))) //Escritura en archivo
                    printf("\n   Registro no ingresado  \n");
                else {
                    //printf("\n   Registro ingresado, %i \n", npos);
                    hash_add(hsh, animal, pos, nomarch);
                    creg = creg + 1;
                }
    }
    do {
        menu();
        scanf("%d", &opc);
        system("clear");
        int i;

        switch (opc) {
            case 1:
                recibir(animal);
                fseek(f1, 0, SEEK_END); //Se lleva el puntero del archivo al final
                pos = ftell(f1);
                int npos = pos / (sizeof(struct dogType));
                sprintf(nomarch, "Historia%d", pos);
                if (0 >= (fwrite(animal, sizeof (*animal), 1, f1))) //Escritura en archivo
                    printf("\n   Registro no ingresado  \n");
                else {
                    printf("\n   Registro ingresado, %i \n", npos);
                    hash_add(hsh, animal, pos, nomarch);
                    creg = creg + 1;
                }
                aguante();
                system("clear");
                break;

            case 2: //system("sudo gedit historia");
                printf("Hay %d registros\n", creg);
                printf("\n   Ingrese numero de registro a ver:  \n");
                scanf("%d", &his);
                i = his * sizeof(struct dogType);
                fseek(f1, i, SEEK_SET);
                aux = fread(animal, sizeof (*animal), 1, f1);
                if (aux == 0) {
                    system("clear");
                    printf("\n %i   Registro vacio o inexistente\n", aux);
                } else {
                    printf("\n   Registro existente\n");
                    imprimir(animal);
                    sprintf(nomarch, "gedit Historia%d", i);
                    //p se extrae del guardado en
                    system(nomarch);
                }
                    aguante();
                    system("clear");
                break;
            case 3: //borrar
                printf("Hay %d registros\n", creg);
                printf("\n   Ingrese numero de registro a borrar:  \n");
                scanf("%d", &his);
                i = his * sizeof(struct dogType);
                fseek(f1, 0, SEEK_END);
                int fpos = ftell(f1);
                creg = creg - 1;
		if (i < fpos) {
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

			printf("Zona critica\n");
			rename("dataDogs.dat", "dataDogs.old");
			rename("temporal.tmp", "dataDogs.dat");
			remove("dataDogs.old");
			f1 = fopen("dataDogs.dat","r+b");
			printf("Eliminación Correcta\n");
                    }
		else {//fuera del documento
                        printf("\n   Registro no existente\n");
                        creg = creg + 1;
                    }
		aguante();
		system("clear");
                break;
            case 4: //buscar
                printf("\n  Ingrese el nombre completo a buscar\n");
                scanf("%s", &find[32]);
                int ind = hash_key(find);
                if (hsh[ind].num_datos > 0) {
                    printf("\n Registro(s) Encontrado(s) \n");
                    for (i = 0; i < hsh[ind].num_datos; i++) {
                        int ret = hsh[ind].datos[i].val;
                        printf("%lu \n", ret / sizeof (struct dogType));
                    }
                } else {
                    printf("\n Registros  no encontrados \n");
                }

                //imprimir(animal);//esto mostraria el ultimo valor guardado en animal
                break;
            default:break;
        }

    } while (opc != 5);
    for (i = 0; i < TAMHASH; i++) {
        free(hsh[i].datos);
    }
    free(hsh);
    free(animal); //Se libera el espacio antes reservado con malloc
    fclose(f1);
    system("rm dataDogs.dat");
    fclose(fso); //Se cierra el archivo
    return 0;
}

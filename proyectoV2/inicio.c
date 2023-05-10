#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <semaphore.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>


int main(int argc,char *argv[]){

     
    char entrada [50], numeros [3];
    int i, j, k, cont, nNodosAux, numProcesosAux,nProcConPrio;
    int numPrioridades = 5;
   
    FILE * ficheroIn = fopen (argv [1], "r");


    if (ficheroIn == NULL) {

        printf ("Error:Fichero de entrada no encontrado. \n\n");
        return 0;
    }



    fgets (entrada, 50, ficheroIn);


     do {

        char variable [15] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};

        for (i = 0, cont = 0; entrada [i] != '='; i++) {

            variable [i] = entrada [i];
            cont ++;
        }
                   


        memcpy (numeros, &entrada [cont + 1], 3);
        i = atoi (numeros);

        if (strcmp (variable, "nNodos") == 0) {

            nNodosAux = i;

            printf ("nNodos = %i\n", nNodosAux);
         
        } else if (strcmp (variable, "numProcesos") == 0) {

            numProcesosAux = i;
            printf ("numProcesos = %i\n", numProcesosAux);
        }


        fgets (entrada, 50, ficheroIn);

    } while (entrada [0] != '\n');
                         
 


    int procReceptor[nNodosAux];

    for (i = 1; i < nNodosAux+1; i++) {
       

       procReceptor [i] = fork ();

        if (procReceptor [i] == 0) {
          
            char iAux [2];
            sprintf (iAux, "%i", i);

            execl ("receptor", "receptor",iAux, (char *) NULL);
                               
        }

    }



   
     for (i = 0; i < nNodosAux ; i++) {

        char iAux [5];
        sprintf (iAux, "%i", i);
        int procHijo[numProcesosAux*numPrioridades];


        for (k = 0; k < numProcesosAux; k++) {
            for(i = 0 ; i < numPrioridades; i++){

                procHijo[k+i] = fork ();

                if (procHijo[k+i] == 0) {
                    if(i == 0){
                         execl ("pagos", "pagos",iAux, (char *) NULL);
                         
                        

                    }
                    if( i == 1){
                        execl ("reservas", "reservas",iAux, (char *) NULL);



                    }
                    if( i == 2){
                        execl ("anulaciones", "anulaciones",iAux, (char *) NULL);

                    }
                    if( i == 3){
                        execl ("administracion", "administracion",iAux, (char *) NULL);

                        
                      
                    }

                    //execl ("consultas", "consultas",iAux, (char *) NULL);

                    return 0;
                }

               
            }
        }

    }

    fclose (ficheroIn);
   
    return 0;


}

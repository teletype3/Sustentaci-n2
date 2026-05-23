#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *chequeo_columna(void *data);
void *chequeo_fila(void *data);
void *chequeo_subcuadricula(void *data);

int cuadricula[9][9];
int valida_rcolu_rfil_rsubc[11] = {1,1,1,1,1,1,1,1,1,1,1};
int error_rcolu_rfil_rsubc[11] = {0,0,0,0,0,0,0,0,0,0,0};
typedef struct
{
    int fila;
    int columna;
    int id_thread;
} parametros; 

void *chequeo_columna(void *data){
    for(int f=0; f<9; f++){
        for(int c=0; c<9; c++){                          
            for(int p=0; p<9; p++){                
                if(cuadricula[c][f] == cuadricula[p][f] && c!=p){              
                    valida_rcolu_rfil_rsubc[0]=0;
                    error_rcolu_rfil_rsubc[0]=f+1;
                    pthread_exit(0);                    
                }
            }            
        }                   
    }    
}

void *chequeo_fila(void *data){
    for(int f=0; f<9; f++){
        for(int c=0; c<9; c++){              
            for(int p=0; p<9; p++){     
                if(cuadricula[f][c] == cuadricula[f][p] && c!=p){              
                    valida_rcolu_rfil_rsubc[1]=0;
                    error_rcolu_rfil_rsubc[1]=f+1;
                    pthread_exit(0);                    
                }
            }            
        }      
    }    
}

void *chequeo_subcuadricula(void *data){    
    parametros *datos =  (parametros*)data;
    int exponenete[9] = {0,0,0,0,0,0,0,0,0};    
    for(int f=datos->fila; f<(datos->fila)+3; f++){
        for(int c=datos->columna; c<(datos->columna)+3; c++)
            exponenete[cuadricula[f][c]-1]=exponenete[cuadricula[f][c]-1]+1;
    }

    for(int t=0; t<9;t++){
        if(exponenete[t]>1){
            valida_rcolu_rfil_rsubc[datos->id_thread]=0;
            error_rcolu_rfil_rsubc[datos->id_thread]=t+1;              
            pthread_exit(0);                    
        }        
    }
    pthread_exit(0);                    
}

int main(int argc, char *argv[]) {

    for(int r=0; r<9; r++){
        for(int c=0; c<9; c++)
            cuadricula[r][c] = argv[r+1][c]-'0';                            
    }
    
    printf("\nSudoku Ingresado:\n");
    for(int r=0; r<9; r++){
        for(int c=0; c<9; c++){
            printf(" %d ",cuadricula[r][c]);        
            if(c==2 || c==5)printf("|");
        }        
        printf("\n");
        if(r==2 || r==5)printf(" ─  ─  ─   ─  ─  ─   ─  ─  ─\n");
        
        
    }          

    parametros *data_subc1 = (parametros *) malloc(sizeof(parametros));
    parametros *data_subc2 = (parametros *) malloc(sizeof(parametros));
    parametros *data_subc3 = (parametros *) malloc(sizeof(parametros));
    parametros *data_subc4 = (parametros *) malloc(sizeof(parametros));
    parametros *data_subc5 = (parametros *) malloc(sizeof(parametros));
    parametros *data_subc6 = (parametros *) malloc(sizeof(parametros));
    parametros *data_subc7 = (parametros *) malloc(sizeof(parametros));
    parametros *data_subc8 = (parametros *) malloc(sizeof(parametros));
    parametros *data_subc9 = (parametros *) malloc(sizeof(parametros));
    
    data_subc1->fila = 0;
    data_subc1->columna = 0;
    data_subc1->id_thread = 2;
    data_subc2->fila = 0;
    data_subc2->columna = 3;
    data_subc2->id_thread = 3;
    data_subc3->fila = 0;
    data_subc3->columna = 6;
    data_subc3->id_thread = 4;
    
    data_subc4->fila = 3;
    data_subc4->columna = 0;
    data_subc4->id_thread = 5;
    data_subc5->fila = 3;
    data_subc5->columna = 3;
    data_subc5->id_thread = 6;
    data_subc6->fila = 3;
    data_subc6->columna = 6;
    data_subc6->id_thread = 7;

    data_subc7->fila = 6;
    data_subc7->columna = 0;
    data_subc7->id_thread = 8;
    data_subc8->fila = 6;
    data_subc8->columna = 3;
    data_subc8->id_thread = 9;
    data_subc9->fila = 6;
    data_subc9->columna = 6;
    data_subc9->id_thread = 10;


    pthread_t hilo[11];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    pthread_create(&hilo[0], &attr, chequeo_columna, NULL);
    pthread_create(&hilo[1], &attr, chequeo_fila, NULL);
    pthread_create(&hilo[2], NULL, chequeo_subcuadricula, data_subc1);
    pthread_create(&hilo[3], NULL, chequeo_subcuadricula, data_subc2);
    pthread_create(&hilo[4], NULL, chequeo_subcuadricula, data_subc3);
    pthread_create(&hilo[5], NULL, chequeo_subcuadricula, data_subc4);
    pthread_create(&hilo[6], NULL, chequeo_subcuadricula, data_subc5);
    pthread_create(&hilo[7], NULL, chequeo_subcuadricula, data_subc6);
    pthread_create(&hilo[8], NULL, chequeo_subcuadricula, data_subc7);
    pthread_create(&hilo[9], NULL, chequeo_subcuadricula, data_subc8);
    pthread_create(&hilo[10], NULL, chequeo_subcuadricula, data_subc9);
    
    
    pthread_join(hilo[0], NULL);
    pthread_join(hilo[1], NULL);

    for(int i =2; i<11; i++)        
        pthread_join(hilo[i], NULL);    
    
    int valido = 1;
    for(int i=0; i<11; i++){
        if(valida_rcolu_rfil_rsubc[i]==0){
            printf("\n¡Sudoku no valido!\n");
            valido = 0;
            break;
        } 
    }

    if(valido==0){
        if(error_rcolu_rfil_rsubc[0]!=0) printf("Ver columna: %d\n", error_rcolu_rfil_rsubc[0]);
        if(error_rcolu_rfil_rsubc[1]!=0) printf("Ver fila: %d\n", error_rcolu_rfil_rsubc[1]);
        for(int j = 2; j<11; j++){
            if(error_rcolu_rfil_rsubc[j]!=0) printf("Ver subcuadricula: %d, por ejemplo el numero %d\n", j-1, error_rcolu_rfil_rsubc[j]);            
        }

    }else{
        printf("\n¡Sudoku valido!\n");
    }     

    return 0;
}
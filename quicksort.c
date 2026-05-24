#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <omp.h>

void intercambio(long long* a, long long* b){
    long long temp = *a;
    *a = *b;
    *b = temp;
}

void selection_sort(long long arr[], unsigned long long inicio, unsigned long long fin){
    for(unsigned long long i=inicio; i<=fin; i++){
        unsigned long long primer_elemento=i;

        for (unsigned long long j=i+1; j<=fin; j++)        
            if (arr[j]<arr[primer_elemento])
                primer_elemento=j;        

        intercambio(&arr[i],&arr[primer_elemento]);
    }
}

unsigned long long secc_izq_der(long long arr[], unsigned long long inicio, unsigned long long fin){
    long long pivote=arr[fin];
    unsigned long long i=inicio-1;

    for (unsigned long long j=inicio; j<fin; j++){
        if (arr[j]<=pivote){
            i++;
            intercambio(&arr[i], &arr[j]);
        }
    }

    intercambio(&arr[i+1], &arr[fin]);
    return i+1;
}

void quicksort(long long arr[], unsigned long long inicio, unsigned long long fin){
    if(inicio>= fin)
        return;
    
    if (((fin-inicio)+1) <= 10)    {
        selection_sort(arr, inicio, fin);
        return;
    }

    unsigned long long nuevo_pivote=secc_izq_der(arr, inicio, fin);

    #pragma omp task
    quicksort(arr, inicio, nuevo_pivote-1);

    #pragma omp task
    quicksort(arr, nuevo_pivote+1, fin);

    #pragma omp taskwait
}

void cargue(long long *lista, unsigned long long tamano){
    int x;    
    for(unsigned long long j =0; j<tamano; j++){
        getrandom(&x, sizeof(x), 0);        
        srand(x);
        lista[j]=rand()%((tamano+1)-(-(tamano+1))+1)+(-(tamano+1));
    }
}

int main(int argc, char *argv[]){
    unsigned long long tamano=0;
    printf("Ingrese el tamaño de la lista: ");
    scanf("%lld",&tamano);    
    long long *arr = malloc(tamano * sizeof(long long));
    cargue(arr, tamano);

    //CARGAR LISTA DESDE LINEA DE COMANDOS
    /*unsigned long long tamano=argc-1;    
    long long *arr = malloc(tamano * sizeof(long long));
    for(unsigned long long i = 1; i < argc; i++) {
        arr[i-1]= atoi(argv[i]);        
    } */   
    
    printf("\nLista generada: \n");
    for(unsigned long long j =0; j<tamano; j++)
        printf("%lld, ", arr[j]);    
    printf("\n");

    for(unsigned long long j =0; j<tamano; j++)
        printf("%lld ",arr[j]);
    printf("\n\n");

    #pragma omp parallel
    {
        #pragma omp single
        {
            quicksort(arr, 0, tamano-1);
        }
    }

    printf("\nLista ordenada: \n");
    for (int i = 0; i < tamano; i++)
        printf("%lld ", arr[i]);
    printf("\n");

    return 0;
}
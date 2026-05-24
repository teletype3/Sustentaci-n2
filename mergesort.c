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

void merge(long long arr[], unsigned long long inicio, unsigned long long mitad, unsigned long long fin){
    
    unsigned long long n1=mitad-inicio+1;
    unsigned long long n2=fin-mitad;

    long long* izq = malloc(n1*sizeof(long long));
    long long* der = malloc(n2*sizeof(long long));

    for (unsigned long long i=0; i<n1; i++)
        izq[i] = arr[inicio+i];
    for (unsigned long long i=0; i<n2; i++)
        der[i] = arr[mitad+1+i];

    unsigned long long i=0;
    unsigned long long j=0;
    unsigned long long k=inicio;

    while (i<n1 && j<n2)
    {
        if (izq[i]<=der[j])
            arr[k++]=izq[i++];
        else
            arr[k++]=der[j++];
    }

    while(i<n1)
        arr[k++]=izq[i++];

    while(j<n2)
        arr[k++] = der[j++];

    free(izq);
    free(der);
}

void mergesort(long long arr[], unsigned long long inicio, unsigned long long fin){

    if(inicio>=fin)
        return;
    
    if (((fin-inicio)+1) <= 10)
    {
        selection_sort(arr,inicio,fin);
        return;
    }

    unsigned long long mitad = (inicio+fin)/2;
    
    #pragma omp task
    mergesort(arr, inicio, mitad);

    #pragma omp task
    mergesort(arr, mitad+1, fin);

    #pragma omp taskwait
    
    merge(arr, inicio, mitad, fin);
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
    }*/   
    
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
            mergesort(arr,0,tamano-1);
        }
    }

    printf("\nLista ordenada: \n");
    for (unsigned long long i = 0; i < tamano; i++)
        printf("%lld ", arr[i]);
    printf("\n");

    

    return 0;
}
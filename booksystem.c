
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "booksystem.h"
#include <ctype.h>

int getnumber(library *lib,int index,int ct){
    char al[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    int i=0;
    for (i=0; i<26;i++){
        if (al[i] == tolower(lib->books[index].autore[ct])) break;
    }
    return i;
}

void merge(wrap arr[], int len, int m, int r) 
{ 
    int n1 = m - len + 1; 
    int n2 = r - m; 
    wrap l1[n1], l2[n2]; 

    for (int i = 0; i < n1; i++) l1[i] = arr[len + i]; 
    for (int j = 0; j < n2; j++) l2[j] = arr[m + 1 + j]; 

    int a = 0; int b=0;
    int k = len;
    while (a < n1 && b < n2) { 
        if (l1[a].index <= l2[b].index) {arr[k] = l1[a];a++;} 
        else                {arr[k] = l2[b]; b++;} 
        k++; 
    } 
    while (a < n1) {arr[k] = l1[a];a++;k++;} 
    while (b < n2) {arr[k] = l2[b];b++;k++;} 
} 

void mergesort(wrap arr[], int len, int r){
    if (len < r) {
        int m = (len+r)/ 2; 
        mergesort(arr, len, m); 
        mergesort(arr, m + 1, r); 
        merge(arr, len, m, r); 
    } 
} 

void waitforit(){
    printf("Premi [INVIO] per tornare al menu'\n");
    char in;
    getchar();
    do{
        scanf("%c",&in);
    }while(in!='\n');
    system("clear");


}

void lineanalysis(library *lib,char *string){
    strcpy(lib->books[lib->size-1].titolo,"");
    int buffer = 0;
    char y[80]="";
    for (int i=0;i<(int)strlen(string);i++){
        if (string[i]=='\t') ++buffer;
        else if (string[i]=='\n'){}//donothing
        else{
            switch (buffer)
            {
            case 0:{strncat(lib->books[lib->size-1].titolo,&string[i],1);break;}
            case 1:{strncat(lib->books[lib->size-1].autore,&string[i],1);break;}
            case 2:{strncat(lib->books[lib->size-1].editore,&string[i],1);break;}
            case 3:{strncat(y,&string[i],1);break;}
            
            default:break;
            }
        }
    }
    lib->books[lib->size-1].anno = atoi(y);
}

book inputbook(){
    book output;
    printf("Inserisci il titolo: ");
    scanf(" %80s", output.titolo);
    printf("Inserisci l'autore: ");
    scanf(" %80s", output.autore);
    printf("Inserisci la casa editrice: ");
    scanf(" %80s", output.editore);
    char buffer[80];
    do{
        
        printf("Inserisci l'anno di pubblicazione: ");
        scanf("%s",buffer);
        output.anno = atoi(buffer);
        if (output.anno<=0)printf("Valore non valido\n");
    }
    while(output.anno<=0);
    return output;
}

void addbook(library *lib,int par,...){
    if (!par) lib->books[lib->size-1] = inputbook();//INSERIMENTO DA PROGRAMMA
       
    else {//INSERIMENTO DA LINEA DI COMANDO
        lib->books = (book*)realloc(lib->books,(++lib->size)* sizeof(book));
        char temp[80];
        va_list l;
        va_start(l,par);
        strcpy(lib->books[lib->size-1].titolo,va_arg(l,const char * ));
        strcpy(lib->books[lib->size-1].autore,va_arg(l,const char * ));
        strcpy(lib->books[lib->size-1].editore,va_arg(l,const char * ));
        strcpy(temp,va_arg(l,const char * ));
        va_end(l);
        if (atoi(temp)==0){
            printf("Errore nell'inserimento dei dati\n");
            exit(1);
        }
        else lib->books[lib->size-1].anno = atoi(temp);
        
        savedata(lib,1);
    }    
    printf("Aggiunta avvenuta con successo\n");
    if (!par)waitforit();
}

void removebook(library *lib,int par,...){
    book todel;
    if (!par) todel = inputbook();//RIMOZIONE DA PROGRAMMA
  
    else{
        fetchdata(lib,1);
        char temp[80];
        va_list l;
        va_start(l,par);
        strcpy(todel.titolo,va_arg(l,const char * ));
        strcpy(todel.autore,va_arg(l,const char * ));
        strcpy(todel.editore,va_arg(l,const char * ));
        strcpy(temp,va_arg(l,const char * ));
        va_end(l);
        if (atoi(temp)==0){
            printf("Errore nell'inserimento dei dati\n");
            exit(1);
        }
        else todel.anno = atoi(temp);
    }
    for (int i=1;i<=(int)lib->size;i++){
        if (!strcmp(lib->books[i].titolo,todel.titolo) && !strcmp(lib->books[i].autore,todel.autore) && !strcmp(lib->books[i].editore,todel.editore) && lib->books[i].anno==todel.anno){
                for (int j=0;j<(int)lib->size;j++){
                    if (j>=i){
                        strcpy(lib->books[j].titolo,lib->books[j+1].titolo);
                        strcpy(lib->books[j].autore,lib->books[j+1].autore);
                        strcpy(lib->books[j].editore,lib->books[j+1].editore);
                        lib->books[j].anno = lib->books[j+1].anno;

                    }
                    
                }
                
                printf("trovato ed eliminato\n");
                lib->books = (book*)realloc(lib->books,(--lib->size)* sizeof(book));
                if (!par)waitforit();
                else savedata(lib,2);
                return;
            }
        }
    printf("Libro non trovato. Ricontrollare i dati inseriti\n");  
    if (!par) waitforit();
}

void findbook(library *lib,int par,...){
    char aut[80];

    if (!par) {printf("Inserisci l'autore: ");scanf("%80s",aut);}//RICERCA DA PROGRAMMA

    else {//RICERCA DA LINEA DI COMANDO
        va_list l;
        va_start(l,par);
        strcpy(aut,va_arg(l,const char * restrict));
        va_end(l);
        fetchdata(lib,1);
    }
    printf("La ricerca di '%s' ha prodotto i seguenti risultati:\n",aut);
    printf("TITOLO\tAUTORE\tEDITORE\tANNO\n");
    for (int i=0;i<(int)lib->size;i++){
        if (!strcmp(lib->books[i].autore,aut)){
            printf("%s\t%s\t%s\t%d\n",
                                lib->books[i].titolo,
                                lib->books[i].autore,
                                lib->books[i].editore,
                                lib->books[i].anno);
                
            }
    }
    if (!par)waitforit();
        

}

void savedata(library *lib, int m){
    if (lib->size>1 || m){
        char path[80]="";
        strcat(path,"library.dat");
        FILE *file ;
        if(!m && access(path,F_OK) != -1 ) {
            printf("Un file di salvataggio esiste gia'. Sovrascrivere?[y/n]: ");getchar();
            if (getchar()=='y') file = fopen(path,"w+");
            else file = fopen(path,"a+");
        }
        else if(m==1 && access(path,F_OK) != -1 )file = fopen(path,"a+");
        else if(m==2 && access(path,F_OK) != -1 )file = fopen(path,"w+");
        else file = fopen(path,"w+");
        
        
        if (file==NULL)printf("Qualcosa e' andato storto, riprovare\n");
        else{
            sortdata(lib,2);
            for (int i=1; i<(int)lib->size;i++){
                fprintf(file,"%s\t%s\t%s\t%d\n",lib->books[i].titolo,lib->books[i].autore,lib->books[i].editore,lib->books[i].anno);
            }
            fclose(file);
            if (!m) printf("Salvataggio di %d libri in %s completato\n",(int)lib->size-1,path);
        }
        
    }
    else printf("La libreria e' vuota. Sarebbe inutile salvarla\n");
    if (!m) waitforit();
}

void fetchdata(library *lib,int m){
    char path[80]="";
    strcat(path,"library.dat");

    FILE *file = fopen(path,"r");
    
    if (file==NULL)printf("Qualcosa e' andato storto, riprovare\n");
    else{
        char temp[80];
        int count=0;
        while(fgets (temp, 80, file)!= NULL){
            lib->books = (book*)realloc(lib->books,(++lib->size)* sizeof(book));
            lineanalysis(lib,temp);
            ++count;
        } 
        
        if (!m){
            printf("Caricamento di %d libri in %s completato\n",count,path);
            waitforit();
        }
        fclose(file);
    }
    
}

void sort_ricorsivo(wrap *list,library *lib, int ct,int a,int b){

    for (int i=1;i<(int)lib->size;i++){
            list[i].index = getnumber(lib,i,ct);
            list[i].boo = lib->books[i];
            
    } 
    
    mergesort(list,a,b);
    for (int c=1; c<(int)lib->size;c++) lib->books[c] = list[c].boo;
    
    int buf = a;
    int check = a;
    for (int c=a+1;c<=b;c++){
        if (lib->books[c].autore[ct]!=lib->books[c-1].autore[ct]){
            sort_ricorsivo(list,lib,ct+1,buf,c-1);
            buf=c;
        }
        else ++check;
    }
    if (check==b && (b-a)!=0 ) sort_ricorsivo(list,lib,ct+1,a,b);
}

void sortdata(library *lib,int p){
    if (p==1) {
        fetchdata(lib,1);
        savedata(lib,2);
        return;
    }
    if (lib->size==1) {system("clear");return;}
    wrap list[lib->size];

    int carat=0;
    int sx = 1;
    int dx =(int)lib->size-1;
    sort_ricorsivo(list,lib,carat,sx,dx);
    if (!p) {printf("Ordinato con successo\n");waitforit();}

}




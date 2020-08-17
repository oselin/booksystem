#include <stdio.h>
#include <string.h>
#include "booksystem.h"
#include <stdlib.h>
#include <signal.h>



static int handler = 0;
static int endless = 1;
static library lib;

void quit(){
    if (handler==1){
        printf("\nATTENZIONE, LE MODIFICHE NON SONO STATE SALVATE.\nSei sicuro di voler procedere? [y/n] ");
        int c = getchar();
        if (c=='y'){
            endless=0;
            printf("\nTerminato. Arrivederci\n");
            free(lib.books);
            exit(1);
        }
        else if (c=='n'){
            return;}
        else printf("Comando non riconosciuto, uscita anullata\n");
    }
    else {printf("\nTerminato. Arrivederci\n");
    free(lib.books);exit(1);}

}

void help(){
    printf("-h | --help\t\t informazioni per comandi in linea\n");
    printf("-a | --add\t\t aggiungi un nuovo libro alla libreria locale.\t\t --add [title] [author] [editor] [year]\n");
    printf("-r | --remove\t\t rimuovi un libro dalla libreria locale.\t\t --remove [title] [author] [editor] [year]\n");
    printf("-f | --find\t\t cerca tra i titoli disponibili nella libreria.\t\t --find [author]\n");
    printf("-o | --order\t\t ordina la libreria per autore.\t\t\t\t --order\n");
}


void instruction(){
    printf("\nPuoi utilizzare uno dei seguenti comandi:\n");
    printf("a | Aggiungi un nuovo libro alla libreria\n");
    printf("r | Rimuovi un libro dalla libreria\n");
    printf("f | Cerca se un libro e' disponibile\n");
    printf("s | Salva le modifiche\n");
    printf("l | Carica una libreria gia' esistente\n");
    printf("o | Ordina la libreria per autore\n");
    printf("e | Esci\n");
}


void mainloop(){
    system("clear");
    char input[80];
    lib.size=1;
    lib.books = (book*)malloc(sizeof(book));
    
    do{
        instruction();
        scanf("%80s",input);        
        switch (input[0]){
            case 'a':
                system("clear");
                lib.books = (book*)realloc(lib.books,(++lib.size)* sizeof(book));
                handler = 1;
                addbook(&lib,0);
                break;
            case 'r' :
                system("clear");
                handler = 1;
                removebook(&lib,0);
                break;
            case 'f':
                system("clear");
                findbook(&lib,0);
                break;
            case 's':
                system("clear");
                savedata(&lib,0);
                handler = 0;
                break;
            case 'l':
                system("clear");
                handler = 1;
                fetchdata(&lib,0);
                break;
            case 'o':
                handler = 1;
                sortdata(&lib,0);
                break;
            case 'p':
                for (int i=1; i<(int)lib.size; i++) printf("%d|\t%s\t%s\t%s\t%d\n",
                                i,
                                lib.books[i].titolo,
                                lib.books[i].autore,
                                lib.books[i].editore,
                                lib.books[i].anno);
                getchar();
                break;
            case 'e':
                getchar();
                quit();
                break;
            default:
                system("clear");
                printf("Input non riconosciuto, riprovare.");
                break;
            }
                
    }while(endless);
    free(lib.books);
    exit(1);
}


int check(int a, int b){
    if ((a-2)==b) return 1;
    else return 0;
}


int main(int argc,char *argv[]){
    signal(SIGINT,quit);
    if (argc==1){
        printf("Benvenuto nel sistema bibliotecario.\n");
        mainloop();
    }
    
    else{
        
        lib.size=1;
        lib.books = (book*)malloc(sizeof(book));
        if      (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0)    
        {
            help();//NO switch qua perché non si poteva fare l'enum
        }
        
        else if (strcmp(argv[1],"-a") == 0 || strcmp(argv[1],"--add") == 0)
        {
            if (check(argc,4)){addbook(&lib,3,argv[2],argv[3],argv[4],argv[5]);}
            else printf("Parametri insufficienti\n");
        }    
        
        else if (strcmp(argv[1],"-r") == 0 || strcmp(argv[1],"--remove") == 0)
        {
            if (check(argc,4)) {removebook(&lib,3,argv[2],argv[3],argv[4],argv[5]);}
            else printf("Parametri insufficienti\n");
        }  
        
        else if (strcmp(argv[1],"-f") == 0 || strcmp(argv[1],"--find") == 0)
        {
            if (check(argc,1)) {findbook(&lib,1,argv[2]);}
            else printf("Parametri errati\n");
        }
                
        
        else if (strcmp(argv[1],"-o") == 0 || strcmp(argv[1],"--order") == 0)
        {
            sortdata(&lib,1);
        }
        
        else printf("Comando non riconosciuto. Prova main --help per vedere una lista di comandi.\n");
        free(lib.books);
    }
    
}


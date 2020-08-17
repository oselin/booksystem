struct Book{
    int anno;
    char titolo[80];
    char autore[80];
    char editore[80];
};

typedef struct Book book;

struct Library{
    long unsigned size;
    book *books;
};

typedef struct Library library;

struct Wrap {
    int index;
    book boo;
};

typedef struct Wrap wrap;

extern void addbook(library *,int,...);

extern void removebook(library *,int,...);

extern void findbook(library *,int,...);

extern void savedata(library *,int);

extern void fetchdata(library *,int);

extern void sortdata(library *,int);

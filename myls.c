#include<stdio.h>
#include<sys/types.h>
#include<grp.h>
#include<pwd.h>
#include<sys/stat.h>
#include<grp.h>
#include<dirent.h>
#include<string.h>
#include <unistd.h>
#include<stdlib.h>
#include<time.h>
struct calosc
{
    unsigned int permissions;
    long ilosclinkow;
    char UserName[256];
    char GroupName[256];
    unsigned int fileSize;
    time_t fileTime;
    char FileName[256];
    char LinkName[256];
} ;
void my_free(void * x)
{
    x = NULL;
    free(x);
}
//Funkcja wypisujaca pierwszy znak czyli typ pliku dla trybu 1 (bez parametru)
void pierwszyznak(int a)
{
    if((a & S_IFMT)==S_IFLNK)
    {
        printf("l");
        return;
    }
    if((a & S_IFMT)==S_IFDIR)
    {
        printf("d");
        return;
    }
    printf("-");
    return;
}
//Funkcja wypisujaca pierwszy znak czyli typ pliku dla trybu 2 (z parametrem)
void typpliku(int a)
{
    if((a & S_IFMT)==S_IFLNK)
    {
        printf("link symboliczny");
        return;
    }
    if((a & S_IFMT)==S_IFDIR)
    {
        printf("katalog");
        return;
    }
    printf("zwykly plik");
    return;
}
//Funkcja wypisujaca uprawnienia dla danego pliku w trybie 1 (bez parametru)
void uprawnienia(int a)
{
    printf((a & S_IRUSR) ? "r" : "-" );
    printf((a & S_IWUSR) ? "w" : "-" );
    printf((a & S_IXUSR) ? "x" : "-" );
    printf((a & S_IRGRP) ? "r" : "-" );
    printf((a & S_IWGRP) ? "w" : "-" );
    printf((a & S_IXGRP) ? "x" : "-" );
    printf((a & S_IROTH) ? "r" : "-" );
    printf((a & S_IWOTH) ? "w" : "-" );
    printf((a & S_IXOTH) ? "x" : "-" );
    printf(" ");
    return;
}
int checkSizeInt(int a)
{
    int ile=0;
    while(a!=0)
    {
        ile++;
        a=a/10;
    }
    return ile;
}
int checkSizeChar(char *napis)
{
    int i=0;
    for(;;)
    {
        if(napis[i]!='\0')
        {
            i++;
        }
        else
        {
            break;
        }
    }
    return i;
}
int main(int argc, char **argv)
{
    /* zmienne do szerokosci kolumn*/
    int maxLinkLength=1;
    int ilepodfolderow=0;
    int maxUserNameLength=1;
    int maxGroupNameLength=1;
    int maxSizeLength=1;
    int aktualnyLinkLength=1;
    int aktualnyUserNameLength=1;
    int aktualnyGroupNameLength=1;
    int aktualnySizeLength=1;
    /* wszystkie staty, grupy, diry i file*/
    struct stat statbuf;
    struct passwd *pwd;
    struct group *grupa;
    struct dirent* direntp;
    DIR* dirp;
    FILE *plikdoczytania;
    char buffer[256];
    char nazwadir[256];
    char znak;
    char dataa[10];
    char* path;
    int ileplikow=0;
    int i;
    ssize_t link_string_length;
    //TRYB 1
    if(argc==1)
    {
        dirp = opendir("./");
        if(dirp==NULL)
        {
            perror("Wystapil blad w otwieraniu folderu!");
            return 0;
        }
        //Sprawdzam dlugosci wybranych dlugosci linijek, tak abym pozniej mogl sobie wkleic je do kolumn
        for(;;)
        {
            direntp=readdir(dirp);
            if(direntp==NULL) break;
            ileplikow++;
            lstat(direntp->d_name,&statbuf);
            pwd = getpwuid(statbuf.st_uid);
            grupa = getgrgid(statbuf.st_gid);

            aktualnyLinkLength=checkSizeInt(statbuf.st_nlink);
            if(aktualnyLinkLength>maxLinkLength) maxLinkLength=aktualnyLinkLength;

            aktualnyUserNameLength=checkSizeChar(pwd->pw_name);
            if(aktualnyUserNameLength>maxUserNameLength) maxUserNameLength=aktualnyUserNameLength;

            aktualnyGroupNameLength=checkSizeChar(grupa->gr_name);
            if(aktualnyGroupNameLength>maxGroupNameLength) maxGroupNameLength=aktualnyGroupNameLength;

            aktualnySizeLength=checkSizeInt(statbuf.st_size);
            if(aktualnySizeLength>maxSizeLength) maxSizeLength=aktualnySizeLength;
        }
        /* przeskakuje znow na "poczatek" folderu i dla x plikow uzupelniam je danymi*/
        closedir(dirp);
        dirp = opendir("./");
        struct calosc *cale = malloc(sizeof(struct calosc)*ileplikow);
        for(i=0; i<ileplikow; i++)
        {
            direntp=readdir(dirp);
            if(direntp==NULL) break;
            lstat(direntp->d_name,&statbuf);
            pwd = getpwuid(statbuf.st_uid);
            grupa = getgrgid(statbuf.st_gid);
            cale[i].permissions=statbuf.st_mode;
            strcpy(cale[i].UserName,pwd->pw_name);
            cale[i].ilosclinkow=statbuf.st_nlink;
            strcpy(cale[i].GroupName,grupa->gr_name);
            cale[i].fileSize=statbuf.st_size;
            cale[i].fileTime=statbuf.st_mtime;
            strcpy(cale[i].FileName,direntp->d_name);
            strcpy(cale[i].LinkName,"");
            if(S_ISLNK(statbuf.st_mode))
            {
                if ((link_string_length = readlink(direntp-> d_name, buffer, sizeof buffer)) != -1)
                {
                    buffer[link_string_length] = '\0';
                    strcpy(cale[i].LinkName,buffer);
                }
            }
        }
        /* segreguje je alfabetycznie */
        struct calosc chwila;
        for(i=0; i<ileplikow; i++)
        {
            for(int j=0; j<ileplikow; j++)
            {
                if(strcmp(cale[i].FileName,cale[j].FileName)<0)
                {
                    chwila=cale[i];
                    cale[i]=cale[j];
                    cale[j]=chwila;
                }
            }
        }
        /* printuje je */
        for(i=0; i<ileplikow; i++)
        {
            pierwszyznak(cale[i].permissions);
            uprawnienia(cale[i].permissions);
            printf("%*ld", (maxLinkLength+1),cale[i].ilosclinkow);
            printf("%*s",(maxUserNameLength+1),cale[i].UserName);
            printf("%*s ",(maxGroupNameLength+2),cale[i].GroupName);
            printf("%*d ",(maxSizeLength+1), cale[i].fileSize);
            strftime(dataa, 20, "%b %d %H:%M", localtime(&(cale[i].fileTime)));
            printf("%s ",dataa);
            printf("%s", cale[i].FileName);
            if(S_ISLNK(cale[i].permissions))
            {
                printf(" -> %s",cale[i].LinkName);
            }
            printf("\n");
        }
        /* zamykam i zwalniam uzywane rzeczy*/
        closedir(dirp);
        my_free(pwd);
        my_free(grupa);
        free(cale);
    }
    /* TRYB DZIAŁANIA 2. Z JEDNYM ATRYBUTEM */
    if(argc==2)
    {
        if((lstat(argv[1],&statbuf))==-1)
        {
            perror("Blad obslugi pliku!");
            return -1;
        }
        path=realpath(argv[1], NULL);
        pwd = getpwuid(statbuf.st_uid);
        grupa = getgrgid(statbuf.st_gid);
        printf("\nInformacje o %s:\n",argv[1]);
        printf("Typ pliku:      ");
        typpliku(statbuf.st_mode);
        printf("\n");
        /* SPRAWDZA ILOSC PODKATALOGOW*/
        if((statbuf.st_mode & S_IFMT)==S_IFDIR)
        {
            dirp = opendir(argv[1]);
            if(dirp==NULL)
            {
                perror("Wystapil blad w otwieraniu folderu!");
                return 0;
            }
            for(;;)
            {
                direntp=readdir(dirp);
                if(direntp==NULL) break;
                lstat(direntp->d_name,&statbuf);
                if((statbuf.st_mode & S_IFMT)==S_IFDIR && (strcmp(direntp->d_name,".")!=0) && (strcmp(direntp->d_name,"..")!=0)) ilepodfolderow++;
            }
            closedir(dirp);
            printf("Liczba podkatalogow: %d\n", ilepodfolderow);
        }
        /* SPRAWDZAM LINK I DOKAD WSKAZUJE*/
        if((statbuf.st_mode & S_IFMT)==S_IFLNK)
        {

            dirp = opendir("./");
            if(dirp==NULL)
            {
                perror("Wystapil blad w otwieraniu folderu!");
                return 0;
            }
            getcwd(nazwadir, sizeof(nazwadir));
            closedir(dirp);
            printf("Sciezka:\t%s/%s\n",nazwadir,argv[1]);
            printf("Wskauje na:\t%s\n", path);
            free(path);
        }
        else
        {
            printf("Sciezka: \t%s\n",path);
            free(path);
        }
        printf("Rozmiar:\t%ld bajtow\n",statbuf.st_size);
        printf("Uprawnienia: uzytkownik: ");
        printf((statbuf.st_mode & S_IRUSR) ? "r" : "" );
        printf((statbuf.st_mode & S_IWUSR) ? "w" : "" );
        printf((statbuf.st_mode & S_IXUSR) ? "x" : "" );
        printf(", grupa: ");
        printf((statbuf.st_mode & S_IRGRP) ? "r" : "" );
        printf((statbuf.st_mode & S_IWGRP) ? "w" : "" );
        printf((statbuf.st_mode & S_IXGRP) ? "x" : "" );
        printf(", inni: ");
        printf((statbuf.st_mode & S_IROTH) ? "r" : "" );
        printf((statbuf.st_mode & S_IWOTH) ? "w" : "" );
        printf((statbuf.st_mode & S_IXOTH) ? "x\n" : "\n" );
        strftime(dataa, 40, "%d %B %Y roku o %H:%M:%S", localtime(&(statbuf.st_atime)));
        printf("Ostatnio uzywany:        %s\n",dataa);
        strftime(dataa, 40, "%d %B %Y roku o %H:%M:%S", localtime(&(statbuf.st_mtime)));
        printf("Ostatnio modyfikowany:   %s\n",dataa);
        strftime(dataa, 40, "%d %B %Y roku o %H:%M:%S", localtime(&(statbuf.st_ctime)));
        printf("Ostatnio zmieniany stan: %s\n",dataa);
        /* JEZELI NIE MA UPRAWNIEN WYKONYWANIA, TO CZYTAJ PLIK*/
        if(!(statbuf.st_mode & S_IXUSR) && !(statbuf.st_mode & S_IXGRP) && !(statbuf.st_mode & S_IXOTH))
        {
            plikdoczytania=fopen(argv[1],"r");
            if (plikdoczytania == NULL)
            {
                perror("Blad otwierania pliku");
                return -1;
            }
            printf("Pierwsze 77 znakow:\n");
            znak = fgetc(plikdoczytania);
            for(i=0; i<77; i++)
            {
                if(znak!=EOF)
                {
                    printf ("%c", znak);
                    znak = fgetc(plikdoczytania);
                }
            }
            printf("\n");
            fclose(plikdoczytania);
        }
        my_free(pwd);
        my_free(grupa);
    }
    if(argc>2)
    {
        printf("Podano wiecej niz 2 atrybuty! Mozna uzywac tylko 1 albo 0!\n");
    }
    return 0;
}

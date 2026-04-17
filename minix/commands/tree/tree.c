#include <dirent.h>  
#include <stdio.h>    
#include <string.h>   
#include <sys/stat.h> 
#include <sys/ioctl.h>
#include <unistd.h>   
#include <stdlib.h>  


int cmpfunc(const void *a, const void *b) {
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    return strcmp(s1, s2);
}


void print_indent(int level, int is_last) {
    for (int i = 0; i < level; i++) {
        printf("│   ");
    }
    if (level > 0) {
        if (is_last) {
            printf("└── ");
        } else {
            printf("├── ");
        }
    }
}



void tree (const char* path , int level)
{
    char **names_array = NULL ;
    int count = 0;
    DIR* dir = opendir(path);
    struct dirent* entry ;
    struct stat info ;
    char buffer[1024];

    if (!dir)
    {
        perror("Error en la ruta ");
        return;
    }


    entry = readdir(dir);
    while(entry != NULL)
    {
        //ignorar . y ..
        if(strcmp(entry->d_name,".")== 0 || strcmp(entry->d_name,"..")== 0)
        {
            entry =readdir(dir);
            continue;
        }

        //si un archivo valido guardarlo en el espacio de punteros 
        names_array = realloc(names_array, sizeof(char*)*(count+1));
        names_array[count]= strdup(entry->d_name);
        
        count ++;

        entry = readdir(dir);
    }
    closedir(dir);

    //ordenar alfabeticamnte 
    qsort(names_array, count, sizeof(char*), cmpfunc);

    for (int i = 0; i < count; i++)
    {
        //concatenar la  con etilo %s/%s y guardarla en buffer
        snprintf(buffer, sizeof(buffer), "%s/%s", path, names_array[i]);

        //verifica q sea un direccion valida 
        if (lstat(buffer, &info) == -1) {
            perror("lstat");
            continue;
        }

        print_indent(level, i == count-1);
        printf("%s\n", names_array[i]);

         // Si es directorio realizar la recursion
        if (S_ISDIR(info.st_mode) ) {
            tree(buffer, level+1);
        }

        free(names_array[i]);
    }
    free(names_array);
}



int main(int argc, char const *argv[])
{
    const char*  path ;
    if(argc <2)
    {
        //no pasaron un path
        path = ".";
    }
    else{
        path = argv[1]; //pasar el path
    }

    tree(path,1);

    return 0;
}



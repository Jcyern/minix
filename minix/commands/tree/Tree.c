#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

# define PATH_MAX 4096



void tree (const char* path , int level)
{
    DIR *dir = opendir(path);
    char buffer[PATH_MAX];
    struct stat info ;

    if(dir == NULL)
    {
        perror("We cannot open the directory");
    }
    else{

        struct dirent* entry ; //entrada q posee nombre del archivo entre otras 

        entry =readdir(dir);
        while (entry != NULL)
        {
            //no imprimir . ni .. q posee todos rutas que poseen todos los directorios
            if(strcmp(".",entry->d_name)== 0 || strcmp("..",entry->d_name)== 0)
            {
                entry = readdir(dir);
                continue;
            }

            for (int i = 0; i < level; i++)
            {
                printf(" ");
            }


            printf("%s\n",entry->d_name);

            
            snprintf(buffer,sizeof(buffer),"%s/%s",path,entry->d_name);

            //Analizar si es una posible ruta 
            if(lstat(buffer,&info)== 0 && S_ISDIR(info.st_mode))
            {
                tree(buffer,level+1);
            }

            entry = readdir(dir);

        }
        
        
    }
    closedir(dir);


    return ;
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

    tree(path,0);

    return 0;
}

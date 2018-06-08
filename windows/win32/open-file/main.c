#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LINE_MAX 1024

bool get_file_path_A(char *fpath)
{
    OPENFILENAMEA ofn;
    memset(&ofn, 0, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "Config File(*.ini)\0*.ini\0All File(*.*)\0*.*\0\0";  
    ofn.lpstrFile = fpath;  
    ofn.nFilterIndex = 1;  
    ofn.Flags =  OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ;  
    ofn.hInstance = (HMODULE)GetCurrentProcess();

    return GetOpenFileNameA(&ofn);
}

bool get_file_path(wchar_t *fpath)
{
    OPENFILENAMEW ofn;
    memset(&ofn, 0, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"Config File(*.ini)\0*.ini\0All File(*.*)\0*.*\0\0";  
    ofn.lpstrFile = fpath;  
    ofn.nFilterIndex = 1;  
    ofn.Flags =  OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ;  
    ofn.hInstance = (HMODULE)GetCurrentProcess();

    return GetOpenFileNameW(&ofn);
}

void wchar_to_char(const wchar_t *src, char *dst)
{
    int len = WideCharToMultiByte(CP_ACP , 0, src, wcslen(src), NULL, 0, NULL, NULL); 
    WideCharToMultiByte(CP_ACP, 0, src, wcslen(src), dst, len, NULL, NULL); 
    dst[len] = '\0';

    return;
}

char *get_ini_key_string(char *filename, char *title, char *key)   
{   
    FILE *fp;   
    char line[LINE_MAX];
    static char tmpstr[LINE_MAX];

    int rtnval;
    int i = 0;
    bool find = false;

    char *tmp;

    fp = fopen(filename, "r");
    assert(fp);

    while(!feof(fp)) {   
        rtnval = fgetc(fp);   
        if(rtnval == EOF)
            break; 
        else
            line[i++] = rtnval;

        if(rtnval == '\n') {   
#ifndef WIN32  
            i--;  
#endif    
            line[--i] = '\0';
            i = 0;
            tmp = strchr(line, '=');   
  
            if(( tmp != NULL ) && (find == true) ) {   
                if(strstr(line, key) != NULL) {
                    if ('#' == line[0])
                        ;//do nothing
                    else {
                        strcpy(tmpstr, tmp+1);
                        fclose(fp);
                        return tmpstr;
                    }
                }
            } else {
                strcpy(tmpstr, "[");   
                strcat(tmpstr, title);   
                strcat(tmpstr, "]");  
                if(strncmp(tmpstr, line, strlen(tmpstr)) == 0)
                    find = true;
            }  
        }
    }  
    fclose(fp);   
    return "";   
}  
  
int get_ini_key_int(char *filename, char *title, char *key)  
{
    return atoi(get_ini_key_string(filename, title, key));
}

int main(void)
{
    wchar_t fpath[MAX_PATH] = {0};
    char str[MAX_PATH] = {0};

    bool ret = get_file_path(fpath);
    if(ret == false)
        return 0;

    wchar_to_char(fpath, str);

    get_ini_key_int(fpath, "Resolution", "ImageWidth"));
    get_ini_key_int(fpath, "Resolution", "ImageHeight"));

    return 0;
} 
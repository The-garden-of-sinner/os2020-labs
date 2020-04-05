int strlen(char *str)
{
    int n=0;
    while(*str++) n++;
    return n;
}

int strcpy(char *src, char *dst){
    int n=0;
    while(*src) {
        *dst++ = *src++;
        n++;
    }
    return n;
}

int strcmp(char *str1, char *str2){
    char c1, c2;
    do {
        c1 = *str1++;
        c2 = *str2++;
        if (c1 != c2) return (c1 > c2)?1:-1;
        if (c1 == '\0') break;
    } while(1);
    return 0;
}

int strncmp( char *str1,  char *str2,  int n){
     char c1, c2;    

    while(n){
        c1 = *str1++;
        c2 = *str2++;        
        if (c1 != c2) return (c1 > c2)?1:-1;
        if (c1 == '\0') break;
    };
    return 0;
}

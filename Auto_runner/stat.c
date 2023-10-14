#include <stdio.h>
#include <string.h>
#define log ".log"
char buf[5 << 20];
const char p[][20] = {"FINISHED", "Ôç²Ù", "¿ÎÍâ»î¶¯", "Ò¹ÅÜ", "ERROR"};
int main()
{
    FILE *fp = fopen(log, "r");
    fread(buf, 1, sizeof(buf), fp);
    // for (int i = 0; i < 10; ++i) putchar(buf[i]);
    for (int i = 0; i < sizeof(p) / sizeof(*p); ++i)
    {
        int cnt = 0;
        const char *t = buf;
        while (t = strstr(t, p[i]))
            ++cnt, t++;
        printf("[%s]: \t%d\n", p[i], cnt);
    }
    fclose(fp);
    puts("Press any key to continue.");
    getchar();
}

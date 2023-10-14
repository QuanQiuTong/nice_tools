#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define log ".log"
const char p[][20] = {"FINISHED", "早操", "课外活动", "夜跑", "ERROR"};
int main()
{
    FILE *fp = fopen(log, "r");
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buf = malloc(len);
    fread(buf, 1, len, fp);
    for (int i = 0; i < sizeof(p) / sizeof(*p); ++i)
    {
        int cnt = 0;
        for (const char *t = buf; t = strstr(t, p[i]); ++t)
            ++cnt;
        printf("[%s]: \t%d\n", p[i], cnt);
    }
    fclose(fp);
    puts("Press any key to continue.");
    getchar();
}

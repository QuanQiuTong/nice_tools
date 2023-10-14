#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define log ".log"

void exec(struct tm *tt)
{
    static _Bool run[3];
    static int lasday;

    if (tt->tm_wday != lasday)
        lasday = tt->tm_wday, run[0] = run[1] = run[2] = 0;

    switch (tt->tm_hour * 100 + tt->tm_min)
    {
    case 701:
    case 702:
        if (!run[0])
        {
            run[0] = 1;
            system(".\\main.exe -r 28 -d 888 >> " log);
            FILE *fp = fopen(log, "a");
            fputs(asctime(tt), fp);
            fputs("------------", fp);
            fclose(fp);
        }
        break;

    case 1604:
    case 1605:
        if (!run[1])
        {
            run[1] = 1;
            system(".\\main.exe -r 33 -d 1340 >> " log);
            FILE *fp = fopen(log, "a");
            fputs(asctime(tt), fp);
            fputs("------------", fp);
            fclose(fp);
        }
        break;

    case 1909:
    case 1910:
        if (!run[2])
        {
            run[2] = 1;
            system(".\\main.exe -r 38 -d 1250 >> " log);
            FILE *fp = fopen(log, "a");
            fputs(asctime(tt), fp);
            fputs("------------", fp);
            fclose(fp);
        }
        break;

    default:
        puts(asctime(tt));
        break;
    }
}
int main()
{
    for (time_t t;; _sleep(1000 * 60))
        exec(localtime((t = time(NULL), &t)));
}
#if 0
int main()
{
    for (int lasday = -1;; _sleep(1000))
    {
        time_t t = time(NULL);
        struct tm *tt = localtime(&t);

        if (tt->tm_sec != lasday)
        {
            FILE *fp = stdout; // fopen(".log", "a");
            fputs(asctime(tt), fp);
            /* if (tt->tm_hour == 21 && tt->tm_min == 30)
            {
                FILE *p = _popen(view, "r");
                fwrite(buf, 1, fread(buf, 1, sizeof(buf), p), fp);
                _pclose(p);
            } */
            if (tt->tm_hour == 21 && tt->tm_min == 34)
            {
                system(".\\main.exe -v >> "log);
            }
            lasday = tt->tm_sec;
            // fclose(fp);
        }
    }
}
#endif

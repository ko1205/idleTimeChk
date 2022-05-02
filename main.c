#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <time.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char** argv)
{
    int limitIdle;
    char logPath[256];
    char logName[256];
    XIEventMask mask;
    Display *display;
    Window window;
    timer_t start,end;
    struct tm start_time;
    struct tm end_time;
    FILE *fp;

    if(argc != 4)
    {
        printf("use by ./idleTime [idle min] [log path] [filename]\n");
        return -1;
    }
    DIR *dir = opendir(argv[2]);
    if(dir)
    {

    } else if(ENOENT == errno) {
        printf("directory does not exist.\n");
        return -1;
    } else{
        printf("opendir() failed for some other reason.\n");
        return -1;
    }

    if ((display = XOpenDisplay(NULL)) == NULL)
    {
        printf("can not open Display\n");
        return -1;
    }
    window = DefaultRootWindow(display);

    mask.deviceid = XIAllDevices;
    mask.mask_len = XIMaskLen(XI_LASTEVENT);
    mask.mask = calloc(mask.mask_len, sizeof(char));
    XISetMask(mask.mask, XI_ButtonPress);
    XISetMask(mask.mask, XI_ButtonRelease);
    XISetMask(mask.mask, XI_KeyPress);
    XISetMask(mask.mask, XI_KeyRelease);
    XISetMask(mask.mask, XI_Motion);

    XISelectEvents(display, window, &mask, 1);
    XMapWindow(display, window);

    start = time(NULL);
    localtime_r(&start, &start_time);
//    printf("%d\n", 1900+start_time.tm_year);
    sprintf(logPath,"%s/%04d_%02d_%02d_%s",argv[2], 1900+start_time.tm_year,
    start_time.tm_mon+1, start_time.tm_mday, argv[3]);
    fp = fopen(logPath,"a");
    fprintf(fp, "Service Start Time [%04d/%02d/%02d] %02d:%02d:%02d\n",
    1900 + start_time.tm_year, start_time.tm_mon + 1, start_time.tm_mday, start_time.tm_hour, start_time.tm_min, start_time.tm_sec);
    fclose(fp);
    while(True)
    {
        XEvent event;
        XGenericEventCookie *cookie = (XGenericEventCookie*)&event.xcookie;
        XNextEvent(display, (XEvent*)&event);
        end = time(NULL);
        int idleTime = end-start;
        if(idleTime > atoi(argv[1])*60)
        {
            localtime_r(&start, &start_time);
            localtime_r(&end, &end_time);
            sprintf(logPath,"%s/%04d_%02d_%02d_%s",argv[2],1900+end_time.tm_year,
            end_time.tm_mon+1, end_time.tm_mday, argv[3]);
            fp = fopen(logPath,"a");
            if(fp!=NULL)
            {
                fprintf(fp, "idle Time [%04d/%02d/%02d] %02d:%02d:%02d - [%04d/%02d/%02d] %02d:%02d:%02d (%.3f min)\n",
                    1900 + start_time.tm_year, start_time.tm_mon + 1, start_time.tm_mday, start_time.tm_hour, start_time.tm_min, start_time.tm_sec,
                    1900 + end_time.tm_year, end_time.tm_mon + 1, end_time.tm_mday, end_time.tm_hour, end_time.tm_min, end_time.tm_sec, (double)idleTime/60 );
                fclose(fp);
            }
        }
        start = end;
        XFreeEventData(display, cookie);
    }
    return 0;
}

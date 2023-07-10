#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libnotify/notify.h>
#include <canberra.h>

void printBanner(){
    printf("██████╗  ██████╗ ███╗   ███╗ ██████╗ ██████╗  ██████╗ ██████╗  ██████\n██╔══██╗██╔═══██╗████╗ ████║██╔═══██╗██╔══██╗██╔═══██╗██╔══██╗██╔═══██╗\n██████╔╝██║   ██║██╔████╔██║██║   ██║██║  ██║██║   ██║██████╔╝██║   ██║\n██╔═══╝ ██║   ██║██║╚██╔╝██║██║   ██║██║  ██║██║   ██║██╔══██╗██║   ██║\n██║     ╚██████╔╝██║ ╚═╝ ██║╚██████╔╝██████╔╝╚██████╔╝██║  ██║╚██████╔╝\n╚═╝      ╚═════╝ ╚═╝     ╚═╝ ╚═════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝ ╚═════╝   \n");
}

void displayProgressBar(int progress, int total, int type, int rep, int max) {

    fflush(stdin);
    int barWidth = 50;
    int filledWidth = (int)((double)progress / total * barWidth);

    printf("\r[");
    if(type == 0)
        printf("\033[0;31m"); // red
    else if(type == 1)
        printf("\033[0;32m"); // green
    else
        printf("\033[0;34m"); // blue
    for (int i = 0; i < barWidth; ++i) {
        if (i < filledWidth) {
            printf("=");
        } else {
            printf(" ");
        }
    }
    printf("\033[0m");
    printf("] ");

    if(progress == total){
        ca_context * hello;
	ca_context_create (&hello);
	ca_context_play (hello, 0,
		CA_PROP_EVENT_ID, "bell",
		CA_PROP_EVENT_DESCRIPTION, "hello world",
		NULL);
        if(type == 0){
            printf(" START BREAK ");
                NotifyNotification *notification = notify_notification_new(
        "Pomodoro",          // Notification title
        "Start break!",  // Notification body
        NULL              // Icon file path (NULL to use default)
    );
                    if (!notify_notification_show(notification, NULL)) {
        fprintf(stderr, "Failed to show notification\n");
    }
        g_object_unref(notification);

        }
        else {
            printf("  START WORK ");
            NotifyNotification *notification = notify_notification_new(
        "Pomodoro",          // Notification title
        "Start work!",  // Notification body
        NULL              // Icon file path (NULL to use default)
    );
                    if (!notify_notification_show(notification, NULL)) {
        fprintf(stderr, "Failed to show notification\n");
    }
        g_object_unref(notification);
        }
        printf("%d/%d", rep, max);
        fflush(stdout);
        getchar();
        system("clear");
        printBanner();

        return;
    }

    if (type == 0){
            printf("WORK: ");
            int left = total - progress;
            int left_min = left / 60;
            int left_s = left - left_min*60 ;
            printf("%02d:%02d ", left_min, left_s);
            printf("%d/%d", rep, max);
    }
    else if(type == 1){
            printf("BREAK: ");
            int left = total - progress;
            int left_min = left / 60;
            int left_s = left - left_min*60 ;
            printf("%02d:%02d ", left_min, left_s);
            printf("%d/%d", rep, max);
    }
    else{
            printf("LONG BREAK: ");
            int left = total - progress;
            int left_min = left / 60;
            int left_s = left - left_min*60 ;
            printf("%02d:%02d ", left_min, left_s);
    }

    fflush(stdout);
}

int main(int argc, char** argv){
    system("clear");
    if (!notify_init("Notification Example")) {
        fprintf(stderr, "Failed to initialize libnotify\n");
        return 1;
    }
    printBanner();

    int laptime = argc > 1 ? atoi(argv[1])*60 : 30*60;
    int breaktime = argc > 2 ? atoi(argv[2])*60 : 5*60;
    int lap_num = argc > 3 ? atoi(argv[3]) : 4;
    int long_break = argc > 4 ? atoi(argv[4]) * 60 : 40* 60;

    while(1){
        int elapsed_time = 0;
                            int progress = 0;
            for(int curr_rep = 0; curr_rep < lap_num; curr_rep++){
                elapsed_time = 0;
                    while (elapsed_time <= laptime) {
                        displayProgressBar(elapsed_time, laptime, 0, curr_rep+1, lap_num);
                        sleep(1);  // Wait for 1 second
                        elapsed_time++;
                        progress = (int)((double)elapsed_time / laptime* 100);
                    }
                    elapsed_time = 0;
                    if(curr_rep != lap_num-1)
                    while (elapsed_time <= breaktime) {
                        displayProgressBar(elapsed_time, laptime, 1, curr_rep+1, lap_num);
                        sleep(1);  // Wait for 1 second
                        elapsed_time++;
                        progress = (int)((double)elapsed_time / laptime* 100);
                    }
            }
            elapsed_time = 0;
            while (elapsed_time <= long_break){
                        displayProgressBar(elapsed_time, laptime, 2, lap_num, lap_num);
                        sleep(1);  // Wait for 1 second
                        elapsed_time++;
                        progress = (int)((double)elapsed_time / laptime* 100);
                    }


    }
    notify_uninit();
    return 0;
}    

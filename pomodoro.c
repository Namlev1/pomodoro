#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <unistd.h>

void clear_stdin()
{
    char c;
    do
    {
        c = getchar();
    } while (c != EOF && c != '\n');
}

void printBanner()
{
    printf("██████╗  ██████╗ ███╗   ███╗ ██████╗ ██████╗  ██████╗ ██████╗  ██████\n██╔══██╗██╔═══██╗████╗ ████║██╔═══██╗██╔══██╗██╔═══██╗██╔══██╗██╔═══██╗\n██████╔╝██║   ██║██╔████╔██║██║   ██║██║  ██║██║   ██║██████╔╝██║   ██║\n██╔═══╝ ██║   ██║██║╚██╔╝██║██║   ██║██║  ██║██║   ██║██╔══██╗██║   ██║\n██║     ╚██████╔╝██║ ╚═╝ ██║╚██████╔╝██████╔╝╚██████╔╝██║  ██║╚██████╔╝\n╚═╝      ╚═════╝ ╚═╝     ╚═╝ ╚═════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝ ╚═════╝   \n");
}

void displayProgressBar(int progress, int total, int type, int rep, int max)
{
    int barWidth = 50;
    int filledWidth = (int)((double)progress / total * barWidth);

    printf("\r[");
    if (type == 0)
        printf("\033[0;31m"); // red
    else if (type == 1)
        printf("\033[0;32m"); // green
    else
        printf("\033[0;34m"); // blue
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < filledWidth)
        {
            printf("=");
        }
        else
        {
            printf(" ");
        }
    }
    printf("\033[0m");
    printf("] ");

    if (progress == total)
    {
        if (type == 0)
        {
            printf(" START BREAK ");
            system("notify-send \"Pomodoro\" \"Start break\"");
            system("paplay break.wav &");
        }
        else
        {
            printf("  START WORK ");
            system("notify-send \"Pomodoro\" \"Start work\"");
            system("paplay work.wav &");
        }
        printf("%d/%d", rep, max);
        fflush(stdout);
        getchar();
        system("clear");
        printBanner();

        return;
    }

    if (type == 0)
    {
        printf("WORK: ");
        int left = total - progress;
        int left_min = left / 60;
        int left_s = left - left_min * 60;
        printf("%02d:%02d ", left_min, left_s);
        printf("%d/%d", rep, max);
    }
    else if (type == 1)
    {
        printf("BREAK: ");
        int left = total - progress;
        int left_min = left / 60;
        int left_s = left - left_min * 60;
        printf("%02d:%02d ", left_min, left_s);
        printf("%d/%d", rep, max);
    }
    else
    {
        printf("LONG BREAK: ");
        int left = total - progress;
        int left_min = left / 60;
        int left_s = left - left_min * 60;
        printf("%02d:%02d ", left_min, left_s);
    }

    fflush(stdout);
}

void pausee()
{
    struct pollfd pfd = {.fd = fileno(stdin), .events = POLLIN};
    if (poll(&pfd, 1, 0) > 0)
        if (pfd.revents & POLLIN)
            clear_stdin();

    printf("\nPAUSED. Press ENTER button to continue\n");
    getchar();
    system("clear");
    printBanner();

    struct pollfd pfd1 = {.fd = fileno(stdin), .events = POLLIN};
    if (poll(&pfd1, 1, 0) > 0)
        if (pfd1.revents & POLLIN)
            clear_stdin();
}

int main(int argc, char **argv)
{
    system("clear");
    printBanner();
    int laptime = argc > 1 ? atoi(argv[1]) * 60 : 8;
    int breaktime = argc > 2 ? atoi(argv[2]) * 60 : 8;
    int long_break = argc > 3 ? atoi(argv[4]) * 60 : 8;
    int lap_num = argc > 4 ? atoi(argv[3]) : 2;

    while (1)
    {
        int elapsed_time = 0;
        for (int curr_rep = 0; curr_rep < lap_num; curr_rep++)
        {
            // WORK
            elapsed_time = 0;
            while (elapsed_time <= laptime)
            {
                displayProgressBar(elapsed_time, laptime, 0, curr_rep + 1, lap_num);
                sleep(1); // Wait for 1 second
                elapsed_time++;
                struct pollfd pfd = {.fd = fileno(stdin), .events = POLLIN};
                if (poll(&pfd, 1, 0) > 0)
                    if (pfd.revents & POLLIN)
                    {
                        char c = getchar();
                        if (c == '\n')
                        {
                            system("clear");
                            printBanner();
                            displayProgressBar(elapsed_time, laptime, 0, curr_rep + 1, lap_num);
                            pausee();
                        }
                    }
            }

            // SHORT BREAK
            elapsed_time = 0;
            if (curr_rep != lap_num - 1)
                while (elapsed_time <= breaktime)
                {
                    displayProgressBar(elapsed_time, laptime, 1, curr_rep + 1, lap_num);
                    sleep(1); // Wait for 1 second
                    elapsed_time++;
                    struct pollfd pfd = {.fd = fileno(stdin), .events = POLLIN};
                    if (poll(&pfd, 1, 0) > 0)
                        if (pfd.revents & POLLIN)
                        {
                            char c = getchar();
                            if (c == '\n')
                            {
                                system("clear");
                                printBanner();
                                displayProgressBar(elapsed_time, laptime, 1, curr_rep + 1, lap_num);
                                pausee();
                            }
                        }
                }
        }

        // LONG BREAK
        elapsed_time = 0;
        while (elapsed_time <= long_break)
        {
            displayProgressBar(elapsed_time, laptime, 2, lap_num, lap_num);
            sleep(1); // Wait for 1 second
            elapsed_time++;
            struct pollfd pfd = {.fd = fileno(stdin), .events = POLLIN};
            if (poll(&pfd, 1, 0) > 0)
                if (pfd.revents & POLLIN)
                {
                    char c = getchar();
                    if (c == '\n')
                    {
                        system("clear");
                        printBanner();
                        displayProgressBar(elapsed_time, laptime, 2, lap_num, lap_num);
                        pausee();
                    }
                }
        }
    }
    return 0;
}

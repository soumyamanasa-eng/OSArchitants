#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

volatile sig_atomic_t sigterm_received = 0;
volatile sig_atomic_t sigint_received = 0;
volatile sig_atomic_t sigchld_received = 0;

pid_t child_pid = -1;

/* Signal handler */
void signal_handler(int sig)
{
    if (sig == SIGTERM)
        sigterm_received = 1;
    else if (sig == SIGINT)
        sigint_received = 1;
    else if (sig == SIGCHLD)
        sigchld_received = 1;
}

/* Play alarm sound */
void play_alarm_sound(void)
{
    for (int i = 0; i < 3; i++)
    {
        pid_t sound_pid = fork();

        if (sound_pid == 0)
        {
            execlp("paplay", "paplay", "alarm.wav", (char *)NULL);
            perror("paplay");
            _exit(1);
        }
        else if (sound_pid > 0)
        {
            waitpid(sound_pid, NULL, 0);
        }
        else
        {
            perror("fork");
        }
    }
}

/* Display alarm notification */
void show_alert(const char *message)
{
    printf("\n");
    printf("=============================================\n");
    printf("          LINUX OS EVENT ALARM\n");
    printf("=============================================\n");
    printf("ALERT: %s\n", message);
    printf("=============================================\n");
}

/* Get Y/N choice from user */
char get_choice(void)
{
    char choice;

    while (1)
    {
        printf("Enter choice [Y/N]: ");
        fflush(stdout);

        if (scanf(" %c", &choice) == 1)
        {
            while (getchar() != '\n')
                ;

            if (choice == 'Y' || choice == 'y' ||
                choice == 'N' || choice == 'n')
            {
                return choice;
            }
        }

        printf("Please enter Y or N.\n");
    }
}

int main(void)
{
    struct sigaction sa;

    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    /* Register Linux signal handlers */
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGCHLD, &sa, NULL);

    printf("=============================================\n");
    printf("       LINUX OS EVENT ALARM SYSTEM\n");
    printf("=============================================\n\n");

    printf("Main Monitor PID : %d\n", getpid());

    /* Create child process */
    child_pid = fork();

    if (child_pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (child_pid == 0)
    {
        printf("Child Process PID : %d\n", getpid());
        printf("Child will finish automatically after 05 seconds.\n");
        printf("\nMonitoring OS events...\n");
        fflush(stdout);

        sleep(5);

        printf("\nChild process is terminating normally.\n");
        _exit(0);
    }

    printf("Child PID         : %d\n", child_pid);
    printf("\n");
    printf("Events being monitored:\n");
    printf("1. SIGTERM - Termination request\n");
    printf("2. SIGINT  - Keyboard interrupt (Ctrl+C)\n");
    printf("3. SIGCHLD - Child process termination\n");
    printf("\n");
    printf("System is ready.\n");
    printf("=============================================\n");

    while (1)
    {
        /* EVENT 1: SIGTERM */
        if (sigterm_received)
        {
            sigterm_received = 0;

            show_alert("Termination request (SIGTERM) received.");

            char choice = get_choice();

            if (choice == 'Y' || choice == 'y')
            {
                printf("\nTermination accepted.\n");
                printf("The alarm system will now terminate.\n");

                if (child_pid > 0)
                {
                    kill(child_pid, SIGTERM);
                    waitpid(child_pid, NULL, 0);
                }

                printf("Process terminated successfully.\n");
                return 0;
            }
            else
            {
                printf("\nTermination rejected.\n");
                printf("The alarm system will continue running.\n");
                printf("Playing alarm sound...\n");

                play_alarm_sound();

                printf("Monitoring continues.\n");
            }
        }

        /* EVENT 2: SIGINT */
        if (sigint_received)
        {
            sigint_received = 0;

            show_alert("Keyboard interrupt (SIGINT / Ctrl+C) received.");

            char choice = get_choice();

            if (choice == 'Y' || choice == 'y')
            {
                printf("\nExit request accepted.\n");
                printf("Shutting down alarm system...\n");

                if (child_pid > 0)
                {
                    kill(child_pid, SIGTERM);
                    waitpid(child_pid, NULL, 0);
                }

                printf("Process terminated successfully.\n");
                return 0;
            }
            else
            {
                printf("\nExit request rejected.\n");
                printf("The alarm system will continue running.\n");
                printf("Playing alarm sound...\n");

                play_alarm_sound();

                printf("Monitoring continues.\n");
            }
        }

        /* EVENT 3: SIGCHLD */
        if (sigchld_received)
        {
            int status;
            pid_t result;

            sigchld_received = 0;

            result = waitpid(child_pid, &status, WNOHANG);

            if (child_pid > 0 && result == child_pid)
            {
                show_alert("Child process has terminated (SIGCHLD received).");

                printf("Child PID %d has ended.\n", child_pid);
                printf("Playing alarm sound...\n");

                play_alarm_sound();

                printf("Parent monitor is still running.\n");

                child_pid = -1;
            }
        }

        sleep(1);
    }

    return 0;
}

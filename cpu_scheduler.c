#include <stdio.h>

#define MAX 100

struct Process
{
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    int completion_time;
    int waiting_time;
    int turnaround_time;
};

void printHeader()
{
    printf("\n==================================================\n");
    printf("           CPU SCHEDULING SIMULATOR\n");
    printf("==================================================\n");
}

void printMenu()
{
    printf("\nSelect Scheduling Algorithm:\n");
    printf("1. First Come First Serve (FCFS)\n");
    printf("2. Shortest Job First (Non-Preemptive)\n");
    printf("3. Priority Scheduling (Non-Preemptive)\n");
    printf("4. Round Robin\n");
    printf("0. Exit\n");
    printf("\nEnter choice: ");
}

void inputProcesses(struct Process p[], int n)
{
    for (int i = 0; i < n; i++)
    {
        p[i].pid = i + 1;

        printf("\nProcess %d\n", p[i].pid);
        printf("Arrival Time  : ");
        scanf("%d", &p[i].arrival_time);

        printf("Burst Time    : ");
        scanf("%d", &p[i].burst_time);

        printf("Priority (lower number = higher priority): ");
        scanf("%d", &p[i].priority);

        p[i].remaining_time = p[i].burst_time;
    }
}

void printProcessTable(struct Process p[], int n)
{
    printf("\n--------------------------------------------\n");
    printf("PID\tAT\tBT\n");
    printf("--------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time);
    }

    printf("--------------------------------------------\n");
}

void sortByArrival(struct Process p[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (p[j].arrival_time > p[j + 1].arrival_time)
            {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

void fcfs(struct Process p[], int n)
{
    sortByArrival(p, n);

    int current_time = 0;
    float total_wt = 0, total_tat = 0;

    printf("\n==================== FCFS Scheduling ====================\n");

    // Gantt Chart Header
    printf("\nGantt Chart:\n| ");

    for (int i = 0; i < n; i++)
    {
        // Handle CPU Idle Time
        if (current_time < p[i].arrival_time)
        {
            printf("Idle | ");
            current_time = p[i].arrival_time;
        }

        printf("P%d | ", p[i].pid);

        p[i].completion_time = current_time + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;

        current_time = p[i].completion_time;

        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }

    // Print timeline
    printf("\n0");
    for (int i = 0; i < n; i++)
    {
        printf("    %d", p[i].completion_time);
    }
    printf("\n");

    // Print Table
    printf("\n-------------------------------------------------------------\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].completion_time,
               p[i].turnaround_time,
               p[i].waiting_time);
    }

    printf("-------------------------------------------------------------\n");
    printf("Average Waiting Time    : %.2f\n", total_wt / n);
    printf("Average Turnaround Time : %.2f\n", total_tat / n);
}

void sjf(struct Process p[], int n)
{
    int completed = 0;
    int current_time = 0;
    float total_wt = 0, total_tat = 0;
    int visited[MAX] = {0};

    while (completed < n)
    {
        int idx = -1;
        int min_bt = 99999;

        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival_time <= current_time &&
                !visited[i] &&
                p[i].burst_time < min_bt)
            {
                min_bt = p[i].burst_time;
                idx = i;
            }
        }

        if (idx == -1)
        {
            current_time++;
        }
        else
        {
            p[idx].completion_time = current_time + p[idx].burst_time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;

            current_time = p[idx].completion_time;

            total_wt += p[idx].waiting_time;
            total_tat += p[idx].turnaround_time;

            visited[idx] = 1;
            completed++;
        }
    }

    printf("\n-------------------------------------------------------------\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].completion_time,
               p[i].turnaround_time,
               p[i].waiting_time);
    }

    printf("-------------------------------------------------------------\n");
    printf("Average Waiting Time    : %.2f\n", total_wt / n);
    printf("Average Turnaround Time : %.2f\n", total_tat / n);
}

void priorityScheduling(struct Process p[], int n)
{
    int completed = 0;
    int current_time = 0;
    float total_wt = 0, total_tat = 0;
    int visited[MAX] = {0};

    while (completed < n)
    {
        int idx = -1;
        int best_priority = 99999;

        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival_time <= current_time &&
                !visited[i] &&
                p[i].priority < best_priority)
            {

                best_priority = p[i].priority;
                idx = i;
            }
        }

        if (idx == -1)
        {
            current_time++;
        }
        else
        {
            p[idx].completion_time = current_time + p[idx].burst_time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;

            current_time = p[idx].completion_time;

            total_wt += p[idx].waiting_time;
            total_tat += p[idx].turnaround_time;

            visited[idx] = 1;
            completed++;
        }
    }

    printf("\n-------------------------------------------------------------\n");
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\n");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].priority,
               p[i].completion_time,
               p[i].turnaround_time,
               p[i].waiting_time);
    }

    printf("-------------------------------------------------------------\n");
    printf("Average Waiting Time    : %.2f\n", total_wt / n);
    printf("Average Turnaround Time : %.2f\n", total_tat / n);
}

void roundRobin(struct Process p[], int n, int tq)
{
    int completed = 0;
    int current_time = 0;
    float total_wt = 0, total_tat = 0;

    int done;

    while (completed < n)
    {
        done = 1;

        for (int i = 0; i < n; i++)
        {

            if (p[i].arrival_time <= current_time &&
                p[i].remaining_time > 0)
            {

                done = 0;

                if (p[i].remaining_time > tq)
                {
                    current_time += tq;
                    p[i].remaining_time -= tq;
                }
                else
                {
                    current_time += p[i].remaining_time;
                    p[i].remaining_time = 0;

                    p[i].completion_time = current_time;
                    p[i].turnaround_time =
                        p[i].completion_time - p[i].arrival_time;
                    p[i].waiting_time =
                        p[i].turnaround_time - p[i].burst_time;

                    total_wt += p[i].waiting_time;
                    total_tat += p[i].turnaround_time;

                    completed++;
                }
            }
        }

        if (done)
            current_time++;
    }

    printf("\n-------------------------------------------------------------\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].completion_time,
               p[i].turnaround_time,
               p[i].waiting_time);
    }

    printf("-------------------------------------------------------------\n");
    printf("Average Waiting Time    : %.2f\n", total_wt / n);
    printf("Average Turnaround Time : %.2f\n", total_tat / n);
}

int main()
{
    int choice;

    do
    {
        printHeader();
        printMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            int n;
            struct Process p[MAX];

            printf("\nEnter number of processes: ");
            scanf("%d", &n);

            inputProcesses(p, n);
            fcfs(p, n);
            break;
        }

        case 2:
        {
            int n;
            struct Process p[MAX];

            printf("\nEnter number of processes: ");
            scanf("%d", &n);

            inputProcesses(p, n);
            sjf(p, n);
            break;
        }

        case 3:
        {
            int n;
            struct Process p[MAX];

            printf("\nEnter number of processes: ");
            scanf("%d", &n);

            inputProcesses(p, n);
            priorityScheduling(p, n);
            break;
        }

        case 4:
        {
            int n, tq;
            struct Process p[MAX];

            printf("\nEnter number of processes: ");
            scanf("%d", &n);

            inputProcesses(p, n);

            printf("\nEnter Time Quantum: ");
            scanf("%d", &tq);

            roundRobin(p, n, tq);
            break;
        }

        case 0:
            printf("\nExiting...\n");
            break;

        default:
            printf("\nInvalid choice!\n");
        }

        printf("\nPress Enter to continue...");
        getchar();
        getchar(); // pause

    } while (choice != 0);

    return 0;
}

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

struct Process
{
    int pid;        // Process ID
    int bt;         // Burst-Time
    int wt;         // Waiting-Time
    int tt;         // Turnaround-Time
    int prio;       // Priority
    int quantum;    // Quantum-Time
    int su;         // User/System
};
int process_count;
int process_system_count = 0;
int process_user_count = 0;

/**
 * @brief First-Come First-Serve Algorithm Implementation
 * 
 * @param processes 
 */
void FCFS(struct Process processes[], int pc) {    

    // Calculate waiting time
    processes[0].wt = 0;
    for (int i = 1; i < pc; i++)
        processes[i].wt = processes[i - 1].bt + processes[i - 1].wt;
    
    // Calculate turnaround time
    for (int i = 0; i < pc; i++)
        processes[i].tt = processes[i].wt + processes[i].bt;
}

/**
 * @brief Shortest-Job-First Algorithm Implementation
 * 
 * @param processes 
 */
void SJF(struct Process processes[], int pc) {

    // Sorting all processos by their burst-time using selection-sort
    for (int i = 0; i < pc; i++) {
        int pos = i;
        for (int j = i + 1; j < pc; j++)
            if (processes[j].bt < processes[pos].bt)
                pos = j;
 
        // Swap
        struct Process temp = processes[i];
        processes[i] = processes[pos];
        processes[pos] = temp;
    }

    // Rest is the same as FCFS
    FCFS(processes, pc);
}

/**
 * @brief Round-Robin Algorithm Implementation
 * 
 * @param processes 
 */
void RR(struct Process processes[], int quantum, int pc) {

    // Calculate waiting times
    int current_time = 0;
    // Make a copy of burst times because we don't want the change them at first
    int temp_bt[pc];
    for (int i = 0; i < pc; i++)
        temp_bt[i] = processes[i].bt;

    while (1) {

        // Done is true by default
        int is_done = 1;

        for (int i = 0; i < pc; i++) {

            // Do we still have work to do?
            if (temp_bt[i] > 0) {
                is_done = 0;
                
                // (PROCESS) If burst-time is greater thatn quantum-time, then add to the current process time and decrease burst-time
                if (temp_bt[i] > quantum) {
                    current_time += quantum;
                    temp_bt[i] -= quantum;
                    printf("[%d] Process is running...\n", processes[i].pid);
                // (WAIT) Otherwise, add to the current time, set waiting time and zero out the burt-time
                } else {
                    current_time += temp_bt[i];
                    processes[i].wt = current_time - processes[i].bt;
                    temp_bt[i] = 0;
                }
            }
        }

        // If all processes are processed, then break out of the loop
        if (is_done == 1)
            break;
    }

    // Calculate turnaround time
    for (int i = 0; i < pc; i++)
        processes[i].tt = processes[i].wt + processes[i].bt;
}

/**
 * @brief Prints the reults of simulation
 */
void print(struct Process processes[], int pc) {

    int sum_tt = 0, sum_wt = 0;

    printf("\nP_ID\tBurst\tWaiting\tTurnaround\n");
    for (int i = 0; i < pc; i++) {
        printf("%d\t%d\t%d\t%d\n", processes[i].pid, processes[i].bt, processes[i].wt, processes[i].tt);
        sum_tt += processes[i].tt;
        sum_wt += processes[i].wt;
    }
    printf("Avg Waiting-Time: %f\tAvg Turnaround-Time: %f\n", (sum_wt / (float)pc), (sum_tt / (float)pc));
}

int main()
{
    int i, j, k, t;
    
    // Get number of processes
    printf("Enter number of processes: ");
    scanf("%d", &process_count);

    struct Process processes[process_count];
    for (i = 0; i < process_count; i++) {
        processes[i].pid = i;
        processes[i].tt = 0;
        processes[i].wt = 0;
        processes[i].prio = 0;
        processes[i].quantum = 1;

        // Burst Time        
        printf("Enter the Burst Time of Process %d:", i);
        scanf("%d", &processes[i].bt);

        // Prority (User/System)
        printf("System/User Process (0/1) for Process %d?", i);
        scanf("%d", &processes[i].su);

        // Increment number of system processes by one
        if (processes[i].su == 0)
            process_system_count++;
        else
            process_user_count++;
    }
    
    struct Process system_processes[process_system_count];
    struct Process user_processes[process_user_count];

    // Move to the Queues
    j = 0;
    k = 0;
    t = process_count;
    for (i = 0; i < process_count; i++)
        // System
        if (processes[i].su == 0) {
            system_processes[j] = processes[i];
            j++;
        // User
        } else {
            user_processes[k] = processes[i];
            k++;
        }
    printf("%d", process_system_count);


    // SJF for System processes
    printf("System Processes:\n");
    SJF(system_processes, process_system_count);
    print(system_processes, process_system_count);

    // RR for User processes
    printf("\n\nUser Processes:\n");
    RR(user_processes, processes[0].quantum, process_user_count);
    print(user_processes, process_user_count);

    // All
    while (j >= 0) {
        processes[t] = system_processes[j];
        j--;
        t--;
    }
    while (k >= 0) {
        processes[t] = user_processes[k];
        k--;
        t--;
    }
    printf("\nAll:\n");
    print(processes, process_count);

    return 0;
}
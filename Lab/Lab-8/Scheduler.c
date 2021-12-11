#include <stdio.h>
#include <stdlib.h>

int process_count;

struct Process
{
    int pid;        // Process ID
    int bt;         // Burst-Time
    int wt;         // Waiting-Time
    int tt;         // Turnaround-Time
    int prio;       // Priority
    int quantum;    // Quantum-Time
};

void print(struct Process processes[]);
void get_burst_times(struct Process processes[]);
void get_priorities(struct Process processes[]);
void get_quantum(struct Process processes[]);


/**
 * @brief First-Come First-Serve Algorithm Implementation
 * 
 * @param processes 
 */
void FCFS(struct Process processes[]) {    

    // Calculate waiting time
    processes[0].wt = 0;
    for (int i = 1; i < process_count; i++)
        processes[i].wt = processes[i - 1].bt + processes[i - 1].wt;
    
    // Calculate turnaround time
    for (int i = 0; i < process_count; i++)
        processes[i].tt = processes[i].wt + processes[i].bt;
}


/**
 * @brief Shortest-Job-First Algorithm Implementation
 * 
 * @param processes 
 */
void SJF(struct Process processes[]) {

    // Sorting all processos by their burst-time using selection-sort
    for (int i = 0; i < process_count; i++) {
        int pos = i;
        for (int j = i + 1; j < process_count; j++)
            if (processes[j].bt < processes[pos].bt)
                pos = j;
 
        // Swap
        struct Process temp = processes[i];
        processes[i] = processes[pos];
        processes[pos] = temp;
    }

    // Rest is the same as FCFS
    FCFS(processes);
}


/**
 * @brief Priorrity Algorithm Implementation
 * 
 * @param processes 
 */
void Priority(struct Process processes[]) {

    // Sorting all processos by their prioroties using selection-sort
    for (int i = 0; i < process_count; i++) {
        int pos = i;
        for (int j = i + 1; j < process_count; j++)
            if (processes[j].prio < processes[pos].prio)
                pos = j;
 
        // Swap
        struct Process temp = processes[i];
        processes[i] = processes[pos];
        processes[pos] = temp;        
    }

    // Rest is like FCFS
    FCFS(processes);
}


/**
 * @brief Round-Robin Algorithm Implementation
 * 
 * @param processes 
 */
void RR(struct Process processes[], int quantum) {

    // Calculate waiting times
    int current_time = 0;
    // Make a copy of burst times because we don't want the change them at first
    int temp_bt[process_count];
    for (int i = 0; i < process_count; i++)
        temp_bt[i] = processes[i].bt;

    while (1) {

        // Done is true by default
        int is_done = 1;

        for (int i = 0; i < process_count; i++) {

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
    for (int i = 0; i < process_count; i++)
        processes[i].tt = processes[i].wt + processes[i].bt;
}


int main() {

    // Get number of processes
    printf("Enter number of processes: ");
    scanf("%d", &process_count);

    // Construct array of processes and initialize them all
    struct Process processes[process_count];
    for (int i = 0; i < process_count; i++) {
        processes[i].pid = i;
        processes[i].tt = 0;
        processes[i].wt = 0;
        processes[i].prio = 0;
        processes[i].quantum = 0;
    }    

    get_burst_times(processes);        
    printf("FCFS:\n");
    FCFS(processes);
    print(processes);

    // get_burst_times(processes);
    // printf("SJF:\n");
    // SJF(processes);
    // print(processes);

    // get_burst_times(processes);
    // get_priorities(processes);
    // printf("Priority:\n");    
    // Priority(processes);
    // print(processes);

    // get_burst_times(processes);
    // get_quantum(processes);
    // printf("Round Robin:\n");
    // RR(processes, processes[0].quantum);
    // print(processes);
}


/**
 * @brief Get the burst time of all processes
 * 
 * @param processes 
 */
void get_burst_times(struct Process processes[]) {

    printf("Enter Processes Burst Times:\n");
    for (int i = 0; i < process_count; i++)
        scanf("%d", &processes[i].bt);
}


/**
 * @brief Get the priority of all processes
 * 
 * @param processes 
 */
void get_priorities(struct Process processes[]) {
    
    printf("Enter Processes priorities:\n");
    for (int i = 0; i < process_count; i++)
        scanf("%d", &processes[i].prio);
}


/**
 * @brief Get the quantum of all processes
 * 
 */
void get_quantum(struct Process processes[]) {

    int quantum;
    printf("Enter Quantum Time: ");
    scanf("%d", &quantum);

    for (int i = 0; i < process_count; i++)
        processes[i].quantum = quantum;
}


/**
 * @brief Prints the reults of simulation
 */
void print(struct Process processes[]) {

    int sum_tt = 0, sum_wt = 0;

    printf("\nP_ID\tBurst\tWaiting\tTurnaround\n");
    for (int i = 0; i < process_count; i++) {
        printf("%d\t%d\t%d\t%d\n", processes[i].pid, processes[i].bt, processes[i].wt, processes[i].tt);
        sum_tt += processes[i].tt;
        sum_wt += processes[i].wt;
    }
    printf("Avg Waiting-Time: %f\tAvg Turnaround-Time: %f\n", (sum_wt / (float)process_count), (sum_tt / (float)process_count));
}
//
// tsh - A tiny shell program with job control
//  
// CSC 2300
// Thomas Shaffer
// Programming Assignment #4
// Due: December 4, 2008
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

// Miscellaneous constants
#define MAXLINE    1024   // max line size
#define MAXARGS     128   // max args on a command line
#define MAXJOBS      16   // max jobs at any point in time
#define BUFFER      255

// Job states
#define UNDEF 0 // undefined
#define FG 1    // running in foreground
#define BG 2    // running in background
#define ST 3    // stopped

// 
// Jobs states: FG (foreground), BG (background), ST (stopped)
// Job state transitions and enabling actions:
//     FG -> ST  : ctrl-z
//     ST -> FG  : fg command
//     ST -> BG  : bg command
//     BG -> FG  : fg command
// At most 1 job can be in the FG state.
//

// Global variables
extern char **environ;      // defined in libc
char prompt[] = "tsh> ";    // command line prompt (DO NOT CHANGE)
int verbose = 0;            // if true, print additional output
int nextjid = 1;            // next job ID to allocate
char sbuf[MAXLINE];         // for composing sprintf messages

struct job_t {              // The job struct 
    pid_t pid;              // job PID
    int jid;                // job ID [1, 2, ...]
    int state;              // UNDEF, BG, FG, or ST
    char cmdline[MAXLINE];  // command line
};
struct job_t jobs[MAXJOBS]; // The job list

// End global variables

// Function prototypes

// Here are the functions that you will implement
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);
void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

// Here are helper routines that have been provided for you
int parseline(const char *cmdline, char **argv); 
void sigquit_handler(int sig);
void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs); 
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid); 
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid); 
int pid2jid(pid_t pid); 
void listjobs(struct job_t *jobs);
void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

//
// main - The shell's main routine 
//
int main(int argc, char **argv) 
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; // emit prompt (default)

    // Redirect stderr to stdout (so that driver will get all output
    // on the pipe connected to stdout)
    dup2(1, 2);

    // Parse the command line
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
            case 'h':             // print help message
                usage();
                break;
            case 'v':             // emit additional diagnostic info
                verbose = 1;
                break;
            case 'p':             // don't print a prompt
                emit_prompt = 0;  // handy for automatic testing
                break;
	    default:
                usage();
	    }
    }

    // Install the signal handlers
    //
    // NOTE:  These are the ones you will need to implement
    //
    Signal(SIGINT,  sigint_handler);   // ctrl-c
    Signal(SIGTSTP, sigtstp_handler);  // ctrl-z
    Signal(SIGCHLD, sigchld_handler);  // terminated or stopped child

    // This one provides a clean way to kill the shell
    Signal(SIGQUIT, sigquit_handler);
    // Initialize the job list
    initjobs(jobs);

    // Execute the shell's read/eval loop
    while (1) {
    	// Read command line
	    if (emit_prompt) {
	        printf("%s", prompt);
	        fflush(stdout);
	    }
	
        if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
	        app_error("fgets error");
	    if (feof(stdin)) { // End of file (ctrl-d)
	        fflush(stdout);
	        exit(0);
	    }
	    // Evaluate the command line
	    eval(cmdline);
	    fflush(stdout);
	    fflush(stdout);
    } 

    exit(0); // control never reaches here
}
  
// 
// eval - Evaluate the command line that the user has just typed in
// 
// If the user has requested a built-in command (quit, jobs, bg or fg)
// then execute it immediately. Otherwise, fork a child process and
// run the job in the context of the child. If the job is running in
// the foreground, wait for it to terminate and then return.  Note:
// each child process must have a unique process group ID so that the
// background children don't receive SIGINT (SIGTSTP) from the kernel
// when ctrl-c (ctrl-z) is typed at the keyboard.  
//
void eval(char *cmdline) 
{
    int i = 0;  // Counter
    int pid;
    int jid;

    // Quick error check to see if there is any arguments given.
    // Avoids unneeded allocation.
    if( strcmp( cmdline, "\n" ) == 0 )
    {
        return;
    }

    // Allocate memory for argv.
    char** argv = (char**) malloc( MAXARGS*sizeof(char*) );

    if( argv == NULL )
    {
        app_error( "Unable to allocate memory!" );
    }

    for ( i = 0; i < MAXARGS; i++ )
    {
        *(argv+i) = (char*) malloc( BUFFER*sizeof(char) );

        if( *(argv+i) == NULL )
        {
            app_error( "Unable to allocate memory!" );
        }
    }

    sigset_t *set = ( sigset_t* ) malloc( sizeof( sigset_t ) );

    if ( set == NULL )
    {
        app_error( "Unable to allocate memory!" );
    }

    // parse the command line and set an indicator as to whether or not the 
    // command is to run in the background      
    int bg = parseline( cmdline, argv );

    // initialize the signal set
    sigemptyset( set );

    // add the SIGCHLD signal to the signal mask
    if( sigaddset( set, SIGCHLD ) == -1 )
    {
        unix_error( "Unable to add signal!" );
    }

    // check to see if it is one of the built-in commands
    if( builtin_cmd( argv ) == 0 )
    {
        // send a SIG_BLOCK to the signal mask of this calling thread
        if ( sigprocmask( SIG_BLOCK, set, NULL ) == -1 )
        {
            unix_error( "Unable to send signal!" );
        }

        pid = fork();
        if ( pid < 0 )
        {
            unix_error( "Unable to create child process!" );
        }
        else if ( pid == 0 )    // Child
        {
            // make a new process group, check for error
            if ( setpgid( 0,0 ) == -1 )
            {
                unix_error( "Unable to create the process group!" );
            }

            // send a SIG_UNBLOCK to the signal mask of this calling thread
            if ( sigprocmask( SIG_UNBLOCK, set, NULL ) == -1 )
            {
                unix_error( "Unable to send signal!" );
            }

            //error check existence of command and arguments
            //(in this environment)

            // run the process in the foreground
                // send a SIG_UNBLOCK to the signal mask of this calling thread
                // wait for the foreground child to terminate.
            // run the process in the background
                // send a SIG_UNBLOCK to the signal mask of this calling thread
                // map process ID to job ID
                // print job ID, process ID, and command line
            if ( sigprocmask( SIG_UNBLOCK, set, NULL ) == -1 )
            {
                unix_error( "Unable to send signal!" );
            }
           
            if( execv( argv[0], argv ) == -1 )
            {
                if ( errno == ENOENT )
                {
                    printf( "%s: Command not found.\n", argv[0] );
                }
                
                free( (void*) set );
                exit(0);
            }
        }
        else    // Parent
        {
            if ( bg )
            {
                if ( addjob( jobs, (pid_t) pid, BG, cmdline ) == 0 )
                {
                    app_error( "Unable to add job!" );
                }

                if ( sigprocmask( SIG_UNBLOCK, set, NULL ) )
                {
                    unix_error( "Unable to send signal!" );
                }

                if ( (jid = pid2jid((pid_t) pid)) == 0 )
                {
                    app_error( "Unable to obtain jid!" );
                }

                printf( "[%d] (%d) %s", jid, pid, cmdline );
            }
            else
            {
                if ( addjob( jobs, (pid_t) pid, FG, cmdline ) == 0 )
                {
                    app_error( "Unable to add job!" );
                }

                if( sigprocmask( SIG_UNBLOCK, set, NULL ) == -1 )
                {
                    unix_error( "Unable to send signal!" );
                }

                waitfg((pid_t) pid);
            }
        }
    }

    // Free memory.
    free( (void*) set );

    return;
}
 
// 
// parseline - Parse the command line and build the argv array.
// 
// Characters enclosed in single quotes are treated as a single
// argument.  Return true (1) if the user has requested a BG job, false if
// the user has requested a FG job.  
//
int parseline(const char *cmdline, char **argv) 
{
    static char array[MAXLINE]; // holds local copy of command line
    char *buf = array;          // ptr that traverses command line
    char *delim;                // points to first space delimiter
    int argc;                   // number of args
    int bg;                     // background job?
    
    strcpy(buf, cmdline);
    
    buf[strlen(buf)-1] = ' ';  // replace trailing '\n' with space
    while (*buf && (*buf == ' ')) // ignore leading spaces
	buf++;

    // Build the argv list */
    argc = 0;
    if (*buf == '\'') {
        buf++;
        delim = strchr(buf, '\'');
    }
    else {
        delim = strchr(buf, ' ');
    }
    while (delim) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) // ignore spaces
            buf++;
        if (*buf == '\'') {
            buf++;
            delim = strchr(buf, '\'');
        }
        else {
            delim = strchr(buf, ' ');
        }
    }
    argv[argc] = NULL;

    if (argc == 0)  // ignore blank line
        return 1;
    // should the job run in the background?
    if ((bg = (*argv[argc-1] == '&')) != 0) {
        argv[--argc] = NULL;
    }
    return bg;
}

// 
// builtin_cmd - If the user has typed a built-in command then execute
//    it immediately.  
//
int builtin_cmd(char **argv) 
{
    // Process "quit" command
    if ( strcmp( argv[0], "quit" ) == 0 )
    {
        free( (void*) argv );
        clearjob( jobs );
                  
        exit(0);
    }

    // Process "jobs" command
    if ( strcmp( argv[0], "jobs" ) == 0 )
    {
        listjobs( jobs );
        return 1;
    }

    // Process bg or fg command
    if ( strcmp( argv[0], "bg" ) == 0 || strcmp( argv[0], "fg" ) == 0 )
    {
        do_bgfg( argv );
        return 1;
    }

    return 0;     // not a built-in command
}

// 
// do_bgfg - Execute the builtin bg and fg commands
//
void do_bgfg(char **argv) 
{
    // print error if user does not specify an argument
    if ( argv[1] == NULL )
    {
        printf( " %s Command requires a PID or a %%jobid argument\n",
                 argv[0] );
        return;
    }

    // create termporary pointer to job structure
    struct job_t *temp = (struct job_t*) malloc(sizeof(struct job_t));

    if ( temp == NULL )
    {
        app_error( "Unable to allocate memory!" );
    }

    // if the user specifies a job ID, look for the job in the list of jobs
        // get the structure for the specified job ID
            // make sure only digits specified for a job number
        // print error if job does not exist
    if ( argv[1][0] == '%' )
    {
        // Check that argument is a digit.
        int i = 1;
        char c = argv[1][i];
        while( c != '\0' )
        {
            if ( !isdigit( c ) )
            {
                printf( "%s Argument must be a PID or a %%jobid argument\n",
                argv[0] );
                return;
            }

            i++;
            c = argv[1][i];
        }

        temp = getjobjid( jobs, atoi( (*(argv+1)+1) ) );
        
        if ( temp == NULL )
        {
            printf( "(%s) No such job\n", argv[1] );
            return;
        }
    }
    else
    {
        // Check that argument is a digit.
        int i = 0;
        char c = argv[1][i];
        while( c != '\0' )
        {
            if ( !isdigit( c ) )
            {
                printf( "%s Argument requires a PID or a %%jobid argument\n",
                argv[0] );
                return;
            }

            i++;
            c = argv[1][i];
        }

        temp = getjobpid( jobs, (pid_t) atoi( *(argv+1) ) );

        if ( temp == NULL )
        {
            printf( "(%s) No such process\n", argv[1] );
            return;
        }
    }

    // execute the "bg" command
    if ( strcmp( argv[0], "bg" ) == 0 )
    {
        printf( "[%d] (%d) %s", temp->jid, temp->pid, temp->cmdline );
        // move the state of the process to BG
        temp->state = BG;
        kill( temp->pid, SIGCONT );
    }
    // execute the "fg" command
    else
    {
        if ( temp->state == ST )
        {
            sigset_t *set = ( sigset_t* ) malloc( sizeof( sigset_t ) );

            if ( set == NULL )
            {
                unix_error( "Unable to allocate memory!" );
            }

            sigfillset( set );

            if ( sigprocmask( SIG_BLOCK, set, NULL ) == -1 )
            {
                unix_error( "Unable to block signal!" );
            }

            kill( temp->pid, SIGCONT );

            if ( sigprocmask( SIG_UNBLOCK, set, NULL ) == -1 )
            {
                unix_error( "Unable to unblock signal!" );
            }

            temp->state = FG;
            waitfg( temp->pid );

            free( (void*) set );
        }
        else
        {
            temp->state = FG;
            waitfg( temp->pid );
        }
    }

    // Free memory.
    temp = NULL;
    free( (void*) temp );

    return;
}

// 
// waitfg - Block until process pid is no longer the foreground process
//
void waitfg(pid_t pid)
{
    // get job id for specified pid
    // while job is not stopped or empty
    while( getjobpid( jobs, pid ) != NULL &&
           (*(getjobpid( jobs, pid ))).state == FG )
    {
            // sleep for a second
            // and see if the the job is still running
            sleep(1);
    }

    return;
}

////////////////////////
// Signal handlers    //
////////////////////////

// 
// sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
//     a child job terminates (becomes a zombie), or stops because it
//     received a SIGSTOP or SIGTSTP signal. The handler reaps all
//     available zombie children, but doesn't wait for any other
//     currently running children to terminate.  
//
void sigchld_handler(int sig) 
{ 
    int pid;
    int status;
    int jid;

    // Suspend execution of the current process until a child as 
    // specified by the pid has exited,  returning immediately if no 
    // child has exited or children are stopped.
    pid = (int) waitpid( (pid_t)-1, &status, WNOHANG );

    if ( pid < 0 )
    {
        unix_error( "wait error!" );
    }

    struct job_t *temp = getjobpid( jobs, (pid_t) pid );
    jid = pid2jid( (pid_t) pid );
    // if status indicates process finished, delete the job
    // else if status indicates process terminated by a signal, print
    // else if status indicates process stopped, print and change status
    // else child terminated abnormally
	if ( WIFEXITED( status ) )
    {
        deletejob( jobs, (pid_t) pid );
    }
    else if( WSTOPSIG(status) )
    {
        if ( temp->state == BG )
        {
            printf( "Job [%d] (%d) stopped by signal %d!\n", jid, pid, status );
            temp->state = ST;
        }
        else if ( temp->state == FG )
        {
            temp->state = ST;
        }
    }
    else if ( WIFSTOPPED(status) )
    {
        if ( temp->state == BG )
        {
            printf( "Job [%d] (%d) stopped.\n", jid, pid );
            getjobpid( jobs, (pid_t) pid )->state = ST;
        }
        else if ( temp->state == FG )
        {
            temp->state = ST;
        }
    }

    return;
}

//
//   sigint_handler - The kernel sends a SIGINT to the shell whenver the
//     user types ctrl-c at the keyboard.  Catch it and send it along
//     to the foreground job, and indicate what job/process was terminated.
// 
void sigint_handler(int sig) 
{
    // get process id for foreground job
    int pid = (int) fgpid( jobs );
    int jid;

    if ( pid > 0 )
    {
        // get job id for specified process id
        jid = getjobpid( jobs, (pid_t) pid )->jid;

        // reinitialize the signal set, add SIGCHLD, and send a
        // SIG_BLOCK to the signal mask of this calling thread
        // send kill to process with SIGINT signal
        // send SIG_UNBLOCK to the signal mask
        // delete job
        // print notification of job termination
        sigset_t *set = (sigset_t *) malloc( sizeof( sigset_t ) );

        if ( set == NULL )
        {
            app_error( "Unable to allocate memory!" );
        }

        sigfillset( set );

        if ( sigaddset( set, SIGCHLD ) == -1 )
        {
            unix_error( "Unable to add signal!" );
        }

        if ( sigprocmask( SIG_BLOCK, set, NULL ) == -1 )
        {
            unix_error( "Unable to send signal!" );
        }

        if( kill( (pid_t) pid, SIGINT ) == -1 )
        {
            unix_error( "kill failed: " );
        }

        if ( sigprocmask( SIG_UNBLOCK, set, NULL ) == -1 )
        {
            unix_error( "Unable to send signal!" );
        }

        deletejob( jobs, (pid_t) pid );
        
        printf( "Job [%d] (%d) terminated by signal %d\n", jid, pid, sig );

       free( (void*) set );
    }
    
    return;
}

//
// sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
//     the user types ctrl-z at the keyboard. Catch it and suspend the
//     foreground job by sending it a SIGTSTP, and indicate what
//     job/process was stopped.
//
void sigtstp_handler(int sig) 
{
    // get process id for foreground job
    int pid = (int) fgpid( jobs );
    int jid;

    if ( pid > 0 )
    {
        // reinitialize the signal set, add SIGCHLD, and send a
        // SIG_BLOCK to the signal mask of this calling thread
        // get job id for specified process id
        // send kill to process with SIGTSTP signal
        // send SIG_UNBLOCK to the signal mask
        // change status of job to ST
        sigset_t *set = (sigset_t * ) malloc( sizeof( sigset_t ) );

        if ( set == NULL )
        {
            app_error( "Unable to allocate memory!" );
        }

        sigfillset( set );

        if ( sigaddset( set, SIGCHLD ) == -1 )
        {
            unix_error( "Unable to add signal!" );
        }

        if ( sigprocmask( SIG_BLOCK, set, NULL ) == -1 )
        {
            unix_error( "Unable to send signal!" );
        }

        jid = pid2jid( (pid_t) pid );
        if ( kill( (pid_t) pid, SIGTSTP ) == -1 )
        {
            unix_error( "kill error: " );
        }

        if ( sigprocmask( SIG_UNBLOCK, set, NULL ) == -1 )
        {
            unix_error( "Unable to send signal!" );
        }

        getjobpid( jobs, (pid_t) pid )->state = ST;

        printf( "Job [%d] (%d) stopped by signal %d\n", jid, pid, sig );
        free( (void*) set );
    }

    return;
}

////////////////////////////
// End signal handlers    //
////////////////////////////

//////////////////////////////////////////////////
// Helper routines that manipulate the job list //
//////////////////////////////////////////////////

// clearjob - Clear the entries in a job struct
void clearjob(struct job_t *job) {
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}

// initjobs - Initialize the job list
void initjobs(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
        clearjob(&jobs[i]);
}

// maxjid - Returns largest allocated job ID
int maxjid(struct job_t *jobs) 
{
    int i, max=0;
    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].jid > max)
            max = jobs[i].jid;
    return max;
}

// addjob - Add a job to the job list
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline) 
{
    int i;
  
    if (pid < 1)
        return 0;
 
    for (i = 0; i < MAXJOBS; i++) 
    {
        if (jobs[i].pid == 0) 
        {
            jobs[i].pid = pid;
            jobs[i].state = state;
            jobs[i].jid = nextjid++;
            if (nextjid > MAXJOBS)
                nextjid = 1;
            strcpy(jobs[i].cmdline, cmdline);
            if (verbose)
                printf("Added job [%d] %d %s : %s\n", jobs[i].jid, 
                       jobs[i].pid, jobs[i].cmdline, cmdline);
            return 1;
        }
    }
    printf("Tried to create too many jobs\n");
    return 0;
}

// deletejob - Delete a job whose PID=pid from the job list
int deletejob(struct job_t *jobs, pid_t pid) 
{
    int i;

    if (pid < 1)
        return 0;
    for (i = 0; i < MAXJOBS; i++) {
        if (jobs[i].pid == pid) {
            clearjob(&jobs[i]);
            nextjid = maxjid(jobs)+1;
            return 1;
        }
    }
    return 0;
}

// fgpid - Return PID of current foreground job, 0 if no such job
pid_t fgpid(struct job_t *jobs) 
{
    int i;

    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].state == FG)
            return jobs[i].pid;
    return 0;
}

// getjobpid  - Find a job (by PID) on the job list
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) 
{
    int i;

    if (pid < 1)
        return NULL;
    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].pid == pid)
            return &jobs[i];
    return NULL;
}

// getjobjid  - Find a job (by JID) on the job list
struct job_t *getjobjid(struct job_t *jobs, int jid) 
{
    int i;

    if (jid < 1)
        return NULL;
    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].jid == jid)
            return &jobs[i];
    return NULL;
}

// pid2jid - Map process ID to job ID
int pid2jid(pid_t pid) 
{
    int i;

    if (pid < 1)
        return 0;
    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].pid == pid)
            return jobs[i].jid;
    return 0;
}

// listjobs - Print the job list
void listjobs(struct job_t *jobs) 
{
    int i;
    
    for (i = 0; i < MAXJOBS; i++) {
        if (jobs[i].pid != 0) {
            printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
            switch (jobs[i].state) {
                case BG: 
                    printf("Running ");
                    break;
                case FG: 
                    printf("Foreground ");
                    break;
                case ST: 
                    printf("Stopped ");
                    break;
                default:
                    printf("listjobs: Internal error: job[%d].state=%d ", 
                           i, jobs[i].state);
            }
            printf("%s", jobs[i].cmdline);
        }
    }
}

//////////////////////////////////
// end job list helper routines //
//////////////////////////////////

/////////////////////////////
// Other helper routines   //
/////////////////////////////

//
// usage - print a help message
//
void usage(void) 
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

//
// unix_error - unix-style error routine
//
void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

//
// app_error - application-style error routine
//
void app_error(char *msg)
{
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

//
// Signal - wrapper for the sigaction function
//
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); // block sigs of type being handled
    action.sa_flags = SA_RESTART; // restart syscalls if possible

    if (sigaction(signum, &action, &old_action) < 0)
        unix_error("Signal error");
    return (old_action.sa_handler);
}

//
// sigquit_handler - The driver program can gracefully terminate the
//    child shell by sending it a SIGQUIT signal.
//
void sigquit_handler(int sig) 
{
    if (verbose)
        printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}

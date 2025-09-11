/****************
LE2: Introduction to Unnamed Pipes
****************/
#include <unistd.h> // pipe, fork, dup2, execvp, close
#include <iostream>
#include <sys/wait.h>
using namespace std;

int main () {
    // lists all the files in the root directory in the long format
    char* cmd1[] = {(char*) "ls", (char*) "-al", (char*) "/", nullptr};
    // translates all input from lowercase to uppercase
    char* cmd2[] = {(char*) "tr", (char*) "a-z", (char*) "A-Z", nullptr};

    // TODO: add functionality
    // Create pipe
    int fds[2]; // creates place for pipe
    if (pipe(fds) == -1) { // pipes and checks if error while pipping
        cout << "Error Pipping \n";
        return 1; // error while pipping so exit entire program
    }

    // Create child to run first command
    pid_t pid = fork(); // forks and stores the pid in pid
    if(pid == -1) { // if pid = -1 then the forking did not execute correctly
        cout << "Error Forking \n";
        return 1; // if fork bad then exit program
    }
    // In child, redirect output to write end of pipe
    // Close the read end of the pipe on the child side.
    // In child, execute the command
    if(pid == 0) {
        dup2(fds[1], STDOUT_FILENO); // redirect the standard output to the write of the pipe
        close(fds[0]); // close read of pipe
        if(execvp(cmd1[0], cmd1) == -1) { // replace child with cmd1
            cout << "Failed to run cmd1"; // if failed so exec returned -1 then say it failed and exit
            exit(1);
        }
    }
    
    // Create another child to run second command
    // In child, redirect input to the read end of the pipe
    // Close the write end of the pipe on the child side.
    // Execute the second command.
    pid_t pid2 = fork(); // new child so create new pid
    if(pid2 == -1) { // if fork fail exit program
        cout << "Error Forking \n";
        return 1;
    }
    if(pid2 == 0) { // in child 
        dup2(fds[0], STDIN_FILENO); // replace standard input with read end of pipe
        close(fds[1]); // close write end of pipe
        if(execvp(cmd2[0], cmd2) == -1) { // replace child process with cmd2
            cout << "Failed to run cmd2"; // if failed exec then exit the child
            exit(1);
        }
    }
    // Reset the input and output file descriptors of the parent.
    else{
        close(fds[0]); // close read
        close(fds[1]); // close write of pipe
        waitpid(pid, nullptr, 0); // wait for the first child process to finish
        waitpid(pid2, nullptr, 0); // wait for second child process to finish
    }
}

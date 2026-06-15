# Shivam Shell

A Unix-like shell built in C from scratch to explore process management, pipes, redirection, terminal control, and interactive command-line interfaces.

## Features

### Command Execution
- Execute external commands using fork() and execvp()
- Built-in commands:
  - cd
  - exit
  - history

### I/O Redirection
- Output redirection

bash echo hello > file.txt 

- Append redirection

bash echo hello >> file.txt 

- Input redirection

bash wc < file.txt 

### Pipes
- Single pipe

bash cat file.txt | wc 

- Multiple pipes

bash cat file.txt | grep hello | wc 

### Background Processes
Run commands in the background using &

bash sleep 30 & 

### Process Management
- Fork-based process execution
- Zombie process cleanup using waitpid(..., WNOHANG)
- Signal handling for Ctrl + C

### Command History
- Stores command history during shell execution
- View history using:

bash history 

### Interactive Line Editor
Implemented without GNU Readline.

#### Cursor Navigation
- Left Arrow
- Right Arrow

#### History Navigation
- Up Arrow
- Down Arrow

#### Editing Support
- Mid-line insertion
- Mid-line deletion (Backspace)

#### Keyboard Shortcuts
| Shortcut | Action |
|-----------|----------|
| Ctrl + A | Move to beginning of line |
| Ctrl + E | Move to end of line |
| Ctrl + L | Clear screen |
| Ctrl + U | Delete entire line |

### TAB Completion
- File completion
- Directory completion
- Multiple match suggestions

Examples:

bash cat te<TAB> 

bash cd Doc<TAB> 

---

## Architecture

User Input     |     v read_input()     |     v parse_input()     |     v execute_command()     |     +---- Built-in Commands     |     +---- Redirection     |     +---- Pipes     |     +---- Background Execution     |     +---- Process Management

### Project Structure

. ├── main.c ├── parser.c ├── builtin.c ├── executor.c ├── process.c ├── pipe.c ├── redirection.c ├── history.c ├── input.c ├── autocomplete.c ├── history_navigation.c ├── line_editor.c ├── Makefile

---

## Key Concepts Learned

### Process Management
- fork()
- execvp()
- waitpid()

### Inter-Process Communication
- pipe()
- dup2()

### Signal Handling
- SIGINT
- Background processes
- Zombie process cleanup

### Terminal Programming
- Raw terminal mode using termios
- Escape sequences
- Interactive line editing

### File Handling
- open()
- close()
- O_RDONLY
- O_CREAT
- O_TRUNC
- O_APPEND

---

## Build

bash make 

---

## Run

bash ./shell 

---

## Example Session

bash shivam-shell> pwd /home/shivam  shivam-shell> echo hello > test.txt  shivam-shell> cat test.txt hello  shivam-shell> cat test.txt | wc 1 1 6  shivam-shell> sleep 10 & [Background] PID : 12345  shivam-shell> history 1 pwd 2 echo hello > test.txt 3 cat test.txt 4 cat test.txt | wc 5 sleep 10 & 

---

## Future Enhancements

- Job control (jobs, fg, bg)
- Persistent history
- Auto-suggestions
- Syntax highlighting
- Alias support

---

## Author

Shivam Kachawar

Built as a systems programming project to understand how Unix shells work internally.

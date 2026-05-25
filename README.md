# NS Shell

NS Shell (`ns`) is a simple Unix-like shell written in C.  
It provides a command prompt, supports a few built-in commands, and can launch external programs.

## Features

- Interactive command prompt
- Built-in commands:
  - `cd`
  - `help`
  - `exit`
- Executes external commands using `fork` + `execvp`

## Project Structure

- `main.c` — complete shell implementation

## Build

From the repository root:

```bash
gcc -Wall -Wextra -pedantic main.c -o ns
```

## Run

```bash
./ns
```

You will see a prompt:

```text
>
```

Type commands and press Enter.

## Built-in Commands

- `cd <path>`: change current directory
- `help`: show built-in help text
- `exit`: exit the shell

## Notes

- Input parsing is whitespace-based and minimal.
- For commands other than built-ins, NS Shell delegates execution to the operating system.

# Simple command line based editor - Written in C

> Usage:
> ./editor [-c] FILENAME

The option flag `-c` allow you to create a file if it not exits yet.

The editor is based on command line, so for each interaction you should tell
which action you want to perform.

When you open the editor a `:` will appear at screen. The semicolon indicates that
the editor is waiting for a command. The syntax for commands are: `:COMMAND ARGUMENTS`

Some examples:

```
:a This text will be appended to file

# P command will print the current file content
:p

```

Compiling:

```
make editor
```

Checkout `editor.c` to see a list of available commands

Credits:

This editor is based on simted3 source code avaliable at: http://jwu.50webs.com/Archive/src/simted3/simted3.pdf

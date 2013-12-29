# Simple command line based editor

> Usage:
> ./editor FILE

The editor is based on command line, so for each interaction you should tell
which action you want to perform.

When you open the editor a `:` will appear at screen. The semicolon indicates that
the editor is waiting for a command. The syntax for commands are: `:COMMAND ARGUMENTS`


## Available Commands

* a - append a text to file
* d - remove content from file
* p - print the content inside file
* s - save file
* q - exit editor
* m - Insert multiple lines at once
* r - Add desc

Some examples:

```

:a This text will be appended to file
:p

```

Compiling:

```
make editor
```

Check out `editor.c` to see a list of available commands

Credits:

This editor is based on simted3
source code avaliable at: http://jwu.50webs.com/Archive/src/simted3/simted3.pdf

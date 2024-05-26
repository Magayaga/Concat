# `concat`

`concat` command in Windows, Linux, and Unix operating systems is primarily used to display the contents of a file on the terminal. It was developed by [Cyril John Magayaga](https://github.com/magayaga) and written in **C**. The [`type`](https://github.com/MicrosoftDocs/windowsserverdocs/blob/main/WindowsServerDocs/administration/windows-commands/type.md) for Microsoft Windows and DOS operating systems and [`cat`](https://github.com/coreutils/coreutils/blob/master/src/cat.c) and [`nl`](https://github.com/coreutils/coreutils/blob/master/src/nl.c) for Linux and Unix operating systems.

`type` (Windows-based concatenare files) + `cat` (Unix-based concatenare files)  + `nl` (Unix-based numbering lines) = `concat` is the new modern concatenate files!
## Getting Start

### Windows, Linux, and Unix
```shell
# Download the Concat command
$ git clone https://github.com/Magayaga/Concat.git

# Change the directory of the Concat
$ cd Concat

# Run the program (batchfile)
$ ./make

# or (make)
$ make

# Open the application
$ ./concat README.md

```

## Examples
```shell
$ ./concat examples/a.txt examples/b.txt

$ ./concat -A examples/a.txt examples/b.txt

$ ./concat examples/a.txt > examples/c.txt

$ ./concat --nl examples/a.txt

```

## Copyright
Copyright (c) 2023 - 2024 Cyril John Magayaga. All rights reserved.

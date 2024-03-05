
# myls - Custom LS Command Implementation

## Overview
The `myls` program is a custom implementation of the Unix `ls` command. It is designed to display directory contents and file details in two distinct modes.

## Modes of Operation

### Mode 1: Directory Listing
- Run `myls` without any parameters to display the contents of the current directory.
- The output format resembles the `ls -la` command on Linux, showing detailed file and directory information.
- Files are sorted by name, and column widths are adjusted according to the longest entry.

#### Example:
```
> ./myls

drwxr-xr-x 3 user group 4096 Oct 02 16:46 .
drwxr-xr-x 10 user group 4096 Aug 21 16:46 ..
-rw-r----- 1 user group 26 Oct 20 15:03 file1
...
```

### Mode 2: File Information
- Run `myls` with a single parameter, the filename, to display detailed information about that file.
- Information includes file type, full path, size (or subdirectory count for directories), permissions, access/modification/status change times.
- For regular text files, the first 77 characters are displayed.

#### Examples:
```
> ./myls file

File Information:
Type: regular file
Path: /path/to/file
Size: 12 bytes
Permissions: user: rw, group: r, others: r
Last accessed: October 25, 2017, 12:49:01
Last modified: October 25, 2017, 11:49:33
Last status change: October 25, 2017, 11:49:33
First 77 characters: ...
```

```
> ./myls link

File Information:
Type: symbolic link
Path: /path/to/link -> /path/to/target
Size: 4 bytes
Permissions: user: rwx, group: rwx, others: rwx
...
```


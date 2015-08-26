# swarm
SwaRM: parallelized secure delete application

SwaRM is an application to securely wipe and unlink files and directories where the files to be overwritten are processed by a user specified number of child processes which request the next resource to wipe and remove from the parent process.  SwaRM may also be used to securely remove files in a serial single process mode.

## TODO

* Add explicit configuration for policy on deleting symlinks and hardlinks
* Add support for renaming or overwriting directories before removing
* Add macros for using arc4random_buf shim when compiling for Linux
* Add configuration for number of random write passes, number of zero write passes
* Add automated regressions testing
* Add separate tasks in Makefile for debug versus non-debug builds
* Add man page
* Consider breadth versus depth first directory traversal and its implications

## Setup

```sh
$ make swarm
$ cp swarm ~/bin (optional)
$ swarm <arguments>
```

## Usage

Delete 3 files

```sh
$ swarm file1 file2 file3
```

Recursively delete 2 directories and 2 files

```sh
$ swarm -r dir1 dir2 file1 file2
```

Create 4 child processes to delete a list of files in parallel

```sh
$ swarm -w 4 file1 file2 file3 file4 file5 file6 file7 file8 file9 file10
```

Show verbose output of the files and directories being removed as it runs

```sh
$ swarm -v file1 file2 file3
```

Show the files and directories that would be removed but don't actually remove them 

```sh
$ swarm -v -d file1 file2 file3
```

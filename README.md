# swarm
SwaRM: parallelized secure delete application

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

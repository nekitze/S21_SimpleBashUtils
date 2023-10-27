# Simple Bash Utils

My implementation of Bash text utilities: cat, grep.

## Build and Run
1. Go to ```src/cat``` or ```src/grep``` folder
2. Do ```make s21_cat``` or ```make s21_grep``` to compile executable file
3. Run utility with argumets ```./s21_cat [-benstv] [file ...]``` or ```./s21_grep [OPTION]... PATTERNS [FILE]...```

## Usage
### ```s21_cat:```<br>
![cat](images/cat.gif)
### ```s21_grep:```<br>
![grep](images/grep.gif)

---
### ```s21_cat``` Options:

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -b (GNU: --number-nonblank) | numbers only non-empty lines |
| 2 | -e implies -v (GNU only: -E the same, but without implying -v) | but also display end-of-line characters as $  |
| 3 | -n (GNU: --number) | number all output lines |
| 4 | -s (GNU: --squeeze-blank) | squeeze multiple adjacent blank lines |
| 5 | -t implies -v (GNU: -T the same, but without implying -v) | but also display tabs as ^I  |

### ```s21_grep``` Options:

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -e | pattern |
| 2 | -i | Ignore uppercase vs. lowercase.  |
| 3 | -v | Invert match. |
| 4 | -c | Output count of matching lines only. |
| 5 | -l | Output matching files only.  |
| 6 | -n | Precede each matching line with a line number. |
| 7 | -h | Output matching lines without preceding them by file names. |
| 8 | -s | Suppress error messages about nonexistent or unreadable files. |
| 9 | -f file | Take regexes from a file. |
| 10 | -o | Output the matched parts of a matching line. |





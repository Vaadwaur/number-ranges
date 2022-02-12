# number-ranges
Tools to convert lists of numbers

### explode - takes input from stdin and/or files and print each number in the range
$ echo "1 5" | ./explode -b 00 -a xx<br>
001xx<br>
002xx<br>
003xx<br>
004xx<br>
005xx<br>

### implode - takes input from stdin and/or files and print contained ranges and how big each range is

$ ./implode -s <(seq 10 15) <(seq 5 9)<br>
5 15 11

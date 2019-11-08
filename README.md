# Operating Systems Final Assignment, AUTh [2018]
> <TODO> .

<p align="justify">
This is an <i>experimental</i> application developed as part of the course "Operating Systems" assignment, that took place in the Department of Electrical & Computer Engineering at Aristotle University of Thessaloniki in 2018.
</p>

<p align="justify">
The goal is to create my version of linux Bash Shell. Where,
<ul>
<li> cd, help and quit are handled as Built-In functions. </li>
&emsp;&emsp;<i> For other functions type @function_name @arguments. If you need more info, type man @function_name. </i>
<li> <b> Improper space handle. </b>
<li> <b> Redirection with > handle. </b>
<li> <b> Redirection with < handle. </b>
<li> Two modes supported:
  <ul>
        <li> <b> Interactive</b>, when no arguments were given. </li>
        <li> <b> Batch</b>, when 1 (one) argument (file) was given. </li>
  </ul>
</ul>
<b>Makefile supports</b>: 
		<ul>
    <li> <b>all</b>, to make the object file,</li>
		<li> <b>clean</b>, to delete it. </li>
    </ul>
<b>Valid instructions</b>:
<ul>
<li> echo hello ; echo world ; echo true </li> 
<li> echo hello && echo world && echo true </li>
<li> echo hello ; echo world && echo true </li>
<li> eco hello && echo world ; echo true </li>
<li> echo hello > file_1 > file_2 > file_3 </li>
<li> wc -l < file_1 < file_2 < file_3 &emsp;&emsp;&emsp; <i> if file_1, file_2, file_3 exists</i> </li>
<li> wc -l < file_1 > file_2 &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;   <i> if file_1 exists </i> </li>
</ul>
<b>Invalid instructions</b>:
<ul>
<li> Instruction(s) with piping ( | ). No error message, but the pipe symbol will be ignored. </li>
<li> Instruction(s) with 3 or more sequentially ampersands. (i.e echo hello &&& echo world) </li>
<li> Instruction(s) with 2 or more sequentially semicolons. (i.e echo hello ;; echo world) </li>
<li> Instruction(s) with more than 512 characters. </li>
<li> Other Build- In instruction(s) like: jobs, fg, bg etc. </li>
<li> NULL commands, like:
	<ul>
  <li>    </li> 
	<li> ;  </li>
	<li> && </li>
	<li> echo hello ;&emsp;; echo world </li>
	<li> echo hello &&&emsp;&& echo world </li>
  </ul> </li>
</ul>
<b><i>PS_1</i></b>. At the naming of the files do not use 'name' or "name", just name. Otherwise the '&emsp;' or "&emsp;" will appear at the namefile.

&emsp;&emsp;<b>Proper usage</b>: echo hello > file

&emsp;&emsp;<b>Improper usage</b>: echo hello > 'file' or echo hello > "file"

<b><i>PS_2</b></i>. Maximum instructions = 250 (per line) 
</p>

---

## Execution
<TODO>
To execute the code, you first need to cross compile it (for Raspberry Pi) using:
```sh
make all
```
secure copy it: 
```sh
sudo scp MessengerApp @username@tohost:/remote/directory
(i.e. sudo scp MessengerApp root@192.168.0.1:~ for root user and random ip)
```
and then run using:
```sh
./MessengerApp
```
<TODO>

---

## Status

As of the completion of the project, it will NOT be maintained. By no means should it ever be considered stable or safe to use, as it may contain incomplete parts, critical bugs and security vulnerabilities.

---

## Support

Reach out to me:

- [mpalaourg's email](mailto:gbalaouras@gmail.com "gbalaouras@gmail.com")

---

## License

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://github.com/mpalaourg/OperatingSystems_MyBash/blob/master/LICENSE)

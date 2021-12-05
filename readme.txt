### HOW TO RUN ### 
Note this is a gcc/g++ build using CMake, and requires R if you would like to compile the paper as well.
For some not-yet-known reason, R fails to install on my Linux VM (so have tested on my Windows machine and 
on a WSL(Windows Subsystem for Linux) terminal with another machine. So the entire project should work on a Linux machine easily*,
though this was not tested.

1) *Make sure all the dependencies have been installed
2) $ bash knit.sh -r
	options:
		-b <build directory, default: tsp-build> -> sets build directory
		-o <output directory, default: output>   -> sets output directory
		-r 					 -> runs/reruns cmake configure for build
		-n					 -> does not run c++ code (only R)
		-m					 -> does not run R code (only c++)
		-c 					 -> runs make clean for build
		-w 					 -> wipes and removes build directory

3) View article.html or article.pdf in output directory (the html has video that the pdf does not support at the moment)

#!/bin/bash
#exec 3>&1 4>&2
#trap 'exec 2>&4 1>&3' 0 1 2 3
#exec 1>log.txt 2>&1

export CC=/c/Users/sadam/gcc/bin/gcc
export CXX=/c/Users/sadam/gcc/bin/g++

export src=$PWD/../
export scripts=scripts
export rstudio=rstudio
export build=../tsp-build
export rstudio_img=../$build/img
export output=../output
export out_image=$src/$output/img
export reconfigure=false
export clean=false
export wipe=false
export cplus=true
export rlang=true

cd $src

if [[ ! -e $build ]]; then
    mkdir $build
elif [[ ! -d $build ]]; then
    echo "$build already exists but is not a directory" 1>&2
fi

if [[ ! -e $output ]]; then
    mkdir $output
elif [[ ! -d $output ]]; then
    echo "$output already exists but is not a directory" 1>&2
fi


if [[ ! -e $out_image ]]; then
    mkdir $out_image
elif [[ ! -d $out_image ]]; then
    echo "$out_image already exists but is not a directory" 1>&2
fi


while getopts b:o:rnmcw flag
do
    case "${flag}" in
        b) 
		echo "received -build with $OPTARG"
		build=${OPTARG}
		;;
        o) 
		echo "received -output with $OPTARG"
		output=${OPTARG}
		;;
	r)
		echo "received -reconfigure (cmake src)"
		export reconfigure=true
		;;
	n)
		echo "received -no_cplusplus (only running R)"
		export cplus=false
		;;
	m)
		echo "received -no_rlang (only running cplusplus)"
		export rlang=false
		;;
	c) 
		echo "received -clean (make clean)"
		export clean=true
		;;
	w) 	
		echo "received -wipe (cmake clean and remove build directory)"
		export wipe=true
		;;
    esac
done

echo "Source (Absolute) Directory: $src"
echo "Build (Relative) Directory : $build"
echo "Output (Relative) Directory: $output"
rsrc=$(realpath --relative-to="$build" "$src")
echo "Source (Relative From Build) Directory: $rsrc"

if [ "$clean" = true ] ; then
	cd $src$build
	make clean
	echo "Swept and dusted..."
	exit
fi

if [ "$wipe" = true ] ; then
	cd $src
	rm -rf $build
	echo "Wiped build."
	exit
fi

cd $src$build;

if [ "$reconfigure" = true ] ; then
	echo "CMAKE Configure ..."; cmake -G "MSYS Makefiles" $rsrc;
	#echo "CMAKE Configure ..."; cmake $rsrc;
fi

if [ "$cplus" = true ] ; then
	echo "make ..."; make -j8;
	echo "run Tests"; ./Tests;
fi

export rstudio_out_img=../$output/img

if [ "$rlang" = true ] ; then
	echo "Generating PDF ...."
	cd $src$rstudio
	Rscript -e 'pagedown::chrome_print('"'article.rmd'"')' > log_r.txt
	cd $src
	mv $rstudio/article.html $output/article.html
	mv $rstudio/article.pdf $output/article.pdf
fi
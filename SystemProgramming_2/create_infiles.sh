#!/bin/bash

# produce random directory name (alpharethmetic 8)
random_name () {
    local counter=1
    local length_of_name=$(( ( RANDOM % 8 )  + 1 ))
    while [ $counter -le $1 ]
    do
        temp_name_directory+="$(cat /dev/urandom | tr -cd 'a-f0-9' | head -c $length_of_name)/" # source: https://stackoverflow.com/questions/2793812/generate-a-random-filename-in-unix-shell
        while [ -e $temp_name_directory ]
        do
            temp_name_directory+="$(cat /dev/urandom | tr -cd 'a-f0-9' | head -c $length_of_name)/"
        done
        dir_array[1,$index]="$directory/$temp_name_directory"
        index=$(($index+1))

        length_of_name=$(( ( RANDOM % 8 )  + 1 ))
        ((counter++))
    done
}

# produce random file name (alpharethmetic 8)
randon_filename () {
    local length_of_name=$(( ( RANDOM % 8 )  + 1 ))
    temp_name_file="$(cat /dev/urandom | tr -cd 'a-f0-9' | head -c $length_of_name)" # source: https://stackoverflow.com/questions/2793812/generate-a-random-filename-in-unix-shell
    while [ -e $temp_name_file ]
    do
        temp_name_file="$(cat /dev/urandom | tr -cd 'a-f0-9' | head -c $length_of_name) "
    done
}

# produce random file content (alpharethmetic 1kb - 128 kb)
random_file_content () {
    local length_of_content=$(( ( RANDOM % 128000 )  + 1000 ))
    temp_content="$(cat /dev/urandom | tr -cd 'a-f0-9' | head -c $length_of_content)" # source: https://stackoverflow.com/questions/2793812/generate-a-random-filename-in-unix-shell
}

if [ $# -ne 4 ]
  then
    echo "Error: Script create_infiles: wrong number of arguments! $#"
    exit 0
else
    directory=$1
    number_of_files=$(($2+0))
    number_of_directories=$(($3+0))
    levels=$(($4+0))
fi

if [ $levels -le 0 ] && [ $number_of_files -le 0 ] && [ $number_of_directories -le 0 ] # must levels >= 0 && number of files >=0 && number of directories >=0
    then
        echo "Error: Script create_infiles: Cannot have negative arguments!"
        exit 0
fi

if [ $levels -gt $number_of_directories ] # must levels <= number of directories
    then
        echo "Error: Script create_infiles: You give $levels levels and $number_of_directories directories. Cannot have more levels than directories!"
        exit 0
fi

declare -A dir_array    # array with paths for directories
num_rows=1
num_columns=$(($number_of_directories+1))
index=1

if [ -e $directory ] # make directory (if it doesn't exist) and cd
    then
        echo "Already exists $directory"
    else
        mkdir $directory
fi
cd $directory

dir_array[1,$index]=$directory
index=$(($index+1))

# mkdir
number_of_mkdir=$((number_of_directories / levels))
number_of_modulo_dirs=$((number_of_directories % levels))
counter=1
while [ $counter -le $number_of_mkdir ]
do
    random_name "$levels"
    mkdir -p $temp_name_directory
    unset temp_name_directory
    ((counter++))
done
if [ $number_of_modulo_dirs -ne 0 ]
    then
        random_name "$number_of_modulo_dirs"
        mkdir -p $temp_name_directory
        unset temp_name_directory
fi

cd ./..
myroot="$(pwd)"

declare -A file_array   # array with paths for files
num_rows=1
columns=$(($number_of_files))
index_of_file_array=1

#create file
number_of_touch=$((number_of_files / (number_of_directories+1)))
for ((j=1;j<=num_columns;j++)) do
    cd ${dir_array[1,$j]}
    for ((i=1;i<=number_of_touch;i++)) do
        randon_filename
        random_file_content
        touch $temp_name_file
        echo $temp_content > $temp_name_file
        file_array[1,$index_of_file_array]="$(pwd)/$temp_name_file"
        ((index_of_file_array++))
    done
    cd $myroot
done
number_of_modulo_touch=$((number_of_files % (number_of_directories+1)))
for ((j=1;j<=number_of_modulo_touch;j++)) do
    cd ${dir_array[1,$j]}
    randon_filename
    random_file_content
    touch $temp_name_file
    echo $temp_content > $temp_name_file
    file_array[1,$index_of_file_array]="$(pwd)/$temp_name_file"
    ((index_of_file_array++))
    cd $myroot 
done
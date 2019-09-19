#!/bin/bash

if [ -p /dev/stdin ]; then
    f_client=0  # flags
    f_all=0
    f_dead=0
    f_min=0
    f_max=0
    f_byte_send=0
    f_file_send=0
    f_byte_receive=0
    f_file_receive=0
    max=0
    min=123456789

    declare -A myclients   # array with clients
    num_rows=1
    columns=50
    index=1
    while IFS= read -r var; do  # read from logs
        if [ $f_client -eq 1 ]; then
            f_client=0
            myclients[1,$index]=$var
            index=$(($index+1))  
        fi
        if [ $f_all -eq 1 ]; then
            f_all=0
            all=$(($all+$var))
        fi
        if [ $f_max -eq 1 ]; then
            f_max=0
            if [ $var -gt $max ]; then
                max=$var
            fi
        fi
        if [ $f_min -eq 1 ]; then
            f_min=0
            if [ $var -lt $min ]; then
                min=$var
            fi
        fi
        if [ $f_byte_send -eq 1 ]; then
            f_byte_send=0
            send_bytes=$(($send_bytes+$var))
        fi
        if [ $f_file_send -eq 1 ]; then
            f_file_send=0
            send_files=$(($send_files+$var))
        fi
        if [ $f_byte_receive -eq 1 ]; then
            f_byte_receive=0
            receive_bytes=$(($receive_bytes+$var))
        fi
        if [ $f_file_receive -eq 1 ]; then
            f_file_receive=0
            receive_files=$(($receive_files+$var))
        fi
        if [ $f_dead -eq 1 ]; then
            f_dead=0
            dead=$(($dead+$var))
        fi
  
        if [ $var == "client" ]; then
            f_client=1
        fi
        if [ $var == "max" ]; then
            f_max=1
        fi
        if [ $var == "min" ]; then
            f_min=1
        fi
        if [ $var == "bs" ]; then
            f_byte_send=1
        fi
        if [ $var == "fs" ]; then
            f_file_send=1
        fi
        if [ $var == "br" ]; then
            f_byte_receive=1
        fi
        if [ $var == "fr" ]; then
            f_file_receive=1
        fi      
        if [ $var == "allclients" ]; then
            f_all=1
        fi 
        if [ $var == "dead" ]; then
            f_dead=1
        fi
    done

    index=$(($index-1))

    echo "Number of clients is"
    echo $all
    echo "Number of dead clients is"
    echo $dead
    echo "Clients are"
    for ((j=1;j<=index;j++)) do
        echo ${myclients[1,$j]}
    done
    echo "Min is"
    echo $min
    echo "Max is"
    echo $max
    echo "Number of bytes (send) is"
    echo $send_bytes
    echo "Number of files (send) is"
    echo $send_files
    echo "Number of bytes (receive) is"
    echo $receive_bytes
    echo "Number of file (receive) is"
    echo $receive_files
fi
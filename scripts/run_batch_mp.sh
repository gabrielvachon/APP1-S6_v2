#!/usr/bin/env bash
# Assumes this is run in the build/ folder.
Help()
{
    echo "----- HELP - asset_conv -----"
    echo ""
    echo "-p : process number"
    echo "-t : thread number"
    echo "-h : help"
    echo ""
    echo "-----------------------------"
}
Process=1
Thread=1
while getopts ":h:p:t:" option; do
    case $option in
        h) # display Help
            Help
            exit
            ;;
        p) # process number
            Process=$OPTARG
            ;;
        t) # thread number
            Thread=$OPTARG
            ;;
        \?)
            echo "Incorrect parameter";;
   esac
done
echo "Running $Process process and $Thread thread..."
sleep 2
mkdir -p output
time ../scripts/gen_tasks.py ../data ./output 480 | ../scripts/multi_proc.py $Process $Thread
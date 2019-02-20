getVersionAdb () {
    OUTPUT=$(adb version)
    arr=($OUTPUT)
    for key in  "${!arr[@]}";
    do 
        if [ ${arr[$key]} == 'version' ]
        then
            note='// Version'
            echo $note > server.txt
            nextKey=`expr $key + 1`
            echo ${arr[$nextKey]} >> server.txt
        fi

        if [ ${arr[$key]} == 'Version' ]
        then
            note='// Build'
            echo $note >> server.txt
            nextKey=`expr $key + 1`
            echo ${arr[$nextKey]} >> server.txt
        fi
    done
}
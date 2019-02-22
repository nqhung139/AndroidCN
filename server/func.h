getVersionAdb () {
    ADB_VERSION_INFO=$(adb version)
    IFS=$'\n'
    arrLine=($ADB_VERSION_INFO)
    for i in ${!arrLine[@]}
    do
        line=${arrLine[i]}
        IFS=' '
        arrChar=($line)
        length=$(expr ${#arrChar[@]} - 1)
        if [ $i == 0 ]
        then
            echo -e "\n// adb version" > server.txt
            echo ${arrChar[length]} >> server.txt
        fi

        if [ $i == 1 ]
        then
            echo -e "\n// adb build" >> server.txt
            echo ${arrChar[length]} >> server.txt
        fi
    done
}

getIPAddress () {
    note='\n// IP'
    echo -e $note >> server.txt
    echo $(ipconfig getifaddr en1) >> server.txt
}

deviceIndex=0
getListDevices () {
    echo -e "\n// serial number|model|transport_id" >> server.txt

    deviceIndex=0
    OUTPUT=$(adb devices -l)
    IFS=$'\n'
    for deviceInfo in ${OUTPUT[@]}
    do  
        IFS=$'\n'
        value=$(checkString $deviceInfo "attached")
        if [ $value != 1 ]
        then
            printDeviceInfo $deviceInfo
        fi
    done
}

checkString () {
    if [ `echo $1 | grep -c $2 ` -gt 0 ]
    then
        echo 1
    else
        echo 0;
    fi
}

printDeviceInfo () {
    echo -e "======= Device $deviceIndex =======" >> server.txt
    deviceIndex=$(expr $deviceIndex + 1)
    INPUT=$1
    IFS=' :'
    arr=($INPUT)
    deviceInfo=""
    for key in ${!arr[@]}
    do
        value=${arr[$key]}
        nextValue=${arr[$(expr $key + 1)]}


        if [ $key == 0 ]
        then
            deviceInfo+=$value
        fi
        if [ $value == "model" ]
        then 
            deviceInfo+="|$nextValue"
        fi
        if [ $value == "transport_id" ]
        then
            deviceInfo+="|$nextValue"
        fi
    done
    echo -e "$deviceInfo\n" >> server.txt
}

test() {
    for LINE in ${ARR[@]}
    do
        for i in ${LINE[@]}
        do
            echo $i
            echo '-------'
        done
    done
}
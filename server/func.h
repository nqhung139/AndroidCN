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
            printPortUsed $value
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

getPortOpenForDebug () {
    portStatus=$(checkPortUsed 27)

}

printPortUsed () {
    listPort=$(cat "listPort.txt")
    IFS=$'\n'
    arrLinePortInfo=($listPort)
    inited=0
    length=${#arrLinePortInfo[@]}
    for key in ${!arrLinePortInfo[@]}
    do
        IFS=$'\n'
        if [ $key != 0 ]
        then
            portInfo=${arrLinePortInfo[$key]}
            IFS='|'
            arrPortInfo=($portInfo)
            if [ ${arrPortInfo[1]} == $1 ]
            then
                inited=1
            fi
        fi
    done
    if [ $inited == 0 ]
    then
        range=$(expr $length \* 2)
        scrcpyPort=$(expr 27183 + $range)
        debugPort=$(expr $scrcpyPort + 1)
        adb reverse tcp:$debugPort tcp:$debugPort
        echo "$length|$1|$scrcpyPort|$debugPort" >> listPort.txt
    fi
}

checkPortUsed () {
    if lsof -Pi :$1 -sTCP:LISTEN -t >/dev/null
    then
        echo "running"
    else
        echo "not running"
    fi  
}
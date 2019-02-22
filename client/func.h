checkServer() {
    serverInfo=$(cat '../server/server.txt')
    IFS=$'\n'
    arrLine=($serverInfo)

    deviceInfoIndex=-1
    for key in ${!arrLine[@]}
    do
        value=${arrLine[$key]}
        nextValue=${arrLine[$(expr $key + 1)]}
        if [ $value == "// IP" ]
        then
            checkIP $nextValue
        fi

        if [ $value == "// serial number|model|transport_id" ]
        then
            echo "List devices connected with server is : \n"
            deviceInfoIndex=$key
        fi

        if [ $deviceInfoIndex != "-1" ]
        then
            echo $value
        fi
    done

    echo "\n"
}

checkBrew() {
    checker=$(checkCommandInstalled brew)
    
    if [ "$checker" == "do not install" ]
    then 
        echo "Please install homebrew (Y/n)"
        read command
        checkYes=$(lowercase "$command")
        if [ "$checkYes" == "y" ] || [ "$checkYes" == "yes" ] || [ "$checkYes" == "" ]
        then
            /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
        else 
            exit 0
        fi
    fi
}

checkADB () {
    checker=$(checkCommandInstalled adb)
    
    if [ "$checker" == "do not install" ]
    then 
        installADB
    else
        checkADBVersion
    fi
}

checkADBVersion () {
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
            adbVersionServer=$(getADBVersionFromServer)
            adbVersion=${arrChar[length]}
            if [ $adbVersion != $adbVersionServer ]
            then
                installADB
            fi
        fi
    done
}

getADBVersionFromServer () {
    serverInfo=$(cat '../server/server.txt')
    IFS=$'\n'
    arrLine=($serverInfo)

    for key in ${!arrLine[@]}
    do
        value=${arrLine[$key]}
        nextValue=${arrLine[$(expr $key + 1)]}
        if [ $value == "// adb version" ]
        then
            echo $nextValue
        fi
    done
}

installADB () {
    echo "Please install adb (Y/n)"
    read command
    checkYes=$(lowercase "$command")
    if [ "$checkYes" == "y" ] || [ "$checkYes" == "yes" ] || [ "$checkYes" == "" ]
    then
        brew cask install android-platform-tools
    else 
        exit 0
    fi
}

checkSSHPass() {
    checker=$(checkCommandInstalled sshpass)
    
    if [ "$checker" == "do not install" ]
    then 
        echo "Please install sshpass (Y/n)"
        read command
        checkYes=$(lowercase "$command")
        if [ "$checkYes" == "y" ] || [ "$checkYes" == "yes" ] || [ "$checkYes" == "" ]
        then
            brew install https://raw.githubusercontent.com/kadwanev/bigboybrew/master/Library/Formula/sshpass.rb
        else 
            exit 0
        fi
    fi
}

checkScrCpy() {
    checker=$(checkCommandInstalled scrcpy)
    
    if [ "$checker" == "do not install" ]
    then 
        echo "Please install scrcpy (Y/n)"
        read command
        checkYes=$(lowercase "$command")
        if [ "$checkYes" == "y" ] || [ "$checkYes" == "yes" ] || [ "$checkYes" == "" ]
        then
            brew install scrcpy
        else 
            exit 0
        fi
    fi
}

connectToDevice () {
    echo '\n Enter serial "transport_id" connect : '
    read transportId
    serialNumber=$(getSerialFromTransID $transportId)

    forwardADBPort
    // forwardScrcpyPort &
}

getSerialFromTransID() {
    serverInfo=$(cat '../server/server.txt')
    IFS=$'\n'
    arrLine=($serverInfo)

    for key in ${!arrLine[@]}
    do  
        IFS=$'\n'
        value=${arrLine[$key]}
        isDeviceInfoLine=$(checkString $value "|")
        if [ $isDeviceInfoLine == 1 ]
        then
            IFS='|'
            arrValue=($value)
            transID=${arrValue[2]}
            if [ $1 == $transID ]
            then
                echo ${arrValue[0]}
            fi
        fi
    done
}

forwardADBPort () {
    portStatus=$(checkPortUsed 5037)
    if [ "$portStatus" == "running" ]
    then
        adb kill-server
        killall adb
    fi

    serverInfo=$(cat '../server/server.txt')
    IFS=$'\n'
    arrLine=($serverInfo)

    for key in ${!arrLine[@]}
    do
        value=${arrLine[$key]}
        nextValue=${arrLine[$(expr $key + 1)]}
        if [ $value == "// IP" ]
        then
            sshpass -pAbc123456 ssh -CN -L5037:localhost:5037 -R27183:localhost:27183 "Vu Hoang Ha"@$nextValue
            echo "pasword connect to server \"Vu Hoang Ha\" is wrong!!"
        fi
    done
}

forwardScrcpyPort (){
    echo "dech"
    portStatus=$(checkPortUsed 27183)
    if [ "$portStatus" == "running" ]
    then
        echo "port 27183 is used please check port use"
        exit 0
    fi

    serverInfo=$(cat '../server/server.txt')
    IFS=$'\n'
    arrLine=($serverInfo)

    for key in ${!arrLine[@]}
    do
        value=${arrLine[$key]}
        nextValue=${arrLine[$(expr $key + 1)]}
        if [ $value == "// IP" ]
        then
            sshpass -pAbc123456 ssh -CN -R27183:localhost:27183 "Vu Hoang Ha"@$nextValue
            echo "pasword connect to server \"Vu Hoang Ha\" is wrong!!"
        fi
    done
}

checkPortUsed () {
    if lsof -Pi :$1 -sTCP:LISTEN -t >/dev/null
    then
        echo "running"
    else
        echo "not running"
    fi  
}

lowercase () {
    echo $1 | tr '[:upper:]' '[:lower:]'
}

checkCommandInstalled () {
    if hash -- $1 2> /dev/null
    then 
        echo "installed"
    else 
        echo "do not install"
    fi
}

checkIP(){
    while ! ping -c1 $1 &>/dev/null
    do 
        echo "Ping Fail - `date`\n"
        exit 0
    done
    echo "Host Found - `date`\n"
}

checkString () {
    if [ `echo $1 | grep -c $2 ` -gt 0 ]
    then
        echo 1
    else
        echo 0;
    fi
}
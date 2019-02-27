url="http://10.0.4.35/AndroidCN/server"

test () {
    serverInfo<(curl -s http://10.0.4.35/AndroidCN/client/index.sh)
    echo $serverInfo
}

# checkServer
# checkBrew
# checkADB
# checkScrCpy
# checkSSHPass

# connectToDevice

test


url="http://10.0.4.35/AndroidCN/server"

test () {
    serverInfo=$(cat "$url/server.txt")
    echo $serverInfo
}

# checkServer
# checkBrew
# checkADB
# checkScrCpy
# checkSSHPass

# connectToDevice

test


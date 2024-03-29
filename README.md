# ft_irc
Without a doubt, this is one of the most amazing projects we have done during our 42 journey. This time we had to implement our own **IRC server.** Thanks to this, we have gained a lot of experience in both **socket programming** and **parser development.** We have tried to make it as commercial as possible, always adjusting to the specifications of the subject. If you encounter any problems using it, please feel free to contact us to resolve it. And now, it's time to have fun playing with the **nameless server**... 🤯

Clone the repository:

    git clone https://github.com/pgomez-a/ft_irc.git ; cd ft_irc
    
Compile the program:

    make
    
Run the server on a port of your choice with a password:

    #./ircserv <port> <password>
    ./ircserv 8080 server_password

### nameless IRC server
We wanted to create as pure an IRC server as possible. That is why we have followed [**RFC 1459**](https://datatracker.ietf.org/doc/html/rfc1459). This reference contains all the important concepts that you should know about the **IRC protocol: communication protocols, transport protocols, commands, operators, channels, etc.** Implementing a full IRC server is a very complicated and time consuming task. Because this project is for educational purposes only, we have only implemented the most basic things that make an IRC server functional.<br>
The specifications of the **nameless server** are as follows:
- **IPv4:** Internet Protocol to connect devices to the web.
- **TCP:** Transmission Control Protocol to allow the server to exchange messages with clients.
- **\<ip_address\>:** localhost (127.0.0.1). The network where the server is hosted.
- **\<port\>:** the port from which the server accepts connections from clients.
- **\<password\>:** each nameless server has a password that is required to connect to it as a client.

When the server is run, it is hosted on localhost by default. The localhost domain is associated with the IP address **127.0.0.1**. But how can we connect to our server? The thing is that our host (our computer) is receiving data packets all the time. These packets have been built using different protocols: **HTTP**, **ARP**, **FTP**, etc. Before handling these packets, the host must have a way to differentiate between them. To achieve this, we use ports. The reason our host may treat HTTP packets differently than it treats FTP packets is because HTTP packets arrive on port 80 and FTP packets arrive on port 21. **So, to allow our host to handle IRC packets, we have to connect our server to a port on the host.** When a client connects to this port, all messages it sends will be treated as IRC packets. This is the reason why we have to assign a port to our server.

### nameless IRC commands
All of the commands below have been implemented for client-server communications. Check each command reply in the [**RFC 1459**](https://datatracker.ietf.org/doc/html/rfc1459#section-6).

***

- [**PASS:**](https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.1) used **to set a 'connection password'**. The password must be set before any attempt to register the connection is made. The password supplied must match the server password. It is possible to send multiple PASS commands before registering but only the last one sent is used for verification and it may not be changed once registered.<br><br>
Command: **PASS \<password\>**<br>
Numeric Replies: **ERR_NEEDMOREPARAMS** &emsp; **ERR_ALREADYREGISTRED**

***

- [**NICK:**](https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.2) used **to give user a nickname or change the previous one**. If a NICK message arrives at a server which already knows about an identical nickname for another client, a nickname collision occurs. As a result of a nickname collision, all instances of the nickname are removed from the server's database, and a KILL command is issued to remove the nickname from all other server's database. If the NICK message causing the collision was a nickname change, then the original (old) nick must be removed as well.<br>
If the server recieves an identical NICK from a client which is directly connected, it may issue an ERR_NICKCOLLISION to the local client, drop the NICK command, and not generate any kills.<br><br>
Command: **NICK \<nickname\>**<br>
Numeric Replies: **ERR_NONICKNAMEGIVEN** &emsp; **ERR_NICKNAMEINUSE** &emsp; **ERR_ERRONEUSNICKNAME** &emsp; **ERR_NICKCOLLISION**

***

- [**USER:**](https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.3) used at the beginning of connection to specify the username, hostname, servername and realname of a new server. **Only after both USER and NICK have been received from a client does a user become registered.** It must be noted that realname parameter must be the last parameter, because it may contain space characters and must be prefixed with a colon (':') to make sure this is recognised as such.<br><br>
Command: **USER \<username\> \<hostname\> \<servername\> \<realname\>**<br>
Numeric Replies: **ERR_NEEDMOREPARAMS** &emsp; **ERR_ALREADYREGISTRED**

***

- [**OPER:**](https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.5) used by a normal user **to obtain operator privileges**. The combination of \<user\> and \<password\> are required to gain Operator privileges.<br><br>
Command: **OPER \<user\> \<password\>**<br>
Numeric Replies: **ERR_NEEDMOREPARAMS** &emsp; **ERR_NOOPERHOST** &emsp; **RPL_YOUREOPER** &emsp; **ERR_PASSWDMISMATCH**

***

- [**QUIT:**](https://datatracker.ietf.org/doc/html/rfc1459#section-4.1.6) a client session is ended with a quit message. **The server must close the connection to a client which sends a QUIT message.** If a "Quit Message" is given, this will be sent instead of the default message, the nickname. If, for some other reason, a client connection is closed without  the client  issuing  a  QUIT  command  (e.g.  client  dies and EOF occurs on socket), the server is required to fill in the quit  message  with some sort  of  message  reflecting the nature of the event which caused it to happen.<br><br>
Command: **QUIT [\<Quit message\>]**<br>
Numeric Replies: **None**

***

- [**JOIN:**](https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.1) used by client **to start listening a specific channel**. The conditions which affect this are as follows:<br>
**1 -** the user must be invited if the channel is invite-only;<br>
**2 -** the user's nick/username/hostname must not match any active bans;<br>
**3 -** the correct key (password) must be given if it is set.<br>
Once a user has joined a channel, they receive notice about all commands their server receives which affect the channel.  This includes PART, QUIT and of course PRIVMSG/NOTICE. If a JOIN is successful, the user is then sent the channel's topic (using RPL_TOPIC) and the list of users who are on the channel (using RPL_NAMREPLY), which must include the user joining.<br><br>
Command: **JOIN \<channel\>{,\<channel\>} [\<key\>{,\<key\>}]**<br>
Numeric Replies: **ERR_NEEDMOREPARAMS** &emsp; **ERR_INVITEONLYCHAN** &emsp; **ERR_CHANNELISFULL** &emsp; **ERR_NOSUCHCHANNEL** &emsp; **RPL_TOPIC** &emsp; **ERR_BANNEDFROMCHAN** &emsp; **ERR_BADCHANNELKEY** &emsp; **ERR_BADCHANMASK** &emsp; **ERR_TOOMANYCHANNELS**

***

- [**PART:**](https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.2) causes the client sending the message to be **removed from the list of active users** for all given channels listed in the parameter string.<br><br>
Command: **PART \<channel\>{,\<channel\>}**<br>
Numeric Replies: **ERR_NEEDMOREPARAMS** &emsp; **ERR_NOTONCHANNEL** &emsp; **ERR_NOSUCHCHANNEL**

***

- [**PRIVMSG:**](https://datatracker.ietf.org/doc/html/rfc1459#section-4.4.1) used **to send private messages between users**. \<receiver\> is the nickname of the receiver of the message. \<receiver\> can also be a list of names or channels separated with commas.<br><br>
Command: **PRIVMSG \<receiver\>{,\<receiver\>} \<text to be sent\>**<br>
Numeric Replies: **ERR_NORECIPIENT** &emsp; **ERR_CANNOTSENDTOCHAN** &emsp; **ERR_NOSUCHNICK** &emsp; **ERR_NOTEXTTOSEND** &emsp;

***

- [**NOTICE:**](https://datatracker.ietf.org/doc/html/rfc1459#section-4.4.2) The NOTICE message is used similarly to PRIVMSG. The difference between NOTICE and PRIVMSG is that **automatic replies must never be sent** in response to a NOTICE message.<br><br>
Command: **NOTICE \<nickname\> \<text\>**

***

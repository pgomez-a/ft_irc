# ft_irc

ToDo List:
- Implementar parser
- Implementar comandos
- Gestionar canales
- Gestionar operadores
- (Comunicación segura)
- (Implementar bot)

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
Los comandos implementados en el servidor nameless son los siguientes:
- PASS:
- NICK:
- USER:
- OPER:
- QUIT:
- JOIN:

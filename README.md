# TCP Connection in C
This project demonstrates TCP connections over IPv6 using C. It sends an echo, replies with the echo.

# Requirements
- x64 Native Tools Command Prompt

# Setup
Compile using Windows cl.exe (From VS Code Build Tools)
```bash
cl WSEchoClientv6.c ws2_32.lib
```

# Usage
### Part 1: Setting up Server
Run the server in ./bin/WSEchoServerv6.exe
```bash
WSEchoServerv6 5000
```

Verify it's running in windows by checking in powershell or cmd:
```bash
netstat -ano 
```
Look for: TCP    [::1]:5000          [::]:0              LISTENING       1234

### Part 2: Setting up Client
Then, Run the Connection and pass an argument message for the echo.
```bash
WSEchoClientv6 ::1 5000 "Echo Message"
```


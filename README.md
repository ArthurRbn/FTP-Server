# FTP-Server
FTP server (RFC 959 compliant) that supports passive and active mode.

Supported commands :
- ``USER`` - Enter username
- ``PASS`` - Enter password
- ``QUIT`` - Close connection
- ``HELP`` - Get supported commands
- ``CWD`` &nbsp; - Change working directory
- ``CDUP`` - Go to parent directory
- ``PWD`` &nbsp; - Get working directory path
- ``LIST`` - List files in working directory
- ``RETR`` - Retrieve a file
- ``STOR`` - Store a file
- ``DELE`` - Delete a file
- ``PASV`` - Enter passive mode
- ``PORT`` - Enter active mode
- ``NOOP`` - No operation

To test the server clone the repository and build the binary with ``make``.  
Launch the server : ``./myftp [port] [path]``  
- ``port`` is the listening port
- ``path`` is the path to default working directory when a user logs in. Can be relative or absolute.

Then connect with a ftp client or use the script test_ftp.sh to test user authentication process.  
Test script : ``./test_ftp.sh [ip] [port]``
- ``ip`` is the host IP adress
- ``port`` is the host listening port

Tests passed : 80%

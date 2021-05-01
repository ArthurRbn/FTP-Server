# FTP-Server
FTP server (RFC 959 compliant) that supports passive and active mode.

Supported commands :
- ``USER`` - Enter username
- ``PASS`` - Enter password
- ``QUIT`` - Close connection
- ``HELP`` - Get supported commands
- ``CWD``  - Change working directory
- ``CDUP`` - Go to parent directory
- ``PWD``  - Get working directory path
- ``LIST`` - List files in working directory
- ``RETR`` - Retrieve a file
- ``STOR`` - Store a file
- ``DELE`` - Delete a file
- ``PASV`` - Enter passive mode
- ``PORT`` - Enter active mode
- ``NOOP`` - No operation

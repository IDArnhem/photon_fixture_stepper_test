# Over the Air updates

If you are on Platformio, remember that to use OTA you need to set the upload port and the protocol properly in the `platformio.ini` file. Like:

```
; for OTA stuff you have to update the upload_port
upload_protocol = espota
upload_port = IP_ADDRESS_HERE or mDNS_NAME.local
```

And remember that you can get serial debug output via telnet using the `TelnetStream` library.

```
lib_deps = 
    TelnetStream
```


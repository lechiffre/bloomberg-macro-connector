#!/usr/bin/env -S uv run --script
# /// script
# dependencies = ["blpapi", "python-dotenv"]
# ///

import os
import blpapi
import time
from dotenv import load_dotenv
load_dotenv()

def connect_to_bpipe():
    print("Initializing connection to Bloomberg B-PIPE...")
    # Session options
    sessionOptions = blpapi.SessionOptions()
    
    # Your provided Bloomberg B-PIPE ZFP hosts
    host_primary = os.environ["PRIMARY_HOST"]
    host_secondary = os.environ["SECONDARY_HOST"]
    port = int(os.environ["PORT"])
    
    print(f"Setting up connection to primary host: {host_primary}")
    sessionOptions.setServerAddress(host_primary, port, 0)  # Primary host (UK)
    print(f"Setting up connection to secondary host: {host_secondary}")
    sessionOptions.setServerAddress(host_secondary, port, 0)  # Secondary host (Germany)
    
    # ZFP configuration with TLS - using your specific certificate files
    client_cert_path = os.environ["CLIENT_CERTIFICATE"]
    client_cert_password = os.environ["PASSWORD"]
    root_cert_path = os.environ["ROOT_CERTIFICATE"]
    
    print("Loading certificate files...")
    tlsOptions = blpapi.TlsOptions.createFromFiles(
        client_cert_path,
        client_cert_password,
        root_cert_path
    )
    sessionOptions.setTlsOptions(tlsOptions)
    
    # Additional common configurations
    sessionOptions.setAutoRestartOnDisconnection(True)
    sessionOptions.setNumStartAttempts(3)
    
    # Create and start session
    print("Attempting to start session...")
    session = blpapi.Session(sessionOptions)
    if not session.start():
        print("Failed to start session")
        return None
    
    print("Successfully connected to B-PIPE")
    return session

if __name__ == "__main__":
    connect_to_bpipe()

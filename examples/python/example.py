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
    sessionOptions = blpapi.SessionOptions()
    host_primary = os.environ["PRIMARY_HOST"]
    host_secondary = os.environ["SECONDARY_HOST"]
    port = int(os.environ["PORT"])
    sessionOptions.setServerAddress(host_primary, port, 0)
    sessionOptions.setServerAddress(host_secondary, port, 0)
    client_cert_path = os.environ["CLIENT_CERTIFICATE"]
    client_cert_password = os.environ["PASSWORD"]
    root_cert_path = os.environ["ROOT_CERTIFICATE"]
    tlsOptions = blpapi.TlsOptions.createFromFiles(
        client_cert_path,
        client_cert_password,
        root_cert_path
    )
    sessionOptions.setTlsOptions(tlsOptions)
    sessionOptions.setAutoRestartOnDisconnection(True)
    sessionOptions.setNumStartAttempts(3)
    session = blpapi.Session(sessionOptions)
    if not session.start():
        print("Failed to start session")
        return None
    print("Successfully connected to B-PIPE")
    return session

def geco_service(session):
    identify = session.createIdentity()
    session.openService("//blp/globaleco")
    service = session.getService("//blp/globalecoref")
    request = service.createRequest("GetTickersRequest")
    session.sendRequest(request, identity)
    

if __name__ == "__main__":
    session = connect_to_bpipe()
    geco_service(session)
    

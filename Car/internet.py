import os
import sys
from io import StringIO
import re

def get_ip_and_name():
    ip_addr = os.popen("ip addr show wlan0").read()
    wlan_name = os.popen("iwgetid | grep wlan0").read()

    match_ip = re.search(r'inet ([0-9\.]*)', ip_addr)
    match_wifi_name = re.search(r'ESSID:"(.*)"', wlan_name)
    message_ip = "Cannot connect to WiFi"
    message_name = ""

    if match_ip != None and match_wifi_name != None:
        message_ip = match_ip.group(1) 
        message_name = match_wifi_name.group(1)

    return (message_ip, message_name)
    
    

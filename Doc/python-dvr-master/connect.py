#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import sys
from dvrip import DVRIPCam
from time import sleep
import json

host_ip = "2.180.13.1"
if len(sys.argv) > 1:
    host_ip = str(sys.argv[1])

cam = DVRIPCam(host_ip, user="admin", password="1359")

if cam.login():
    print("Success! Connected to " + host_ip)
else:
    print("Failure. Could not connect.")

img = cam.snapshot()


# cam.setAlarm(alarm)
# cam.alarmStart()

# cam.get_encode_info()
# sleep(1)
# cam.get_camera_info()
# sleep(1)

# enc_info[0]['ExtraFormat']['Video']['FPS'] = 20
# cam.set_info("Simplify.Encode", enc_info)
# sleep(2)
# print(cam.get_info("Simplify.Encode"))
# cam.close()

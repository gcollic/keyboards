#!/bin/bash
sudo docker build -t temp . 
id=$(sudo docker create temp) 
sudo docker cp $id:/src/keyboard_firmware.hex . 
sudo docker rm -v $id
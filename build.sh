#!/bin/bash
docker build -t gcollic/grimoire60 .
docker run --name tempcontainer gcollic/grimoire60
docker cp "tempcontainer:/src/keyboard_firmware.hex" .
docker rm tempcontainer
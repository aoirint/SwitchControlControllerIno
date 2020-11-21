#!/bin/bash

VID="0x0f0d"
PID="0x0092"
arduino-cli compile "$PWD" \
  -b arduino:avr:leonardo \
  --libraries "$PWD/libs" \
  --build-properties "leonardo.build.vid=${VID}" \
  --build-properties "leonardo.build.pid=${PID}" \
  -v

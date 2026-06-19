# Binary Packet Inspector C

C-based binary packet inspector for embedded telemetry and mission diagnostics.

This project parses binary-like telemetry frames, validates packet headers, checks CRC values, classifies packet types, detects abnormal frames, and generates session-level diagnostic summaries.

## Packet Format

```text
SYNC,TYPE,LENGTH,PAYLOAD,CRC

Example:

AA55,01,04,120A33FF,9321
AA55,02,08,01020304AABBCCDD,7142
Goals
Inspect binary-style telemetry packet records
Validate sync word and payload length
Classify packet types
Detect malformed packets
Detect CRC mismatch
Generate diagnostic session report
Project Structure
src/
  main.c
  packet_reader.c
  packet_validator.c
  packet_classifier.c
  crc16.c
  hex_utils.c
  session_report.c

include/
  packet_reader.h
  packet_validator.h
  packet_classifier.h
  crc16.h
  hex_utils.h
  session_report.h

data/
  sample_packets.txt

docs/
  binary_protocol_spec.md
  roadmap.md

tests/
  parser_test_notes.md
Target Position

Embedded systems, telemetry diagnostics, binary protocol analysis, field operation tools, and mission software engineering.

Status

Early implementation.
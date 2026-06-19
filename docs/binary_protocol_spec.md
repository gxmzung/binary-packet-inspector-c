# Binary Protocol Spec

## Record Format

```text
SYNC,TYPE,LENGTH,PAYLOAD,CRC

Fields
Field	Example	Description
SYNC	AA55	Fixed packet synchronization word
TYPE	01	Packet type in hexadecimal
LENGTH	04	Payload byte length
PAYLOAD	120A33FF	Hex-encoded payload
CRC	9321	Expected CRC16 value
Packet Types
Type	Meaning
0x01	Health status
0x02	Mission data
0x03	Sensor frame
0x04	Warning event
0x05	Control acknowledgement
Validation Rules
Sync word must be AA55
Payload must be valid hexadecimal text
Payload byte length must match LENGTH
CRC must match calculated CRC16
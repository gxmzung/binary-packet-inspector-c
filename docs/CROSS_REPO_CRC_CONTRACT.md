# 다른 텔레메트리 저장소와의 CRC 연동 계약 (2026-11-16)

## 비교 대상

- 본 저장소: `binary-packet-inspector-c` PR #1
- 상대 저장소: `telemetry-packet-parser-c` PR #1
- 기준 검증: [Cross-repository CRC contract](https://github.com/gxmzung/telemetry-packet-parser-c/actions/workflows/cross-repo-contract.yml)

## 일치가 확인된 범위

- CRC-16/MODBUS (reflected polynomial 0xA001, init 0xFFFF, xorout 0).
- HEX 문자열을 바이트로 디코딩한 뒤 **payload 바이트만** CRC 계산.
- 본 저장소의 명시적 `calculate_crc16_decoded_payload` / `validate_packet_crc_decoded`와 상대 저장소의 `wire_crc16` / `wire_parse_record`는 검사한 공통 시편에서 일치.
- 상대 저장소의 시험 기록 형식은 `AA55,TYPE_HEX,LENGTH_DEC,PAYLOAD_HEX,CRC_HEX`이다.

## 호환성 경계 (중요)

- 본 저장소 `validate_packet_crc`는 레거시 ASCII HEX **문자** 대상 CRC이며, `wire_parse_record`와 호환되는 기본 모드가 아니다.
- 상대 파서는 payload 최대 128바이트 및 0바이트를 지원하지만, 본 저장소 `BinaryPacket.payload[128]`은 NUL 종료 문자열이므로 최대 **63바이트**를 표현할 수 있다. 또한 `validate_packet_crc_decoded`는 빈 문자열을 거부한다. 따라서 양쪽 `BinaryPacket` 검증기의 공통 지원 구간은 비어 있지 않은 1~63바이트이다.
- CRC 길이와 프레임 범위가 일치하더라도 실제 장비 프로토콜과의 호환성을 뜻하지 않는다.
- 기존 `data/sample_packets.txt`의 CRC 값이 양쪽 방식 중 어느 하나에 맞는 것으로 보장되지 않는다.
- 원본 데이터 형식, `main` 브랜치 및 레거시 함수 기본 동작은 변경하지 않았다.

## 후속 작업

1. 실제 사용 장비의 CRC 계산 범위(헤더 포함 여부 포함)를 문서 또는 패킷 캡처로 확인.
2. 표준 입력 규격을 확정한 뒤 길이·시퀀스·종단 처리 및 비정상 입력에 대한 검증 범위를 넓힐 것.
3. 기기 명세 없이 레거시 기본 모드를 자동으로 decoded 바이트 CRC로 전환하지 않을 것.

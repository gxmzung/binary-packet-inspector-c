# CRC 검증 호환성 — 2026-11-16

## 검토 배경
- 기존 `src/packet_reader.c`는 `%x`에 `int*`를 전달하는 서식-인수 타입 불일치가 있었다. 임시 `unsigned int`로 읽은 뒤 범위를 검사하여 기존 `int type` 필드는 그대로 유지한다.
- 기존 `calculate_crc16_hex_payload`는 16진수로 **표현된 ASCII 문자**를 CRC-16/MODBUS(초깃값 FFFF)로 처리한다. 기존 데모 데이터에 대한 의미를 임의로 바꾸지 않는다.
- 신규 `calculate_crc16_decoded_payload`는 문자열을 **디코딩한 바이트**를 대상으로 MODBUS CRC를 계산한다. 길이가 홀수이거나 16진수가 아니면 오류를 반환한다.

## 호환 정책
| API | 대상 | 사용 정책 |
|---|---|---|
| `validate_packet_crc` | HEX 문자열의 ASCII 문자 | 기존 기본 동작을 유지 |
| `validate_packet_crc_decoded` | HEX 디코딩된 실제 바이트 | 신규 명시적 선택 |
| `count_packet_errors` | 기존 방식 | 변경하지 않음 |

테스트 벡터: CRC-16/MODBUS 9바이트 ASCII `123456789` → `0x4B37`. decoded hex `313233343536373839` → 동일 결과.

## 검증 범위 및 한계
- 타입 불일치 수정 및 각 API의 기능 차이를 테스트와 GitHub Actions로 검증한다.
- 원본 저장소의 샘플 CRC 필드 값은 **실제 장비의 규격이 확인된 값이 아니며** 유효성을 가정하지 않는다.
- 레거시 모드와 decoded 모드가 다르다고 해서 어느 쪽이 장비 호환에 맞는지 결론 내리지 않는다.
- 장비 규격이 확정되기 전까지 자동 전환이나 데이터 마이그레이션 금지.

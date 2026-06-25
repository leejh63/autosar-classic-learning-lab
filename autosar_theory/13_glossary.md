# 13. 용어·약어 사전

AUTOSAR 는 약어가 압도적으로 많다. 자주 마주치는 것들을 모았다. (CP=Classic,
AP=Adaptive 맥락 표기)

## 플랫폼·표준 일반
- **AUTOSAR**: AUTomotive Open System ARchitecture. 차량 SW 표준.
- **CP / AP**: Classic Platform / Adaptive Platform.
- **Foundation**: CP·AP 공통 요소(프로토콜·메타모델·방법론 공통부).
- **SWS / TPS / TR / EXP / PRS / RS**: 명세 문서 종류 — Software Specification /
  Template Specification / Technical Report / Explanation / Protocol Specification /
  Requirements Specification.
- **Rxx-yy**: 릴리스 번호(연-월). 예 R24-11 = 2024년 11월.

## 구조·계층
- **BSW**: Basic Software. 표준 인프라 계층 전체.
- **MCAL**: Microcontroller Abstraction Layer. 레지스터 직결 최하위.
- **ECU Abstraction Layer**: 드라이버를 보드 레벨로 추상화.
- **CDD**: Complex Device Driver. 계층을 가로지르는 특수 드라이버.
- **RTE**: Runtime Environment. SWC↔BSW 배선(생성됨).
- **VFB**: Virtual Function Bus. 배치 전 가상 연결 추상.
- **ECU**: Electronic Control Unit.

## 컴포넌트·모델
- **SWC**: Software Component. 애플리케이션 재사용 단위.
- **Port / P-Port / R-Port**: 연결점 / 제공 / 요구.
- **S-R / C-S**: Sender-Receiver / Client-Server 포트 인터페이스.
- **Runnable**: SWC 내부 실행 단위(이벤트로 트리거).
- **IRV / PIM**: Inter-Runnable Variable / Per-Instance Memory.
- **Composition**: SWC 들을 묶는 상위 구성요소.
- **ARXML**: AUTOSAR XML. 모델 직렬화 파일.
- **ADT / IDT**: Application / Implementation Data Type.
- **ECU Extract**: 시스템 기술에서 한 ECU 부분만 추출한 것.
- **PC / LT / PB**: Pre-compile / Link-time / Post-build 설정 클래스.
- **OIL**: OSEK Implementation Language(OS 설정, 레거시).

## 통신
- **PDU**: Protocol Data Unit. (I-PDU=Com, N-PDU=TP, L-PDU=If/Driver)
- **SDU / PCI**: Service Data Unit(페이로드) / Protocol Control Info. PDU=SDU+PCI.
- **Signal**: 애플리케이션 논리 데이터(시그널).
- **Com / PduR / IpduM**: COM(시그널↔I-PDU) / PDU Router / I-PDU Multiplexer.
- **CanIf / LinIf / FrIf / EthIf**: 각 버스 인터페이스(드라이버 추상).
- **CanTp / LinTp / FrTp / J1939Tp**: 각 버스 Transport Protocol(세그먼테이션).
- **CanSM / LinSM / FrSM / EthSM**: 버스 State Manager.
- **CanNm / FrNm / J1939Nm / Nm**: Network Management.
- **ComM**: Communication Manager(통신 모드 중재).
- **TP**: Transport Protocol. ISO-TP=ISO 15765-2.
- **HTH / HRH**: Hardware Transmit/Receive Handle(CAN 메일박스 추상).
- **SecOC**: Secure Onboard Communication(통신 인증, MAC+freshness).
- **E2E**: End-to-End Protection(통신 무결성, CRC+counter).
- **Transformer (Xf)**: 직렬화/보호 변환기. SomeIpXf / E2EXf / ComXf.
- **SOME/IP**: Scalable service-Oriented MiddlewarE over IP. 서비스 지향 통신.
- **SD**: Service Discovery. **SoAd**: Socket Adaptor. **DoIP**: Diagnostics over IP.
- **DDS**: Data Distribution Service(데이터 중심 발행-구독).
- **J1939**: SAE 상용차 CAN 상위 프로토콜. **PGN/SPN**: Parameter Group/Suspect Param Number.

## 진단
- **UDS**: Unified Diagnostic Services(ISO 14229).
- **OBD**: On-Board Diagnostics(배출가스 법규 진단).
- **Dcm / Dem / Det / FiM / Dlt**: 진단 통신 관리 / 이벤트 관리 / 개발오류 추적 /
  기능 억제 / 로그·트레이스.
- **DTC**: Diagnostic Trouble Code(고장 코드).
- **DID**: Data Identifier(0x22/0x2E 의 데이터 식별자).
- **SID**: Service Identifier(UDS 서비스 ID).
- **Freeze Frame / Snapshot**: 고장 순간 환경 데이터.
- **Debouncing / Aging / Healing**: 오확정 방지 / 노화 / 치유(DTC 생애주기).
- **Operation Cycle**: 진단 평가 주기(예: 주행/점화 사이클).

## 메모리
- **NvM / MemIf**: NVRAM Manager / Memory Abstraction Interface.
- **Fee / Ea**: Flash EEPROM Emulation / EEPROM Abstraction.
- **Fls / Eep**: Flash / EEPROM Driver(MCAL).
- **RAM Block / NV Block / ROM Block**: RAM 미러 / 비휘발 저장본 / 공장 기본값.
- **Wear Leveling**: 플래시 마모 분산.

## 시스템·OS
- **OS**: AUTOSAR OS(OSEK 기반). **OSEK/VDX**: 그 뿌리 표준.
- **Task (Basic/Extended)**: 실행단위(블로킹 불가 / event 대기 가능).
- **BCC1/2, ECC1/2**: OS Conformance Class.
- **Resource / Priority Ceiling**: 상호배제 / 우선순위 천장 프로토콜.
- **Event / Alarm / Counter / Schedule Table**: 동기화·시간 기반 객체.
- **OS-Application**: 보호 도메인(파티션). trusted/non-trusted.
- **IOC**: Inter-OS-Application Communication(멀티코어 통신).
- **EcuM / BswM / SchM**: ECU State Manager / BSW Mode Manager / BSW Scheduler.
- **WdgM / WdgIf / Wdg**: Watchdog Manager / Interface / Driver.
- **Alive/Deadline/Logical Supervision**: WdgM 의 3가지 감시.
- **StbM / Tm / TSyn**: Synchronized Time-Base Manager / Time Service / Time Sync(CanTSyn·EthTSyn).
- **DET / DEV_ERROR_DETECT**: 개발 오류 추적 / 그 스위치.
- **MemMap**: 메모리 섹션 배치 추상화 헤더 패턴.

## 안전·보안
- **ISO 26262 / ASIL**: 기능안전 표준 / 안전무결성등급(QM~ASIL D).
- **FFI**: Freedom From Interference(간섭으로부터의 자유: 공간/시간/통신 격리).
- **HARA**: Hazard Analysis and Risk Assessment.
- **MPU / MMU**: Memory Protection/Management Unit.
- **Csm / CryIf / Crypto**: Crypto Service Manager / Interface / Driver.
- **KeyM / IdsM**: Key Manager / Intrusion Detection System Manager.
- **HSM / SHE**: Hardware Security Module / Secure Hardware Extension.
- **MAC / Freshness**: Message Authentication Code / 신선도값(재전송 방지).
- **MACsec**: L2(이더넷) 보안.
- **ISO/SAE 21434 / UN R155**: 차량 사이버보안 엔지니어링 / 규제(CSMS).

## Adaptive 전용
- **ara::***: AUTOSAR Runtime for Adaptive(ara::com/core/log ...).
- **EM / CM / SM / DM / PHM / UCM / IAM**: Execution / Communication / State /
  Diagnostic / Platform Health / Update&Config / Identity&Access Management.
- **Manifest**: AP 배포·실행·서비스 구성 기술(Execution/Service/Machine Manifest).
- **PSE51**: POSIX 프로파일(AP OS 인터페이스).
- **OTA**: Over-The-Air(무선 업데이트).

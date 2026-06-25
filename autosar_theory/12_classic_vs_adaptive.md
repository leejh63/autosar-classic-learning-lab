# 12. Classic vs Adaptive Platform

지금까지는 Classic Platform(CP)을 다뤘다. 이 장은 **Adaptive Platform(AP)** 을 깊이
소개하고 CP 와 대비한다. ADAS/자율주행·고성능 컴퓨팅으로 가면 AP 가 중요해지므로,
두 플랫폼의 철학 차이를 잡아두는 것이 목표다.

## 왜 Adaptive 가 생겼나

CP 는 "정적·결정론·저자원" 에 최적화돼 있다. 하지만 자율주행/인포테인먼트는 다른
요구를 낸다:
- **고성능 연산**: 카메라/라이다 인지, 센서퓨전, 경로계획 — MCU 가 아니라 MPU/SoC(멀티
  코어 + GPU/가속기)가 필요.
- **동적·유연성**: 앱을 동적으로 시작/중지, **무선 업데이트(OTA)** 로 기능 추가/교체.
- **복잡한 통신**: 고대역·서비스 지향(누가 어떤 서비스를 언제 제공/소비할지 동적).
- **풍부한 SW 생태계**: 표준 OS(POSIX)·C++·이더넷 위에서 빠르게 개발.

CP 의 정적 모델로는 이를 담기 어려워, AUTOSAR 는 별도 플랫폼 AP 를 만들었다. CP 를
대체하는 게 아니라 **역할이 다른 형제**다.

## Adaptive Platform 의 토대

### POSIX 운영체제
AP 는 자체 OS 가 아니라 **POSIX(PSE51 프로파일)** 호환 OS 위에서 돈다(리눅스, QNX
등). 프로세스/스레드/가상메모리 같은 일반 OS 기능을 활용. CP 의 OSEK 정적 OS 와 정반대.

### C++ 와 ara::* API
AP 애플리케이션은 **C++(14/17 기반)** 로 작성하고, **ara::**(AUTOSAR Runtime for
Adaptive) 네임스페이스의 표준 API 를 쓴다. 예: `ara::com`(통신), `ara::core`(기본
타입), `ara::log`(로깅). CP 의 C + 생성된 `Rte_*` 와 대비된다.

### 서비스 지향 아키텍처 (SOA)
CP 가 "시그널을 정적 매트릭스로 교환"한다면, AP 는 **서비스(메서드/이벤트/필드)를
동적으로 제공·발견·구독**한다.
- **ara::com**: 서비스 지향 통신 API. 바인딩(전송 프로토콜)으로 **SOME/IP** 와
  **DDS** 를 지원.
- **Service Discovery**: 서비스 제공자가 offer, 소비자가 find/subscribe. 런타임에
  연결이 형성(정적 배선이 아님).
- 메서드(C-S 호출)/이벤트(발행-구독)/필드(상태+notify)의 3요소로 서비스를 모델링.

> 차량 이더넷의 SOME/IP/SD(→`06`)가 CP 에도 있지만, AP 는 SOA 가 통신의 기본 모델이다.
> DDS 지원(데이터 중심 발행-구독)도 AP 의 특징(최근엔 CP 에도 DDS 가 추가됨).

## Functional Cluster — AP 의 "모듈"

CP 의 BSW 모듈에 대응하는 AP 의 구성 단위가 **Functional Cluster(기능 클러스터)** 다.
주요 클러스터:

| 클러스터 | 역할 | CP 대응(느슨히) |
|----------|------|------------------|
| Execution Management (EM) | 앱(프로세스) 시작/중지/상태, 실행 의존성 | EcuM/BswM 일부 |
| Communication Management (CM, ara::com) | 서비스 지향 통신(SOME/IP·DDS) | Com/통신스택 |
| State Management (SM) | 머신/기능 그룹 상태 | BswM/EcuM 모드 |
| Diagnostic Management (DM) | UDS 진단 | Dcm/Dem |
| Persistency | 키-값/파일 영속 저장 | NvM |
| Time Synchronization | 동기 시간 | StbM |
| Network Management | 네트워크 관리 | Nm/ComM |
| Cryptography | 암호 서비스 | Csm/CryIf |
| Identity & Access Mgmt (IAM) | 권한/접근 제어 | (CP 엔 약함) |
| Update & Config Mgmt (UCM) | 소프트웨어 업데이트(OTA) | (CP 엔 없음/부트로더) |
| Log & Trace | 로깅 | Dlt |
| Platform Health Mgmt (PHM) | 헬스/감시(supervision) | WdgM |
| Operating System Interface | POSIX OS 추상 | OS |

UCM(업데이트 관리)과 IAM(접근 제어)은 AP 에서 특히 부각되는, CP 엔 거의 없던 기능이다
(OTA·보안 접근의 시대 요구).

## Manifest — AP 의 설정

CP 가 ARXML + 생성으로 정적 구성을 한다면, AP 는 **Manifest**(역시 ARXML 기반)로
배포·실행 구성을 기술한다:
- **Execution Manifest**: 앱(프로세스)의 시작 조건/자원/의존성.
- **Service Instance Manifest**: 서비스 제공/소비 인스턴스와 바인딩.
- **Machine Manifest**: 머신(AP 인스턴스) 전역 설정.
Manifest 는 **배포 시점/런타임**에 해석될 수 있어 동적 구성을 가능케 한다(CP 의
컴파일-타임 고정과 대비).

## CP vs AP 비교표

| 축 | Classic Platform | Adaptive Platform |
|----|------------------|--------------------|
| 대상 HW | MCU(마이크로컨트롤러) | MPU/SoC(고성능, 멀티코어/가속기) |
| OS | OSEK 계열 정적 RTOS | POSIX(PSE51) — 리눅스/QNX 등 |
| 언어 | C | C++ (14/17) |
| 앱 단위 | SWC (정적) | Adaptive Application (프로세스, 동적) |
| 통신 | 시그널/PDU, 정적 매트릭스 | 서비스 지향(SOME/IP·DDS), 동적 SD |
| 미들웨어 | RTE (생성) | ara::* 런타임 |
| 구성 | ARXML + 코드 생성(컴파일타임) | Manifest(배포/런타임 해석) |
| 메모리 | 정적, MPU 파티셔닝 | 가상메모리(MMU), 프로세스 격리 |
| 업데이트 | 보통 전체 리프로그래밍 | UCM 으로 동적/부분 업데이트(OTA) |
| 결정론 | 강함(hard real-time) | 상대적으로 약함(soft/유연) |
| 전형 도메인 | 엔진/바디/섀시/도어 | ADAS/AD·인포테인먼트·게이트웨이 |

## 공존 — 실제 차량에서

현대 차량의 E/E 아키텍처는 **CP 와 AP 가 함께** 쓰인다:
- **존(zone)/도메인 컨트롤러·중앙 컴퓨터**: AP(고성능, SOA, OTA).
- **엣지 ECU·액추에이터 제어**: CP(실시간, 저자원).
- 둘은 이더넷(SOME/IP/DDS) 등으로 통신하며, 게이트웨이가 CAN/LIN(CP 도메인)과 이더넷
  (AP 도메인)을 잇는다. Foundation 이 두 플랫폼의 공통 토대(프로토콜/메타모델)를 제공.

## 무엇을 언제 (분석)

- **하드 실시간·저자원·단순 제어** → CP. 모터/밸브/도어처럼 결정론과 효율이 최우선.
- **고성능 연산·동적·OTA·SOA** → AP. 인지/계획/인포테인먼트처럼 유연성과 연산력이 필요.
- 임베디드 SW 커리어 관점: **CP 가 기반기**다. CP 의 계층/통신/진단/OS 를 확실히 잡은
  뒤 AP 로 확장하는 것이 자연스럽다(이 핸드북의 구성도 그 순서). AP 는 "C++/POSIX/SOA
  를 아는 사람"에게 진입장벽이 낮으므로, 일반 SW 역량과도 잘 이어진다.

## 원문에서 확인할 것
- *AUTOSAR AP — Explanation of Adaptive Platform Design* (EXP) — AP 전체 개관.
- *Specification of Communication Management* (SWS CM, ara::com), *Execution Management* (SWS EM).
- *Manifest Specification* (TPS Manifest).
- *AP Release Overview* (TR) — 클러스터/기능의 릴리스별 상태.

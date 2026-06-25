# 06. 통신 스택

AUTOSAR 구조 감각의 중심. 한 데이터가 SWC 에서 출발해 물리 버스에 실리기까지 **여러
모듈을 거치는 이유**와, 그 모듈들이 어떻게 책임을 나누는지를 이해하면 나머지 스택
(진단/메모리)도 같은 논리로 읽힌다.

## PDU 모델 — 통신의 기본 단위

AUTOSAR 통신은 **PDU(Protocol Data Unit)** 단위로 흐른다. 계층마다 부르는 이름이 다르다:

```
시그널(Signal)        : 애플리케이션이 보는 논리 데이터 (예: 차속, 도어상태)
   │  (여러 시그널을 묶음)
I-PDU (Interaction)   : Com 계층의 PDU. 시그널들의 묶음.
   │
N-PDU (Network)       : TP 계층의 PDU. 큰 I-PDU 를 세그먼트로 쪼갠 단위.
   │
L-PDU (Link/Data)     : If/드라이버 계층의 PDU. 실제 버스 프레임에 대응.
   │
Frame                 : 물리 버스 위의 프레임 (CAN/LIN/Ethernet ...)
```

핵심: **시그널 ↔ I-PDU ↔ 프레임** 의 매핑이 통신 설계(통신 매트릭스, →`03` System
Template)의 본질이다. "어떤 시그널이 어떤 PDU 에 어떤 비트 위치로 들어가 어느
프레임으로 어느 버스에 실리는가"를 ARXML 로 기술한다.

PDU 는 보통 `PduInfoType { SduDataPtr, SduLength, ... }` 로 다뤄진다. SDU(Service Data
Unit)=상위가 준 페이로드, PCI=프로토콜 제어정보. PDU = SDU + PCI.

## 송신 경로 (signal → CAN frame)

```
SWC
 │  Rte_Write/Send (포트)
 ▼
Com            시그널을 I-PDU 로 packing, 주기/이벤트 송신, timeout/notification
 │  PduR_ComTransmit
 ▼
PduR           PDU 라우터: 목적지 인터페이스 결정 (Com↔CanIf↔CanTp↔게이트웨이...)
 │  CanIf_Transmit
 ▼
CanIf          컨트롤러/HOH 추상화, L-PDU↔컨트롤러 매핑, 송신 버퍼
 │  Can_Write
 ▼
Can (MCAL)     하드웨어 메일박스 적재
 ▼
물리 CAN 버스
```

## 수신 경로 — 콜백으로 거슬러 올라감

수신은 ISR/폴링으로 시작해 **아래→위 콜백**으로 올라온다(제어 흐름이 송신과 반대):

```
Can Rx ISR → CanIf_RxIndication → CanIf
 → PduR_CanIfRxIndication → PduR
 → Com_RxIndication → Com (I-PDU 언패킹 → 시그널)
 → Rte_Write (수신 SWC 포트) → SWC (DataReceivedEvent 로 runnable 트리거)
```

왜 콜백이냐 → 계층 불변식(의존은 위→아래, →`02`). 하위는 상위를 이름으로 알면 안
되므로, 상위가 등록해 둔 콜백을 부른다. 이름 규칙: `<상위>_RxIndication`,
`<상위>_TxConfirmation`.

## 왜 이렇게 잘게 나눴나 (Com / PduR / If / Driver)

- **Com**: 버스 무관. "시그널/I-PDU"라는 논리만. CAN 이든 FlexRay 든 Com 은 동일.
  시그널 packing/unpacking, 송신 모드(주기/이벤트/혼합), 수신 timeout monitoring,
  notification 을 담당.
- **PduR**: 라우팅 전담. 같은 PDU 를 Com 으로, 진단(Dcm)으로, TP 로, 또는 다른 버스로
  (게이트웨이) 보낼 수 있다. 이 "갈림길"을 한 모듈로 집중. 게이트웨이 ECU 의 핵심.
- **CanIf**: CAN '인터페이스'. 컨트롤러가 여러 개여도, 트랜시버가 달라도 상위엔 동일.
  컨트롤러 모드 제어, L-PDU↔HTH/HRH 매핑.
- **Can**: 진짜 레지스터/메일박스.

분리의 이득: 버스를 CAN→FlexRay 로 바꿔도 Com 위(SWC/RTE)는 불변. PduR 설정과 하위
If/Driver 만 교체. **교체 단위마다 모듈을 끊는다**는 원칙(→`02`)의 전형.

## Transport Protocol (TP) — 큰 메시지 분할

한 프레임 페이로드(CAN 8B, CAN FD 64B)보다 큰 데이터(진단 응답 등)는 세그먼테이션이
필요하다. 그게 TP:
- `CanTp`: ISO 15765-2(ISO-TP). First Frame / Consecutive Frame / Flow Control,
  block size, separation time. 진단의 대용량 페이로드를 운반.
- `LinTp`, `FrTp`, `J1939Tp`(BAM/CMDT): 각 버스의 TP.
- 경로: `Dcm` ↔ `PduR` ↔ `CanTp` ↔ `CanIf` ↔ `Can`. TP 는 주로 진단과 짝(→`07`).

## 버스별 스택 구조

### CAN 스택
```
Com / Dcm / ...
   PduR
   ├─ CanIf ── Can(MCAL) ── CanTrcv     (일반/대량 PDU)
   └─ CanTp ── CanIf ...                (세그먼트 PDU)
   CanSM (상태) · CanNm (네트워크관리) · CanTSyn (시간동기)
```
- `CanSM`(State Manager): 채널 통신 모드 ↔ CanIf 컨트롤러 모드.
- `CanNm`(Network Management): 슬립/웨이크 조율(분산 합의).

### LIN 스택
`LinIf`(스케줄 테이블 포함) ── `Lin`(MCAL) ── `LinTrcv`. 상위에 `LinSM`(상태),
`LinTp`(진단 세그먼트). LIN 은 마스터-슬레이브·스케줄 기반이라 LinIf 가 스케줄 테이블을
돌린다는 점이 특징.

### FlexRay 스택 (고대역·결정론)
`FrIf` ── `Fr`(MCAL) ── `FrTrcv`. 상위 `FrSM`/`FrNm`/`FrTp`. 정적 슬롯(시간 트리거)+
동적 세그먼트의 TDMA 스케줄. 섀시 등 고신뢰 도메인.

### Ethernet 스택 (서비스 지향·고대역)
```
SOME/IP (SomeIpXf) ── SD (Service Discovery)
   SoAd (Socket Adaptor: PDU↔소켓)
   TcpIp (UDP/TCP/IP)
   EthIf ── Eth(MCAL) ── EthTrcv(PHY)
   EthSM (상태) · EthTSyn (시간동기 gPTP) · DoIP (진단 over IP)
```
- `SoAd`: PDU 와 TCP/UDP 소켓을 잇는 핵심.
- `Sd`: 서비스 디스커버리(SOME/IP-SD: offer/find/subscribe).
- 차량 이더넷·SOA(서비스 지향)의 토대. (Adaptive Platform 의 통신과 개념이 이어짐, →`12`)

## Network Management (NM)

버스의 ECU 들이 **합의해서** 같이 깨어있고 같이 잠들게 하는 분산 알고리즘. 혼자 끄면
통신이 깨지므로 조율이 필요.
- `Nm`(코디네이터) + `CanNm`/`FrNm`/`J1939Nm`(버스별). NM PDU 주기 송신/감시.
- **Partial Networking**: 버스 일부만 깨워 전력 절감(필요한 ECU 그룹만 active).

## Communication Manager (ComM)

여러 "사용자"(SWC, Dcm 등)의 통신 요구를 모아 채널 통신 모드(Full / Silent / No
Communication)를 중재. "진단 중이니 버스 깨워둬" 같은 요청을 받아 State Manager 로 내림.
- 흐름: 사용자 → `ComM` → `CanSM`/`LinSM`/... → If → Driver, 그리고 `Nm` 과 슬립 조율.
- 계층 모드의 중심: 누가 통신을 원하는지를 ComM 이 집계.

## IpduM — PDU 멀티플렉싱

한 PDU 자리에 상황에 따라 다른 콘텐츠를 싣는 멀티플렉싱(dynamic part + static part).
대역폭 절약. `Com` ↔ `IpduM` ↔ `PduR`.

## 안전/보안 통신: E2E, SecOC

- **E2E (End-to-End Protection)**: 송신 SWC 가 데이터에 CRC+counter 를 씌우고 수신
  SWC 가 검사 → 중간 어디서 손상/유실/중복/지연돼도 탐지. ISO 26262 안전 데이터의
  무결성을 통신 경로 끝에서 끝까지 보장(중간 모듈을 신뢰하지 않음). 라이브러리(E2E)
  또는 transformer(E2EXf)로 적용. → `10`.
- **SecOC (Secure Onboard Communication)**: PDU 에 MAC(인증코드)+freshness value 를
  붙여 **변조/재전송 공격**을 막는다. PduR 경로에 끼어 인증 수행, 암호 연산은
  `CryIf`/`Csm`(→`11`)에 위임. 보안(authenticity), E2E 는 안전(integrity) — 목적이 다름.

## Transformer 체인

RTE 와 Com 사이에 변환기를 끼워 직렬화/보호를 자동화: `SomeIpXf`(SOME/IP 직렬화),
`E2EXf`(E2E 보호), `ComXf`(시그널↔바이트). SWC 는 transformer 존재를 모르고 RTE 설정이
체인을 조립. "보호/직렬화를 코드가 아니라 설정으로 조립"하는 사고. → `12` 와 연결.

## 게이트웨이 (PduR 의 또 다른 얼굴)

여러 버스를 잇는 게이트웨이 ECU 에선 PduR 이 한 버스에서 받은 PDU 를 다른 버스로
**라우팅**한다(시그널 게이트웨이는 Com 레벨, PDU 게이트웨이는 PduR 레벨). 도메인 간
데이터 중계의 핵심.

## 왜 이런 구조인가 (분석)

- **버스 독립성**: Com 위는 버스를 모름 → SWC/기능을 버스 너머로 재사용.
- **라우팅 집중(PduR)**: 모든 경로 결정을 한 모듈로 모아 게이트웨이/멀티캐스트/진단을
  일관되게.
- **관심사 분리**: packing(Com)·라우팅(PduR)·세그먼트(TP)·상태(SM)·관리(NM)·인증
  (SecOC)·무결성(E2E)을 각자 모듈로. 각각 독립 검증/교체 가능.

## 원문에서 확인할 것
- *Layered Software Architecture* (EXP) — 통신 스택 다이어그램.
- *Specification of Communication* (SWS COM), *PDU Router* (SWS PduR),
  *CAN Interface* (SWS CanIf), *CAN Transport Layer* (SWS CanTp).
- *Specification of SecOC*, *Specification of E2E*, *E2E Protocol* (PRS E2E).
- *Specification of SOME/IP*, *Socket Adaptor* (SoAd) — 이더넷/SOA.

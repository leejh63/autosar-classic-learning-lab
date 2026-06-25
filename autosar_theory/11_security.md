# 11. 사이버보안 (Security)

커넥티드/자율주행으로 차량이 외부와 연결되면서 **사이버보안** 이 안전만큼 중요해졌다.
AUTOSAR 는 암호 서비스, 통신 인증, 침입 탐지, 키 관리를 표준 모듈로 제공한다. 관련
규제로 UN R155(CSMS)·ISO/SAE 21434 가 있다.

## 보안의 목표 (CIA + 인증)

- **기밀성(Confidentiality)**: 데이터를 권한 없는 자가 못 읽게(암호화).
- **무결성(Integrity)**: 데이터가 변조되지 않았음(해시/MAC).
- **인증(Authenticity)**: 데이터/송신자가 진짜임(MAC/서명).
- **가용성·재전송 방지**: freshness(카운터/타임스탬프)로 재전송 공격 차단.

## Crypto 스택 — 암호 서비스의 계층

```
SWC / SecOC / KeyM (요청자)
 │  Csm_*  (Hash, MAC, Encrypt/Decrypt, Signature, RNG ...)
 ▼
Csm            Crypto Service Manager — 암호 서비스의 논리 인터페이스, 잡 큐잉(비동기)
 │  CryIf_*
 ▼
CryIf          Crypto Interface — 여러 crypto driver 중 라우팅
 │
 ▼
Crypto         Crypto Driver — SW 구현 또는 HW 백엔드(HSM/SHE)
```

- **Csm (Crypto Service Manager)**: "MAC 만들어줘/검증해줘" 같은 요청의 표준 창구.
  잡(job) 기반, 비동기. 알고리즘 추상화(상위는 알고리즘 구현 위치를 모름).
- **CryIf (Crypto Interface)**: 여러 드라이버(SW/HSM)로 라우팅하는 디스패처.
- **Crypto Driver**: 실제 연산. 소프트웨어 구현 또는 보안 하드웨어.

### 보안 하드웨어: SHE / HSM
- **SHE (Secure Hardware Extension)**: 경량 보안 하드웨어(키 저장+기본 암호).
- **HSM (Hardware Security Module)**: 별도 보안 코어. 키를 메인 코어로부터 격리 저장,
  고속 암호 연산, secure boot 지원. 키가 소프트웨어에 절대 노출되지 않게 함.

## SecOC — 통신 인증

PDU 에 **MAC(Message Authentication Code) + Freshness Value** 를 붙여 **변조/재전송
공격**을 막는다.
- 송신: 페이로드 + freshness 로 MAC 계산 → PDU 에 MAC(+ freshness 일부) 부착.
- 수신: MAC 재계산·검증 + freshness 검사(재전송 거부). 실패 시 폐기.
- 위치: PduR 경로에 끼어 인증 수행, 암호 연산은 CryIf/Csm 에 위임(→`06`).
- **freshness**: 단조 증가 카운터/타임스탬프. 같은 메시지의 재전송을 무력화.

> 구분: **SecOC = 보안(authenticity, 악의적 공격 방지)**, **E2E = 안전(integrity,
> 무작위 고장 방지)**(→`10`). 둘 다 PDU 에 보호 필드를 붙이지만 목적·위협 모델이 다르다.
> 함께 쓰일 수 있다.

## KeyM — 키 관리

암호 키와 인증서의 생애주기 관리: 저장, 갱신, 인증서 검증(PKI), 배포. 키 자료를 안전하게
다루는 표준 절차. HSM/SHE 의 키 슬롯과 연계.

## IdsM — 침입 탐지 (IDS)

**Intrusion Detection System Manager**. 보안 이벤트(Security Event)를 수집·필터·집계해
보고한다. 비교적 신규로 추가된 영역.
- 각 모듈/SWC 가 의심스러운 활동(인증 실패 반복, 비정상 메시지 등)을 보안 이벤트로 보고.
- IdsM 이 이를 자격(qualify)·필터링해 **IdsR(리포터)** 나 차량 SOC(보안 운영센터)로 전달.
- 목적: 공격을 실시간 탐지·기록해 대응/포렌식에 활용. 진단의 Dem 과 유사한 "이벤트
  관리" 구조지만 대상이 보안 위협.

## Secure Boot / 무결성 검증

부팅 시 펌웨어 이미지의 서명을 검증해 변조된 코드의 실행을 막는다(보통 HSM 기반).
신뢰 사슬(root of trust → 부트로더 → 애플리케이션)을 형성. AUTOSAR 모듈만으로
완결되진 않고 부트로더/HSM 과 함께 구성.

## 최근 흐름 (참고)

차량 이더넷 보안으로 **MACsec**(L2 암호화) 지원이 강화되는 등, 보안 기능은 릴리스마다
확장되고 있다. 세부는 해당 릴리스의 보안 관련 명세/릴리스 노트로 확인.

## 왜 이런 구조인가 (분석)

- **암호 추상화(Csm/CryIf/Crypto)**: 상위는 "MAC 필요"만 알고, SW/HSM 구현 위치는
  드라이버 계층에서 결정 → 보안 하드웨어 교체/업그레이드 유연.
- **키 격리(HSM)**: 키를 메인 SW 로부터 분리해, SW 가 뚫려도 키가 안 새게.
- **통신 인증 표준화(SecOC)**: 모든 ECU 가 동일 방식으로 메시지 진위/신선도를 보장.
- **탐지의 표준화(IdsM)**: 흩어진 보안 이벤트를 한 체계로 모아 차량 차원 대응.
- **safety/security 통합**: 보안 침해가 안전 사고로 번지지 않게 두 영역을 함께 설계.

## 원문에서 확인할 것
- *Specification of Crypto Service Manager* (SWS Csm), *Crypto Interface* (SWS CryIf),
  *Crypto Driver* (SWS Crypto).
- *Specification of Secure Onboard Communication* (SWS SecOC).
- *Specification of Key Manager* (SWS KeyM), *Intrusion Detection System Manager* (SWS IdsM).
- ISO/SAE 21434(차량 사이버보안 엔지니어링), UN R155 — 프로세스/규제 원문.

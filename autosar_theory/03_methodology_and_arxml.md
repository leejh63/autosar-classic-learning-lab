# 03. 방법론과 ARXML — AUTOSAR 의 진짜 핵심

많은 입문자가 계층 구조만 배우고 여기서 막힌다. 하지만 AUTOSAR 개발의 실제 모습은
**"코드를 짠다"가 아니라 "모델을 설정하고 코드를 생성한다"** 이다. 이 장이 그 전환점이다.

## VFB — Virtual Function Bus

설계의 출발점은 **가상 기능 버스(VFB)** 라는 추상이다. 아이디어:

> 모든 SWC 가 하나의 "가상 버스"에 꽂혀 있고, 서로 포트로 연결되어 통신한다고
> **가정하고 설계**한다. 그 SWC 가 실제로 어느 ECU 에 올라가는지, 통신이 같은 ECU
> 내부인지 CAN 버스 너머인지는 **나중에 매핑**한다.

```
설계 시점(VFB):  [SWC A] ──port── [SWC B]   ←  배치/버스 모름. 논리적 연결만.
                       (가상 버스)

실현 시점(RTE):  같은 ECU 면 → 함수 호출/메모리
                 다른 ECU 면 → Com→PduR→CanIf→Can→버스
```

VFB 의 의의: **기능 설계와 배치(deployment)를 분리**한다. 같은 SWC 망을 1-ECU 로도,
3-ECU 로도 배치할 수 있고, SWC 코드는 안 바뀐다. RTE 가 VFB 를 특정 ECU 에서 실제로
구현해 준다(→ `05`).

## 메타모델 — 모든 것의 뿌리

AUTOSAR 의 모든 산출물(SWC 기술, ECU 기술, BSW 설정)은 하나의 **메타모델
(meta-model)** 을 따른다. 메타모델은 "AUTOSAR 모델에 무엇을 적을 수 있는가"를 정의한
스키마다(UML 클래스 모델로 명세됨).

- 메타모델의 인스턴스가 곧 **AUTOSAR 모델**이고, 그 모델을 파일로 직렬화한 게 **ARXML**.
- 즉: 메타모델(규칙) → 모델(내용) → ARXML(파일). 데이터베이스 스키마와 레코드의 관계.

이 단일 메타모델 덕에 서로 다른 벤더 툴이 같은 ARXML 을 주고받을 수 있다(상호운용성).

## ARXML — AUTOSAR XML

ARXML 은 AUTOSAR 모델을 담는 XML 파일이다. SWC 의 포트/인터페이스/runnable, 데이터
타입, ECU 의 통신 매트릭스, 각 BSW 모듈의 설정값까지 전부 ARXML 로 기술된다.

- 사람이 손으로 쓰기엔 방대하고 엄격해서, 보통 **설정 툴(GUI)** 로 편집하고 ARXML 로
  저장한다.
- 텍스트라서 형상관리(git diff)는 가능하지만, 구조가 깊어 실전에선 툴 중심으로 다룬다.

> 베어메탈에서 "헤더에 #define 으로 설정"하던 것을, AUTOSAR 는 "ARXML 모델에 기술
> → generator 가 `_Cfg.h`/`_PBcfg.c` 를 생성"하는 방식으로 끌어올린 것이다. 설정이
> 코드가 아니라 **데이터(모델)** 가 된다.

## 템플릿 — ARXML 의 용도별 분류

ARXML 내용은 역할에 따라 "템플릿"으로 분류된다(파일이 아니라 모델 영역의 종류):
- **SW Component Template (SWCT)**: SWC 의 포트/인터페이스/internal behavior/runnable.
- **System Template**: 시스템 전체 — 토폴로지, ECU 들, 통신 매트릭스(어떤 신호가 어떤
  프레임에 실려 어느 버스로), SWC↔ECU 매핑.
- **ECU Resource Template**: ECU 하드웨어 자원(핀, 페리페럴, 메모리).
- **BSW Module Template / ECU Configuration**: 각 BSW 모듈의 설정 파라미터.

## 방법론 — 설계에서 생성까지의 흐름

AUTOSAR 표준 개발 과정(단순화):

```
1) 시스템 설계 (VFB 레벨)
   - SWC 들과 포트/인터페이스 정의            → SW Component Description (ARXML)
   - 시스템 토폴로지/통신 매트릭스 정의        → System Description (ARXML)
2) SWC → ECU 매핑 (배치 결정)
   - 어느 SWC 가 어느 ECU 에 올라가는지 결정
3) ECU 추출 (ECU Extract)
   - 한 ECU 에 관련된 부분만 System Description 에서 뽑아냄  → ECU Extract (ARXML)
4) ECU 설정 (BSW + RTE Configuration)
   - 그 ECU 의 BSW 모듈들과 RTE 를 설정         → ECU Configuration (ARXML)
5) 코드 생성 (Generation)
   - RTE generator → Rte.c/Rte.h
   - BSW generator → 각 <Mod>_Cfg.h / _PBcfg.c 등
6) 컴파일/링크 → ECU 바이너리
```

이 과정에서 **사람이 손으로 짜는 코드는 주로 SWC 내부 로직(runnable 본문)과 일부
드라이버 구현**뿐이다. 나머지(RTE 배선, BSW 설정 코드)는 모델에서 생성된다. 이게
"AUTOSAR 는 코드보다 설정"이라는 말의 실체다.

## 설정 클래스 — 언제 설정이 확정되나

같은 파라미터라도 "언제 값이 굳는가"가 다르다. 세 가지 설정 클래스:

| 클래스 | 확정 시점 | 산출물 | 바꾸려면 | 장단점 |
|--------|-----------|--------|----------|--------|
| Pre-compile (PC) | 컴파일 전 | `<Mod>_Cfg.h` (#define) | 재컴파일 | 최적화 좋음, 유연성 낮음 |
| Link-time (LT) | 링크 시 | `<Mod>_Lcfg.c` (const) | 재링크 | 중간 |
| Post-build (PB) | 바이너리 로드 후 | `<Mod>_PBcfg.c`/별도 영역 | 재플래시(재빌드 X) | 유연성 최고, 오버헤드 |

**Post-build 가 `<Mod>_Init(const <Mod>_ConfigType*)` 시그니처의 존재 이유다.** 같은
코드 바이너리에 설정 데이터만 차종/트림별로 바꿔 끼우려면, 설정은 "코드"가 아니라
"데이터(포인터로 넘기는 구조체)"여야 한다. 그래서 모든 모듈 초기화가 설정 포인터를
받는다. 이 한 가지를 이해하면 AUTOSAR 의 많은 설계가 풀린다.

- **Variant Handling**: PB 의 확장. 하나의 SW 에 여러 설정 variant(예: 좌/우핸들,
  사양별)를 두고 선택. predefined variant, postbuild selectable 등의 메커니즘.

## 벤더 툴 생태계 (현실)

표준은 명세일 뿐, 실제 작업은 툴로 한다. 대표 카테고리:
- **설정/생성 툴**: EB tresos Studio, Vector DaVinci(Developer/Configurator), ETAS ISOLAR.
- **BSW/MCAL 스택**: 반도체사(NXP/Infineon/...)와 Tier-1 이 칩별로 제공.
- **통신 설계**: DBC/Fibex/ARXML 기반 통신 매트릭스 툴(예: Vector CANdb++/PREEvision).

핸드북은 표준 개념에 집중하지만, 실무는 "어떤 벤더 조합을 쓰느냐"가 경험을 크게
좌우한다는 점은 알아두자.

## 왜 이런 방법론인가 (분석)

- **단일 진실 원천(SSOT)**: 모델(ARXML)이 진실이고 코드는 파생물. 설정 변경이
  코드 곳곳의 손수정으로 흩어지지 않는다.
- **상호운용성**: 표준 메타모델/ARXML 덕에 벤더 툴 간 모델 교환이 가능.
- **자동화·일관성**: 반복적·오류나기 쉬운 배선/설정 코드를 생성으로 대체.
- **분업 친화**: 시스템 설계(OEM)·SWC 개발(공급사)·ECU 통합이 모델 단계로 분리.

대가는 툴 종속과 학습비용이다. 하지만 수백 개 모듈 × 수천 파라미터를 손으로 일관되게
관리하는 건 불가능에 가깝기에, 모델/생성 방식은 사실상 필수적 선택이다.

## 원문에서 확인할 것
- *Methodology* (TR Methodology) — 단계/작업/산출물의 1차 출처.
- *Virtual Functional Bus* (EXP VFB) — VFB 개념.
- *Meta Model* / *Generic Structure Template* / *Template UML Profile* — 메타모델.
- *Specification of ECU Configuration* — 설정 클래스/파라미터 모델.

# V-Model Checklist

기능 하나를 추가할 때마다 아래를 확인한다.

1. 요구사항 ID가 있는가?
2. 요구사항이 테스트 가능한 문장인가?
3. 설계 문서에서 책임 모듈이 정해졌는가?
4. SWC가 MCAL-like driver나 VirtualHw를 직접 호출하지 않는가?
5. RTE interface가 입력과 출력을 분리하는가?
6. IoHwAb가 논리 signal과 hardware signal 변환을 담당하는가?
7. 설정은 `Board_Cfg.c`에 있는가?
8. MCAL-like driver는 VirtualHw에만 접근하는가?
9. unit test가 SWC를 검증하는가?
10. module test가 ECU 흐름을 검증하는가?
11. traceability matrix가 갱신되었는가?

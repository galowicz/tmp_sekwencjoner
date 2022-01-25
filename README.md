# tmp_sekwencjoner

Projekt z przedmiotu Technika Mikroprocesorowa 2
Temat Projektu:
  Sekwencjoner
Projekt wykonali:
  Chohura Monika
  Gałowicz Krzysztof

wykorzystane zasoby:
  FRDM-KL05
  macierz klawiszy 4x4
  głośnik
  pamięć SPI NOR FLASH EN25F80
Sposób połączenia:
  klawisze:
    C1 - PTB1
    C2 - PTB2
    C3 - PTB3
    C4 - PTB4
    R1 - PTA8
    R2 - PTA9
    R3 - PTA10
    R4 - PTA11
  głośnik 
    Audio_in - PTB7
  Pamięć:
    /CS - PTA5
    DO - PTA6
    DI - PTA7
    CLK - PTB0
    
Diody led świecą zgodnie z poniższym:
  niebieska - nagrywana jest pętla
  czerwona:
     przy starcie - trwa inicjalizacja
     w trakcie odgrywania pętli - rozpoczęcie odgrywania od początku pętli
  zielona - odgrywanie dzwięku

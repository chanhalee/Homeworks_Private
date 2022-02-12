### HW2 를 위한 입력 파일 생성기

**실행 방법**

`./generator.pl [RANGE_START][RANGE_END][QUANTITY] > [FILE_NAME]`

**예시**

1 ~ 90000 범위의 9999개의 랜덤한 숫자를 갖는 temp.txt 생성

`./generator.pl 1 90000 9999 > temp.txt`

**주의**

generator.pl의 모든 인자 반드시 양수여야 합니다.

모든 인자는 5자리 이하의 숫자여야 합니다.

RANGE_START 는 RANGE_END 보다 커선 안됩니다.


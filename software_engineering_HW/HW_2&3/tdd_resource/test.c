#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CUnit/Basic.h"
#include "ft.h"

void testAtoi_1(void){	// 첫번째 테스트 ft_atoi()의 정상동작을 시험하는 함수이다.

	char	*str = " -01234i";
	CU_ASSERT_EQUAL(ft_atoi(str), atoi(str));// ft_atoi()와 atoi()의 동작이 동일한지 평가한다.
	//CU_ASSERT_EQUAL()은 인자로 받은 두 값이 일치할 경우 pass판정을 내리는 함수이다.
}
void testStrcat_1(void){	// 두번째 테스트 ft_strcat()의 정상동작을 시험하는 함수이다.

	char	*str = " am I?";
	char	compare[10];
	int i = -1;
	while (++i < 10)
	{
		compare[i] = '\0';
	}
	compare[0] = 'W';
	compare[1] = 'h';
	compare[2] = 'o';
	CU_ASSERT_STRING_EQUAL(ft_strcat(compare, str), "Who am I?");// ft_strcat()의 동작이 정확한지 평가한다.
	//CU_ASSERT_STRING_EQUAL()은 인자로 받은 두 문자열이 일치할 경우 pass 판정을 내리는 함수이다.
}
int main(){
	CU_pSuite pSuite = NULL; // CU_pSuite는 테스트를 주관하는 스위트 함수의 포인터를 보관한다.
	if (CUE_SUCCESS != CU_initialize_registry())	// 레지스트리를 초기화 하며 실패시 에러 코드 반환후 종료
		return CU_get_error();
	pSuite = CU_add_suite("Suite_1", NULL, NULL);// 스위트를 등록하는데 죄측에 보이는 함수의 2번째와 3번째 인자는
												 // 원래 테스트 시작과 종료시 수행되어야 할 함수가 들어가야 한다.
												 // 현제 테스트는 파일입출력 등 이러한 동작이 필요하지 않으므로
												 // 아무것도 넣지 않는다.
	if (NULL == pSuite) {						 // 레지스트리 등록에 실패할 경우 cleanUp하고 종료하도록 한다.
		CU_cleanup_registry();
		return CU_get_error();
	}
	/* add the tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if ((NULL == CU_add_test(pSuite, "test of my_atoi()", testAtoi_1)) ||	// 첫번째 테스트를 등록한다.
			(NULL == CU_add_test(pSuite, "test of my_strcat()", testStrcat_1)))	// 두번째 테스트를 등록한다.
	{
		CU_cleanup_registry();	//수행중 문제가 발생하면 cleanup하고 종료한다.
		return CU_get_error();
	}	
	CU_basic_set_mode(CU_BRM_NORMAL);	//실패사유와 테스트 서머리 출력 모드로 설정. 
										//(해당 모드 실행에 필요한 상수들을 자동으로 세팅해준다.)
	CU_basic_run_tests(); // 등록된 모든 suite의 테스트 실행. 에러발생 즉시 테스트 중단.
	CU_cleanup_registry();
	return CU_get_error();	// 테스트 중 fail이나 에러가 발생할 경우 error코드를 반환하고
							// 정상적으로 모두 통과되었을 경우 CUE_SUCCESS 라는 상수를 반환한다.
}

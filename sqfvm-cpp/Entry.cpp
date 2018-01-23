#include "full.h"


int main(int argc, char** argv)
{
	sqf::commandmap::get().init();
	auto vm = sqf::virtualmachine();

	vm.out() << std::endl << L"ASSEMBLY" << std::endl;
	//vm.parse_assembly(L"push SCALAR 1; push SCALAR 1; callBinary +; push SCALAR 2; callBinary -;");
	//vm.parse_assembly(L"push SCALAR 1; push STRING 'test'; push STRING 'test''escape'; push SCALAR 1; push SCALAR 2; push SCALAR 3; callnular ntest; makeArray 4; makeArray 4;");
	//vm.parse_assembly(L"push SCALAR 12; assignToLocal _test;");
	vm.execute();
	
	vm.out() << std::endl << L"SQF" << std::endl;
	vm.parse_sqf(L"1 + 1 - 2; 0; {1 + 2 * 3 - 1 * 4}; 6; utest 1; ntest; [1, 'test', 'test''escape', [1, 2, 3, ntest]]; private _test = 12; _test2 = 13; _test; globalvar;");
	//vm.parse_sqf(L"1 + 1 - 2;");
	//vm.parse_sqf(L"[1, 'test', 'test''escape', [1, 2, 3, ntest]]");
	//vm.parse_sqf(L"private _test = 12;");
	vm.execute();

	sqf::commandmap::get().uninit();
	system("pause");
}

/*
    0       4         0       4    0
[1, 2, 3] select ([0, 1, 2] select 2);

                  [0, 1, 2]        2
[1, 2, 3]        (          select  )
          select                     ;
                       [0, 1]
                 count          1
[1, 2, 3]                     +
          select                     ;

- (...) wie konstanten wert behandeln
- (...) rekursiv abarbeiten
- array mit index f�hren

*/
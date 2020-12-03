//saltiniai: https://stackoverflow.com/questions/33938212/chow-to-convert-string-into-integer-without-using-any-built-in-functions is C++ kodo konvertuotas i asembleri stringo i int konvertavimas
// https://en.wikipedia.org/wiki/X86_instruction_listings#SSE_instructions
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char** argv)
{
	int pasirinkimas;

	do
	{
		cout << "Meniu: \n";
		cout << "1. Pirma dalis\n";
		cout << "2. Antra dalis\n";
		cout << "3. Baigti programa\n";
		cout << "Pasirinkite: ";
		cin >> pasirinkimas;

		if (pasirinkimas == 1)
		{
			if (argc < 2)
			{
				cout << "Klaida! Truksta parametru!" << endl;
				return 0;
			}

			int rez = 0;
			string temp = argv[1];
			char tekstas[10];

			for (int i = 0; i < temp.length(); i++)
			{
				tekstas[i] = temp[i];
			}

			int tekstas_ilgis = temp.length();

			_asm {
				push eax // registru inicializavimas, tas pats kaip: int pavyzdis
				push ebx
				push ecx
				push edx
				////////////////////////////

				mov ebx, 0 // ebx = 0
				mov ecx, 0 // ecx = 0

				while_ciklas:
				cmp ecx, tekstas_ilgis// pradedu lygint
					je exit_ciklas //if ecx = tekstas_ilgis, tada exit_loop
					mov al, tekstas[ecx] // al = tekstas[ecx], paimu raide is masyvo
					sub eax, '0' // eax = eax – '0', is tikro al - '0'
					imul ebx, 10 // ebx = ebx * 10
					add ebx, eax // ebx = ebx + eax
					inc ecx // ecx = ecx + 1
					jmp while_ciklas //griztame i ciklo pradzia

					exit_ciklas : //iseiname is ciklo
				mov rez, ebx // rez = ebx

				////////////////////////////
					pop edx
					pop ecx
					pop ebx
					pop eax // registru valymas
			}

			cout << "\nAts: " << rez << endl << endl;
		}
		else if (pasirinkimas == 2)
		{
			ofstream fr("rezpitagoras.txt");

			float A[4], B[4], C[4], rez[4];

			for (int a = 1; a <= 1000; a = a + 4)
			{

				for (int b = 2; b <= 1000; b = b + 4)
				{
					A[0] = a;
					A[1] = a + 1;
					A[2] = a + 2;
					A[3] = a + 3;
					B[0] = b;
					B[1] = b + 1;
					B[2] = b + 2;
					B[3] = b + 3;
					{
						__asm
						{
							//movups - mov
							//mulps - kelimas kvadratu
							//addps - sudetis
							//sqrtps - saknies traukimas
							//roundps - skaiciaus sveika dalis
							//subps - atimtis

							movups xmm0, A// xmm0 = 1 2 3 4
							movups xmm1, B // xmm1 = 2 3 4 5
							movups xmm2, xmm0 // xmm2 = 1 2 3 4
							movaps xmm3, xmm1 // xmm3 = 2 3 4 5
							mulps xmm2, xmm2 // xmm2 = 1 4 9 16
							mulps xmm3, xmm3 // xmm3 = 4 9 16 25
							movups xmm4, xmm3 // xmm4 = 4 9 16 25
							addps xmm4, xmm2 // xmm4 = 5 13 25 41
							movups xmm5, xmm4 // xmm5 = 5 13 25 41
							sqrtps xmm5, xmm5 // xmm5 = 2.23 3.6 5 6.4
							roundps xmm5, xmm5, 1 // sveika dalis arba C++ stiliuje: setprecision(1)
							movups C, xmm5   // C = 2 3 5 6
							mulps xmm5, xmm5 // xmm5 = 4 9 25 36
							subps xmm5, xmm4 // xmm5 = -1 -4 0 -5
							movups rez, xmm5 // rez = -1 -4 0 -5, musu 0 nusako kuriame indekse yra pitagoro skaiciai A B C masyvuose, siuo atveju indeksas 2
						}

						for (int i = 0; i < 4; i++)
						{
							if (rez[i] == 0)
							{
								fr << "A: " << A[i] << "\tB: " << B[i] << "\tZ: " << C[i] << endl;
							}
						}
					}
				}
			}
			fr.close();
			cout << "\nPaziurekite savo txt faila.\n\n";
		}
		else if (pasirinkimas == 3)
		{
			return 0;
		}
		else
		{
			cout << "Blogas pasirinkimas!!!\n";
		}

	} while (pasirinkimas != 3);
}
#include <Windows.h>
#include <virtdisk.h>
#include <string>
#include <iostream>
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"virtdisk.lib")

using namespace std;

/* Tratativa de erro ao usar valores de VIRTUAL_STORAGE_TYPE. */
/* Para usar os valores de VIRTUAL_STORAGE_TYPE, ter� que habilitar a compila��o de cabe�alho pr�-compilado "stdafx.h". */

class cFuncoes
{
private:

	HANDLE Disco;

	VIRTUAL_STORAGE_TYPE VirtType;
	
	COMPACT_VIRTUAL_DISK_PARAMETERS Parametros;

public:

	DWORD Result;

	bool AbrirDiscoVirtual(bool VHDX, wstring Local)
	{
		bool bResult = false;

		if (VHDX == true)
		{
			VirtType = {
			VIRTUAL_STORAGE_TYPE_DEVICE_VHDX,
			VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT
			};
		}
		if (VHDX == false)
		{
			VirtType = {
			VIRTUAL_STORAGE_TYPE_DEVICE_VHD,
			VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT
			};
		}

		//VIRTUAL_DISK_ACCESS_METAOPS para opera��es de modifica��o.
		//Use VIRTUAL_DISK_ACCESS_ALL para desanexar, e depois use VIRTUAL_DISK_ACCESS_METAOPS para compactar.
		Result = OpenVirtualDisk(&VirtType, Local.c_str(), VIRTUAL_DISK_ACCESS_METAOPS, OPEN_VIRTUAL_DISK_FLAG_NONE,
			0, &Disco);
		if (Result == ERROR_SUCCESS)
		{
			bResult = true;
		}
		else
		{
			cout << "Ocorreu um erro ao criar um identificador para o disco virtual, c�digo de erro: " << GetLastError();
		}

		return bResult;
	}

	void CompactarDiscoVirtual(bool VHDX, wstring Local)
	{	

		if (AbrirDiscoVirtual(VHDX, Local) == true)
		{
			memset(&Parametros, 0, sizeof(Parametros));

			Parametros.Version = COMPACT_VIRTUAL_DISK_VERSION_1;
			Parametros.Version1.Reserved = 0;

			//Esta fun��o n�o possui par�metro de fun��o ass�ncrona, sendo �til apenas de uso s�ncrono.
			if (CompactVirtualDisk(Disco, COMPACT_VIRTUAL_DISK_FLAG_NONE, &Parametros, 0) == ERROR_SUCCESS)
			{
				cout << "O disco selecionado foi compactado com sucesso.";
			}
			else
			{
				cout << "Ocorreu um erro durante a opera��o: " << GetLastError();
			}

			CloseHandle(Disco);
		}

	}

	void DesanexarDiscoVirtual(bool VHDX, wstring Local)
	{
		
		if (AbrirDiscoVirtual(VHDX, Local) == true)
		{
			if (DetachVirtualDisk(Disco, DETACH_VIRTUAL_DISK_FLAG_NONE, 0) == ERROR_SUCCESS)
			{
				cout << "O disco selecionado foi desanexado com sucesso.";
			}
			else
			{
				cout << "Ocorreu um erro durante a opera��o: " << GetLastError();
			}

			CloseHandle(Disco);
		}

	}

}Funcoes;

int main()
{

	cout << "O assistente est� compactando um disco virtual...";

	//A opera��o de compacta��o � v�lida somente em discos din�micos, pois podem ser reduzidos.
	//Para discos din�micos, conforme o uso, podem aumentar capacidade ao seu tamanho m�ximo, e podem ser reduzidos facilmente.
	Funcoes.DesanexarDiscoVirtual(false, L"C:\\DISCOS VIRTUAIS\\DISCO.vhd");
	//--
	Funcoes.CompactarDiscoVirtual(false, L"C:\\DISCOS VIRTUAIS\\DISCO.vhd");

	system("pause");
}
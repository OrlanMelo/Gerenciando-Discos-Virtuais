#include <Windows.h>
#include <virtdisk.h>
#include <string>
#include <iostream>
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"virtdisk.lib")

using namespace std;

/* Tratativa de erro ao usar valores de VIRTUAL_STORAGE_TYPE. */
/* Para usar os valores de VIRTUAL_STORAGE_TYPE, terá que habilitar a compilação de cabeçalho pré-compilado "stdafx.h". */

class cFuncoes
{
private:

	HANDLE Disco;

	VIRTUAL_STORAGE_TYPE VirtType;
	
	ATTACH_VIRTUAL_DISK_PARAMETERS Parametros;

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

		Result = OpenVirtualDisk(&VirtType, Local.c_str(), VIRTUAL_DISK_ACCESS_ALL, OPEN_VIRTUAL_DISK_FLAG_NONE,
			0, &Disco);
		if (Result == ERROR_SUCCESS)
		{
			bResult = true;
		}
		else
		{
			cout << "Ocorreu um erro ao criar um identificador para o disco virtual, código de erro: " << GetLastError();
		}

		return bResult;
	}

	void AnexarDiscoVirtual(bool VHDX, wstring Local)
	{	

		if (AbrirDiscoVirtual(VHDX, Local) == true)
		{
			memset(&Parametros, 0, sizeof(Parametros));

			Parametros.Version = ATTACH_VIRTUAL_DISK_VERSION_1;
			Parametros.Version1.Reserved = 0;
			if (AttachVirtualDisk(Disco, 0, ATTACH_VIRTUAL_DISK_FLAG_PERMANENT_LIFETIME, 0, &Parametros, 0) == ERROR_SUCCESS)
			{
				cout << "O disco selecionado foi anexado com sucesso.";
			}
			else
			{
				cout << "Ocorreu um erro durante a operação: " << GetLastError();
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
				cout << "Ocorreu um erro durante a operação: " << GetLastError();
			}

			CloseHandle(Disco);
		}

	}

}Funcoes;

int main()
{

	cout << "O assistente está anexando ou desanexando um disco virtual...";

	Funcoes.AnexarDiscoVirtual(false, L"C:\\DISCOS VIRTUAIS\\DISCO.vhd");
	//--
	//Ou
	//--
	Funcoes.DesanexarDiscoVirtual(false, L"C:\\DISCOS VIRTUAIS\\DISCO.vhd");

	system("pause");
}
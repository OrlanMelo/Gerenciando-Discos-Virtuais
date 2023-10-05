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

		//VIRTUAL_DISK_ACCESS_METAOPS para operações de modificação.
		//Use VIRTUAL_DISK_ACCESS_ALL para desanexar, e depois use VIRTUAL_DISK_ACCESS_METAOPS para compactar.
		Result = OpenVirtualDisk(&VirtType, Local.c_str(), VIRTUAL_DISK_ACCESS_METAOPS, OPEN_VIRTUAL_DISK_FLAG_NONE,
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

	void CompactarDiscoVirtual(bool VHDX, wstring Local)
	{	

		if (AbrirDiscoVirtual(VHDX, Local) == true)
		{
			memset(&Parametros, 0, sizeof(Parametros));

			Parametros.Version = COMPACT_VIRTUAL_DISK_VERSION_1;
			Parametros.Version1.Reserved = 0;

			//Esta função não possui parâmetro de função assíncrona, sendo útil apenas de uso síncrono.
			if (CompactVirtualDisk(Disco, COMPACT_VIRTUAL_DISK_FLAG_NONE, &Parametros, 0) == ERROR_SUCCESS)
			{
				cout << "O disco selecionado foi compactado com sucesso.";
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

	cout << "O assistente está compactando um disco virtual...";

	//A operação de compactação é válida somente em discos dinâmicos, pois podem ser reduzidos.
	//Para discos dinâmicos, conforme o uso, podem aumentar capacidade ao seu tamanho máximo, e podem ser reduzidos facilmente.
	Funcoes.DesanexarDiscoVirtual(false, L"C:\\DISCOS VIRTUAIS\\DISCO.vhd");
	//--
	Funcoes.CompactarDiscoVirtual(false, L"C:\\DISCOS VIRTUAIS\\DISCO.vhd");

	system("pause");
}
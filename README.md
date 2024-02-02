# LoadLibrary-Basic

VirtualAllocEx - Aloca o caminho da dll no processo algo em bytes
WriteProcessMemory - Escreve o caminho da dll no processo algo em bytes
GetProcAddress - Pega o endereço da função LoadLibraryA no kernel32.dll (note que o endereço vai ser igual pra qualquer programa)
CreateRemoteThread - Chama a loadlibraryA com os argumentos definidos

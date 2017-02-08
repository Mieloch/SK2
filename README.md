# SK2
Aby zbudować i uruchomić serwer potrzebny jest python w wersji 3.5 oraz pakiet python3.5-dev
Polecenie:

make server


Program serwera oparty jest na prostym schemacie - serwer tworzy socket TCP i nasłuchuje w pętli while(1). Zgłoszenia przesyłane między serwerem a klientem to struktury
zawierające kod (int) oraz zadanie do wykonania (tablica char o długości 1024). Każde zgłoszenie od klienta
obsługuje nowy proces potomny. Jest w nim wywoływana funkcja process_request, która w zależności od kodu
zgłoszenia wywołuje obsługę zadania:
 - kod GET_CPU_USAGE uruchamia handle_cpu_usage_request odsyłającą do klienta aktualne zużycie procesora na serwerze
 - kod EXECUTE_JOB uruchamia handle_execute_job_request, która wykonuje otrzymany skrypt w pythonie i odsyła do klienta czas przetwarzania skryptu w sekundach
 

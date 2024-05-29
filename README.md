# csvreader
Консольное приложение для простой работы с csv-файлами.
Использование:

```cmd
windows:
  cd /path/to/application
  csvreader.exe <filepath> <query>
linux:
  cd /path/to/application
  ./csvreader <filepath> <query>
```


Запросы строятся в близком к SQL виде.
<> - обязательный аругмент
[] - необязательный агрумент
```SQL
"SELECT <column> [WHERE] [condition]"
"UPDATE SET <setter> [WHERE] [condition]"
"INSERT <value for columns in order>"
"DELETE [WHERE] [condition]"
```

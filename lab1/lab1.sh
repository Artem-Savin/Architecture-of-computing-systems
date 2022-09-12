#!/bin/bash
echo "Дата: $(date)" 
echo "Имя учетной записи:" $USER";"
echo "Доменное имя ПК:" $HOSTNAME";"
echo " Процессор: "
echo -n "Модель: "
cat /proc/cpuinfo | grep model | grep name | sort -u | awk '{print $4,$5,$6}'
echo -n "Архитектура: "
echo $(lscpu) | grep Architecture | sort -u | awk '{print $2}' 
echo -n "Тактовая частота: "
cat /proc/cpuinfo | grep model | grep name | sort -u | awk '{print $9}'
echo -n "Количество ядер:"
cat /proc/cpuinfo | grep cpu | grep cores | sort -u | awk -F: '{print $2}'
echo -n "Количество потоков на одно ядро: "
echo $(lscpu | grep "Thread(s) per core") | sort -u | awk '{print $4}'
echo " Оперативная память: "
echo -n "Всего: "
echo $(free -h| grep Mem) | sort -u | awk '{print $2}'
echo -n "Доступно: "
echo $(free -h| grep Mem) | sort -u | awk '{print $7}'
echo " Жесткий диск:"
echo -n "Всего: "
echo $(df --print-type -h| grep "drvfs" | awk '{print $3}')
echo -n "Доступно: "
echo $(df --print-type -h| grep "drvfs" | awk '{print $5}')
echo -n "Смонтировано в корневую директорию: "
echo $(df --print-type -h| grep "drvfs" | awk '{print $7}') 
echo -n "SWAP всего: "
echo $(free -h| grep Swap) | sort -u | awk '{print $2}'
echo -n "SWAP доступно: "
echo $(free -h| grep Swap) | sort -u | awk '{print $4}' 
echo " Сетевые интерфейсы: "
echo -n "Количество сетевых интерфейсов: "
str_ns_num=$(netstat -i | wc -l )
let int_num=$str_ns_num-2
echo $int_num 
let iter=$int_num+1
echo " Имя сетевого интерфейса: "
str=""
while [ $iter -gt 1 ]
do
int_name=$(echo $(netstat -i| grep -A$int_num "Iface" | awk '{print $1}') | sort -u | awk '{print $'$iter'}')
echo "$int_name " 
str+="$int_name "
let iter=$iter-1
done 
let iter=1
let end=$int_num+1
echo " MAC адрес: "
while [ $iter -lt $end ]
do
cur_int_name=$(echo $str | awk '{print $'$iter'}')
cat /sys/class/net/$cur_int_name/address
let iter=$iter+1 
done 
echo " IP адрес: "
let iter=1
while [ $iter -lt $end ]
do
cur_int_name=$(echo $str | awk '{print $'$iter'}')
cur_int_ip=$(echo $(ip address show dev $cur_int_name | grep "inet ") | sort -u | awk '{print $2}')
echo "$cur_int_ip "
let iter=$iter+1
done
echo " Скорость соединения: "
let iter=1
while [ $iter -lt $end ]
do
cur_int_name=$(echo $str | awk '{print $'$iter'}')
spd=$(echo $(< /sys/class/net/$cur_int_name/speed))
if [[ $spd -eq 0 ]] 
then
echo 'Cant get speed'
else
cat /sys/class/net/$cur_int_name/speed
fi
let iter=$iter+1
done
#EOF#
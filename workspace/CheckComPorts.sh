

N=$(($(ls /dev/ttyUSB* | wc -l) -1))

for i in `seq 0 $N`
do 
	echo ttyUSB$i
	udevadm info -a -n /dev/ttyUSB$i | grep '{serial}' | head -n1
done

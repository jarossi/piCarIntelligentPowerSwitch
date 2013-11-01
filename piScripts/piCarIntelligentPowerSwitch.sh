echo 7 > /sys/class/gpio/export 
echo 8 > /sys/class/gpio/export 
echo out > /sys/class/gpio/gpio7/direction
echo in > /sys/class/gpio/gpio8/direction
echo 1 > /sys/class/gpio/gpio7/value

while [ 1 = 1 ]; do
	sleep 1
	powerdown=$(cat /sys/class/gpio/gpio8/value)
	if [ $powerdown = 1 ]; then
		sleep 1
		poweroff
		break 
	fi
done

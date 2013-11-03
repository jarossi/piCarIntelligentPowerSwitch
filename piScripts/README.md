Copy the script to /etc and setup execution permissions.

chown root:root /etc/piCarIntelligentPowerSwitch.sh

chmod 750 /etc/piCarIntelligentPowerSwitch.sh

In /etc/rc.local add:

/etc/piCarIntelligentPowerSwitch.sh &
